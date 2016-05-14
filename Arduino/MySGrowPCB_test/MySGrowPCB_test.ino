// CS eeprom pin
#define FLASH_SS 8        // D8

// The pin the ATSHA204 is connected on
#define ATSHA204_PIN 17   // A3

// OneWire pin
#define ONE_WIRE_BUS 4    // D4

#include <SPIFlash.h>     // From https://github.com/mysensors/Arduino/tree/development/libraries/SPIFlash as of May 2016 original from https://github.com/LowPowerLab/SPIFlash do not work with arduino 1.6.x
#include <sha204_library.h>
#include <sha204_lib_return_codes.h>
#include <SPI.h>
#include <OneWire.h>
#include <DallasTemperature.h>

SPIFlash flash(FLASH_SS);
const int sha204Pin = ATSHA204_PIN;
atsha204Class sha204(sha204Pin);

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup() {
  Serial.begin(115200);
  sensors.begin();
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
  Serial.println(F("-------- MySGrowLED: optional hardware test --------"));
  Serial.println(F("----------------------------------------------------"));
  Serial.println("Dallas sensor");
  sensors.requestTemperatures(); // Send the command to get temperatures
  float temperature = sensors.getTempCByIndex(0);
  if (temperature != -127.00 && temperature != 85.00) {
    Serial.print("Current temperature: ");
    Serial.print(temperature);
    Serial.println("°C");
  } else {
    Serial.println("Not found");
  }
  Serial.print(F("--------------------------"));

  Serial.println(F("--------------------------"));
  Serial.println(F("ATSHA204"));

  // Wake device before starting operations
  ret_code = sha204.sha204c_wakeup(rx_buffer);
  if (ret_code != SHA204_SUCCESS)
  {
    Serial.println(F("Not found"));
  }
  else {
    // Output device revision on console
    ret_code = sha204.sha204m_dev_rev(tx_buffer, rx_buffer);
    if (ret_code != SHA204_SUCCESS)
    {
      Serial.print(F("Failed to determine device revision. Response: ")); Serial.println(ret_code, HEX);
    }
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

    // Output serial number on console
    ret_code = sha204.getSerialNumber(rx_buffer);
    if (ret_code != SHA204_SUCCESS)
    {
      Serial.print(F("Failed to obtain device serial number. Response: ")); Serial.println(ret_code, HEX);
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
  }

  Serial.print(F("--------------------------"));

  Serial.println(F("--------------------------"));
  Serial.println(F("Eeprom"));
  flash.initialize();
  if (flash.readDeviceId() != 0)
  {
    Serial.print(F("Device ID: "));
    Serial.println(flash.readDeviceId(), HEX);
  }
  else {
    Serial.println(F("Not found"));
  }
  Serial.println(F("----------------------------------------------------"));
  Serial.println(F(""));
  Serial.println(F(""));

  delay(2500);

  Serial.println(F("----------------------------------------------------"));
  Serial.println(F("------------- MySGrowLED: mosfets test -------------"));
  Serial.println(F("----------------------------------------------------"));
}

void loop() {
  Serial.println(F("LED: on - Fan: off - Onboard LED: on"));
  analogWrite(5, 255);
  analogWrite(6, 0);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(10000);
  Serial.println(F("LED: off - Fan: on - Onboard LED: off"));
  analogWrite(5, 0);
  analogWrite(6, 255);
  digitalWrite(LED_BUILTIN, LOW);
  delay(10000);
}
