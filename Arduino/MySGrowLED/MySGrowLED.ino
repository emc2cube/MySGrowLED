//////////////////////////
//     Configuration    //
//////////////////////////

// Customize your device name. Ex: "Greenhouse", "Grow rack", "Top shelf", etc...
String Location = "Plants";       // Location of the device, will be sent to the gateway with each device

// Enable/disable each module according to your hardware configuration
#define LED_CONTROL               // Enable to control a device using LED mosfet
#define FAN_CONTROL               // Enable to control a device using Fan mosfet or by PWM
//#define TEMPHUM_SENSOR            // Enable to monitor temperature and humidity using a SI7021 I2C module
//#define LUX_SENSOR                // Enable to monitor light level using a BH1750 I2C module
//#define DALLAS                    // Enable temperature monitoring using optional onboard dallas sensor or via OneWire terminal

// Use custom static node ID
// If disabled node ID will be automatically assigned
//#define MY_NODE_ID 42

// Use custom parent node ID. Use 0 to force direct connection to the gateway.
// If disabled node ID will be automatically assigned
//#define MY_PARENT_NODE_ID 0

// Enable repeater feature for this node
//#define MY_REPEATER_FEATURE

// Advanced options
// Use low power transmission for RF24, better signal/noise ratio but shorter range
//#define MY_RF24_PA_LEVEL RF24_PA_LOW
// Enable OTA feature
//#define MY_OTA_FIRMWARE_FEATURE         // Require eeprom (U3) on the PCB
// Enable message signing
//#define MY_SIGNING_ATSHA204             // Require ATSHA205 (U5) on the PCB and configured with your system key
// Enable debug prints to serial monitor
//#define MY_DEBUG

//////////////////////////
// End of configuration //
//////////////////////////

// MySensors
#define MY_RADIO_NRF24          // Enable and select radio type attached
#include <SPI.h>
#include <MySensors.h>
#define SN "MySGrowLED"         // Name of the sketch
#define SV "2.1.1"              // Version (2.x : use MySensors 2.x)
boolean metric = true;          // Use SI by default
#define MESSAGEWAIT 500         // Wait a few ms between radio Tx

#if defined LED_CONTROL
#define MosLED_PIN 5            // Arduino pin attached to MOSFET Gate pin
#define CHILD_ID_LED 0
#define EPROM_LED_LEVEL 1
#define FADE_DELAY 10           // Delay in ms for each percentage fade up/down (10ms = 1s full-range dim)
static int currentLEDlevel = 0;
MyMessage msgPercLED(CHILD_ID_LED, V_PERCENTAGE);
MyMessage msgStatLED(CHILD_ID_LED, V_STATUS);
#endif

#if defined FAN_CONTROL
#define MosFAN_PIN 6            // Arduino pin attached to MOSFET Gate pin or fan 4th pin (PWM)
#define CHILD_ID_FAN 1
#define EPROM_FAN_LEVEL 2
static int currentFANlevel = 0;
MyMessage msgPercFAN(CHILD_ID_FAN, V_PERCENTAGE);
MyMessage msgStatFAN(CHILD_ID_FAN, V_STATUS);
#endif

#if defined TEMPHUM_SENSOR
#include <Wire.h>
#include "i2c_SI7021.h"
#define CHILD_ID_TEMP 2
#define CHILD_ID_HUM 3
SI7021 si7021;
const unsigned long SLEEP_TEMPHUM = 150000;  // Sleep time between TempHum reads (in milliseconds), default 2min30 (150000ms)
unsigned long TempHumMillis = 0;
MyMessage msgTemp(CHILD_ID_TEMP, V_TEMP);
MyMessage msgHum(CHILD_ID_HUM, V_HUM);
#endif

#if defined LUX_SENSOR
#include <Wire.h>
#include <BH1750.h>
#define CHILD_ID_LUX 4
BH1750 lightSensor;
const unsigned long SLEEP_LUX = 150000;  // Sleep time between light levels reads (in milliseconds), default 2min30 (150000ms)
unsigned long LuxMillis = 0;
MyMessage msgLux(CHILD_ID_LUX, V_LIGHT_LEVEL);
#endif

