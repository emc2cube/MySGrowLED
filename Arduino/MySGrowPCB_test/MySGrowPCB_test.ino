// CS eeprom pin
#define FLASH_SS 8        // D8

// The pin the ATSHA204 is connected on
#define ATSHA204_PIN 17   // A3

#include <SPIFlash.h>     // From https://github.com/LowPowerLab/SPIFlash
#include <sha204_library.h>
#include <sha204_lib_return_codes.h>
#include <SPI.h>

SPIFlash flash(FLASH_SS);
const int sha204Pin = ATSHA204_PIN;
atsha204Class sha204(sha204Pin);

void setup() {
  Serial.begin(115200);

  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  analogWrite(5, 0);
  analogWrite(6, 0);
  analogWrite(13, 0 );

  uint8_t tx_buffer[SHA204_CMD_SIZE_MAX];
  uint8_t rx_buffer[SHA204_RSP_SIZE_MAX];
  uint8_t key[32];
  uint8_t ret_code;
  uint8_t lockConfig = 0;
  uint8_t lockValue = 0;
  uint16_t crc;

  Serial.println(F("----------------------------------------------------"));
  Serial.println(F("ATSHA204"));

  // Wake device before starting operations
  ret_code = sha204.sha204c_wakeup(rx_buffer);
  if (ret_code != SHA204_SUCCESS)
  {
    Serial.print(F("Failed to wake device. Response: ")); Serial.println(ret_code, HEX);
    halt();
  }

  // Output device revision on console
  ret_code = sha204.sha204m_dev_rev(tx_buffer, rx_buffer);
  if (ret_code != SHA204_SUCCESS)
  {
    Serial.print(F("Failed to determine device revision. Response: ")); Serial.println(ret_code, HEX);
    halt();
  }
  else
  {
    Serial.print(F("Device revision: "));
    for (int i = 0; i < 4; i++)
    {
      if (rx_buffer[SHA204_BUFFER_POS_DATA + i] < 0x10)
      {
        Serial.print('0'); // Because Serial.print does not 0-pad HEX
      }
      Serial.print(rx_buffer[SHA204_BUFFER_POS_DATA + i], HEX);
    }
    Serial.println();
  }

  // Output serial number on console
  ret_code = sha204.getSerialNumber(rx_buffer);
  if (ret_code != SHA204_SUCCESS)
  {
    Serial.print(F("Failed to obtain device serial number. Response: ")); Serial.println(ret_code, HEX);
    halt();
  }
  else
  {
    Serial.print(F("Device serial:   "));
    for (int i = 0; i < 9; i++)
    {
      if (rx_buffer[i] < 0x10)
      {
        Serial.print('0'); // Because Serial.print does not 0-pad HEX
      }
      Serial.print(rx_buffer[i], HEX);
    }
    Serial.println();
  }
  Serial.print(F("--------------------------"));

  Serial.println(F("--------------------------"));
  Serial.println(F("Eeprom"));
  if (flash.initialize())
  {
    Serial.print(F("Eeprom DeviceID: "));
    Serial.println(flash.readDeviceId(), HEX);
  }
  else {
    Serial.println(F("Not found."));
  }
  Serial.println(F("----------------------------------------------------"));

  delay(1000);
}

void loop() {
  analogWrite(5, 0);
  analogWrite(6, 255);
  digitalWrite(LED_BUILTIN, LOW);
  delay(10000);
  analogWrite(5, 255);
  analogWrite(6, 0);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(10000);
}

void halt()
{
  Serial.println(F("Halting!"));
  while (1);
}