#if defined DALLAS
#define ONE_WIRE_BUS 4                      // Pin where dallas sensor(s) is(are) connected
#define CHILD_ID_DALLAS 5
int numSensors = 0;
const unsigned long SLEEP_DALLAS = 150000;  // Sleep time between temperature reads (in milliseconds), default 2min30 (150000ms)
unsigned long DallasMillis = 0;
#include <OneWire.h>
#include <DallasTemperature.h>
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature dallas(&oneWire);
MyMessage msgDallas(CHILD_ID_DALLAS, V_TEMP);
#endif


void setup()
{
  metric = getControllerConfig().isMetric;  // was getConfig().isMetric; before MySensors v2.1.1

#if defined LED_CONTROL
  pinMode(MosLED_PIN, OUTPUT);
  //Fetch the last light state from the eprom
  currentLEDlevel = loadState(EPROM_LED_LEVEL);
  Serial.print(F("Led eprom state: "));
  Serial.print(currentLEDlevel > 0 ? F("ON, ") : F("OFF, "));
  Serial.print(currentLEDlevel);
  Serial.println(F("%"));
  fadeToLEDLevel(currentLEDlevel);
#endif

#if defined FAN_CONTROL
  pinMode(MosFAN_PIN, OUTPUT);
  //Fetch the last fan state from the eprom
  currentFANlevel = loadState(EPROM_FAN_LEVEL);
  Serial.print(F("Fan eprom state: "));
  Serial.print(currentFANlevel > 0 ? F("ON, ") : F("OFF, "));
  Serial.print(currentFANlevel);
  Serial.println(F("%"));
  analogWrite(MosFAN_PIN, currentFANlevel * 2.55 );
#endif

#if defined TEMPHUM_SENSOR
  // Start up SI7021 TempHum sensor
  si7021.initialize();
#endif

#if defined LUX_SENSOR
  // Start up BH1750 sensor
  lightSensor.begin();
#endif

#if defined DALLAS
  // Start up Dallas temperature probes
  dallas.begin();
#endif

  Serial.println(F("Node ready to receive messages"));

}


void presentation() {

  // Send the Sketch Version Information to the Gateway
  wait(MESSAGEWAIT);
  sendSketchInfo(SN, SV);

#if defined LED_CONTROL
  String ledReg = "LEDs " + Location;
  // Register the LED as a dimmable light with the gateway
  wait(MESSAGEWAIT);
  present(CHILD_ID_LED, S_DIMMER, ledReg.c_str());
#endif

#if defined FAN_CONTROL
  String fanReg = "Fan " + Location;
  // Register the Fan as a dimmable light with the gateway
  wait(MESSAGEWAIT);
  present(CHILD_ID_FAN, S_DIMMER, fanReg.c_str());
#endif

#if defined TEMPHUM_SENSOR
  String sitReg = "Temp " + Location;
  String sihReg = "Hum " + Location;
  wait(MESSAGEWAIT);
  present(CHILD_ID_TEMP, S_TEMP, sitReg.c_str());
  wait(MESSAGEWAIT);
  present(CHILD_ID_HUM, S_HUM, sihReg.c_str());
#endif

#if defined LUX_SENSOR
  String bhReg = "Lux " + Location;
  wait(MESSAGEWAIT);
  present(CHILD_ID_LUX, S_LIGHT_LEVEL, bhReg.c_str());
#endif

#if defined DALLAS
  // Fetch the number of attached temperature sensors
  numSensors = dallas.getDeviceCount();

  // Present all sensors to controller
  for (int i = CHILD_ID_DALLAS; i < numSensors + CHILD_ID_DALLAS; i++) {
    String dallasReg = "Temp " + Location + " " + String(i - CHILD_ID_DALLAS + 1);
    wait(MESSAGEWAIT);
    present(i, S_TEMP, dallasReg.c_str());
  }
#endif

  // Pull the current status from the controller
  Serial.println(F("Updating device status from the controller"));

#if defined LED_CONTROL
  wait(MESSAGEWAIT);
  request(CHILD_ID_LED, V_PERCENTAGE);
#endif

#if defined FAN_CONTROL
  wait(MESSAGEWAIT);
  request(CHILD_ID_FAN, V_PERCENTAGE);
#endif

}


void loop() {

  unsigned long currentMillis = millis();       // Start timer

#if defined TEMPHUM_SENSOR
  if (currentMillis - TempHumMillis >= SLEEP_TEMPHUM) {    // Every SLEEP_TIME fetch data from sensors and send to controller
    TempHumMillis = currentMillis;

    // Fetch temperature from sensor
    float temperature;
    si7021.getTemperature(temperature);
    temperature = round(temperature * 10) / 10.0; // 0.1°C accuracy is sensistive enough
    if (!metric) {
      temperature = temperature * 9.0 / 5.0 + 32.0;
    }
    wait(MESSAGEWAIT);
    send(msgTemp.set(temperature, 1));
#if defined MY_DEBUG
    Serial.print(F("Temperature: "));
    Serial.print(temperature);
    Serial.println(metric ? F(" °C") : F(" °F"));
#endif

    // Fetch humidity from sensor
    float humidity;
    si7021.getHumidity(humidity);
    humidity = round(humidity);     // 1% RH accuracy is sensistive enough
    // Clip humidity values to a valid range
    // If sent RH > 100% domoticz will ignore both temperature and humidity readings
    humidity = humidity > 100 ? 100 : humidity;
    humidity = humidity < 0   ? 0   : humidity;
    wait(MESSAGEWAIT);
    send(msgHum.set(humidity, 0));
#if defined MY_DEBUG
    Serial.print(F("Humidity: "));
    Serial.print(humidity);
    Serial.println(F(" %"));
#endif
  }
#endif

#if defined LUX_SENSOR
  if (currentMillis - LuxMillis >= SLEEP_LUX) {     // Every SLEEP_TIME fetch data from sensors and send to controller
    LuxMillis = currentMillis;
    // Fetch light from sensor
    uint16_t lux = lightSensor.readLightLevel();    // Get lux value
    wait(MESSAGEWAIT);
    send(msgLux.set(lux));
#if defined MY_DEBUG
    Serial.print(F("Light: "));
    Serial.print(lux);
    Serial.println(F(" lux"));
#endif
  }
#endif

#if defined DALLAS
  if (currentMillis - DallasMillis >= SLEEP_DALLAS) {    // Every SLEEP_TIME fetch data from sensors and send to controller
    DallasMillis = currentMillis;
    // call sensors.requestTemperatures() to issue a global temperature
    // request to all devices on the bus
#if defined MY_DEBUG
    Serial.print("Measuring  temperatures...");
#endif
    dallas.requestTemperatures(); // Send the command to get temperatures
#if defined MY_DEBUG
    Serial.println(" DONE");
#endif

    for (int i = 0; i < numSensors; i++) {
      // Fetch and round temperature to one decimal
      float temperature = static_cast<float>(static_cast<int>((getConfig().isMetric ? dallas.getTempCByIndex(i) : dallas.getTempFByIndex(i)) * 10.)) / 10.;
      if (temperature != -127.00 && temperature != 85.00) {
        // Send in the new temperature
        wait(MESSAGEWAIT);
        send(msgDallas.setSensor(CHILD_ID_DALLAS + i).set(temperature, 1));

#if defined MY_DEBUG
        // After we got the temperatures, we can print them.
        // We use the function ByIndex, and as an example get the temperature from the first sensor only.
        Serial.print("Temperature for the device ");
        Serial.print(i);
        Serial.print(" is: ");
        Serial.print(temperature);
        Serial.println(getConfig().isMetric ? "C" : "F");
#endif
      }
    }
  }
#endif

}


void receive(const MyMessage &message) {

  if (message.isAck()) {
#if defined MY_DEBUG
    Serial.println("ACK received");
#endif
  }

#if defined LED_CONTROL
  else if (((message.sensor == CHILD_ID_LED)) && (message.type == V_STATUS || message.type == V_PERCENTAGE)) {

    //  Retrieve the power or dim level from the incoming request message
    int requestedLevel = atoi(message.data);

    // Adjust incoming level if this is a V_LIGHT variable update [0 == off, 1 == on]
    requestedLevel *= (message.type == V_STATUS ? 100 : 1);

    // Clip incoming level to valid range of 0 to 100
    requestedLevel = requestedLevel > 100 ? 100 : requestedLevel;
    requestedLevel = requestedLevel < 0   ? 0   : requestedLevel;

#if defined MY_DEBUG
    Serial.print(F("Changing LED level from "));
    Serial.print(currentLEDlevel);
    Serial.print(F(" to "));
    Serial.println(requestedLevel);
#endif

    saveState(EPROM_LED_LEVEL, requestedLevel);
    //fadeToLEDLevel(requestedLevel);
    analogWrite(MosLED_PIN, requestedLevel * 2.55 );
    currentLEDlevel = requestedLevel;

    // Inform the gateway of the current dim level.
    wait(MESSAGEWAIT);
    send(msgPercLED.set(currentLEDlevel));
  }
#endif

#if defined FAN_CONTROL
  else if (((message.sensor == CHILD_ID_FAN)) && (message.type == V_STATUS || message.type == V_PERCENTAGE)) {

    //  Retrieve the power or dim level from the incoming request message
    int requestedLevel = atoi(message.data);

    // Adjust incoming level if this is a V_LIGHT variable update [0 == off, 1 == on]
    requestedLevel *= (message.type == V_STATUS ? 100 : 1);

    // Clip incoming level to valid range of 0 to 100
    requestedLevel = requestedLevel > 100 ? 100 : requestedLevel;
    requestedLevel = requestedLevel < 0   ? 0   : requestedLevel;

#if defined MY_DEBUG
    Serial.print(F("Changing Fan level from "));
    Serial.print(currentFANlevel);
    Serial.print(F(" to "));
    Serial.println(requestedLevel);
#endif

    saveState(EPROM_FAN_LEVEL, requestedLevel);
    analogWrite(MosFAN_PIN, requestedLevel * 2.55 );
    currentFANlevel = requestedLevel;

    // Inform the gateway of the current dim level.
    wait(MESSAGEWAIT);
    send(msgPercFAN.set(currentFANlevel));
  }
#endif

}


/**
    This method provides a graceful fade up/down effect for the LEDs
**/
void fadeToLEDLevel( int toLevel ) {
  int delta = ( toLevel - currentLEDlevel ) < 0 ? -1 : 1;

  while ( currentLEDlevel != toLevel ) {
    currentLEDlevel += delta;
    analogWrite(MosLED_PIN, fscale( 0, 100, 0, 255, currentLEDlevel, -2));  // Logarithmic dimming
    delay(FADE_DELAY);
  }
}

/* fscale
  Floating Point Autoscale Function V0.1
  Paul Badger 2007
  Modified from code by Greg Shakar
*/
float fscale( float originalMin, float originalMax, float newBegin, float newEnd, float inputValue, float curve)
{
  float   OriginalRange = 0;
  float   NewRange = 0;
  float   zeroRefCurVal = 0;
  float   normalizedCurVal = 0;
  float   rangedValue = 0;
  boolean invFlag = 0;

  // condition curve parameter
  // limit range
  if (curve > 10) curve = 10;
  if (curve < -10) curve = -10;

  curve = (curve * -.1) ; // - invert and scale - this seems more intuitive - postive numbers give more weight to high end on output
  curve = pow(10, curve); // convert linear scale into lograthimic exponent for other pow function

  // Check for out of range inputValues
  if (inputValue < originalMin) {
    inputValue = originalMin;
  }
  if (inputValue > originalMax) {
    inputValue = originalMax;
  }

  // Zero Refference the values
  OriginalRange = originalMax - originalMin;

  if (newEnd > newBegin) {
    NewRange = newEnd - newBegin;
  } else {
    NewRange = newBegin - newEnd;
    invFlag = 1;
  }

  zeroRefCurVal = inputValue - originalMin;
  normalizedCurVal  =  zeroRefCurVal / OriginalRange;   // normalize to 0 - 1 float

  // Check for originalMin > originalMax  - the math for all other cases i.e. negative numbers seems to work out fine
  if (originalMin > originalMax ) {
    return 0;
  }

  if (invFlag == 0) {
    rangedValue =  (pow(normalizedCurVal, curve) * NewRange) + newBegin;
  } else { // invert the ranges
    rangedValue =  newBegin - (pow(normalizedCurVal, curve) * NewRange);
  }

  return rangedValue;
}
