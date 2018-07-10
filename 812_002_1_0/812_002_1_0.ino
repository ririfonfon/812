/////////////////////////////////////////ID/////////////////////////////////////
// ID grande = 1  moyene = 2 petite = 3
#define NODE_NUMBER 1
#define VERSION 4
#define NUM_LEDS_PER_STRIP 13


/////////////////////////////////////////Debug///////////////////////////////////////
#define DEBUG 1

///////////////////////////////////////tsl2561///////////////////////////////////////
#include <SparkFunTSL2561.h>
#include <Wire.h>

// Create an SFE_TSL2561 object, here called "light":

SFE_TSL2561 light;

// Global variables:

boolean gain; // Gain setting, 0 = X1, 1 = X16;
unsigned int ms; // Integration ("shutter") time in milliseconds

/////////////////////////////////////////lib/////////////////////////////////////////
#include <WiFi.h>
unsigned long lastRefresh = 0;
#define REFRESH 10

#if defined(ARDUINO) && ARDUINO >= 100
// No extras
#elif defined(ARDUINO) // pre-1.0
// No extras
#elif defined(ESP_PLATFORM)
#include "arduinoish.hpp"
#endif

#define HBSIZE 32
char nodeName[HBSIZE];
byte myID;

///////////////////////////////Lib esp32_digital_led_lib//////////////////////////////
#include "esp32_digital_led_lib.h"
#define min(m,n) ((m)<(n)?(m):(n))
#define NUM_STRIPS 4
int PINS[NUM_STRIPS] = {23, 19, 18, 5};
const int numberOfChannels = NUM_STRIPS * NUM_LEDS_PER_STRIP * 3;
const int numberOfLed = NUM_STRIPS * NUM_LEDS_PER_STRIP ;
strand_t STRANDS[NUM_STRIPS];
strand_t * strands [] = { &STRANDS[0], &STRANDS[1], &STRANDS[2], &STRANDS[3]};


///////////////////////////////////dmx variables////////////////////////////////////
int var = 255;
int count_pwm = 0;
unsigned long now;

float r = 150;
float v = 150;
float b = 255;

int led_r = 255;
int led_v = 255;
int led_b = 255;



///////////////////////////////////////PWM setting/////////////////////////////////////////
int freq = 12000;
int resolution = 16;
int ledChannela = 4;
int ledChannelb = 5;
int ledChannelc = 6;
int ledPina = 0;
int ledPinb = 2;
int ledPinc = 15;
uint8_t ledArray[3] = {1, 2, 3}; // three led channels
const boolean invert = false; // set true if common anode, false if common cathode

///////////////////////////////////////////////// SETUP ////////////////////////////////////////
void setup() {
#ifdef DEBUG
  Serial.begin(115200);
#endif
  // NODE ID
#ifdef NODE_NUMBER
  eeprom_setID((byte)NODE_NUMBER);
#endif

  // NAME
  myID = eeprom_getID();
  String myName("Pira");
  sprintf(nodeName, "Pira %02i %i", myID, VERSION);
#ifdef DEBUG
  Serial.print("Starting ");
  Serial.println(nodeName);
#endif

  ///////////////////////////////////////tsl2561///////////////////////////////////////
  // Initialize the SFE_TSL2561 library

  light.begin();

  // Get factory ID from sensor:
  // (Just for fun, you don't need to do this to operate the sensor)

  unsigned char ID;

  if (light.getID(ID))
  {
    Serial.print("Got factory ID: 0X");
    Serial.print(ID, HEX);
    Serial.println(", should be 0X5X");
  }
  // Most library commands will return true if communications was successful,
  // and false if there was a problem. You can ignore this returned value,
  // or check whether a command worked correctly and retrieve an error code:
  else
  {
    byte error = light.getError();
    printError(error);
  }

  // The light sensor has a default integration time of 402ms,
  // and a default gain of low (1X).

  // If you would like to change either of these, you can
  // do so using the setTiming() command.

  // If gain = false (0), device is set to low gain (1X)
  // If gain = high (1), device is set to high gain (16X)

  gain = 0;

  // If time = 0, integration will be 13.7ms
  // If time = 1, integration will be 101ms
  // If time = 2, integration will be 402ms
  // If time = 3, use manual start / stop to perform your own integration

  unsigned char time = 0;

  // setTiming() will set the third parameter (ms) to the
  // requested integration time in ms (this will be useful later):

  Serial.println("Set timing...");
  light.setTiming(gain, time, ms);

  // To start taking measurements, power up the sensor:

  Serial.println("Powerup...");
  light.setPowerUp();

  // The sensor will now gather light during the integration time.
  // After the specified time, you can retrieve the result from the sensor.
  // Once a measurement occurs, another integration period will start.
  //  pinMode(0, OUTPUT);
  //  digitalWrite(0, HIGH);
  //  pinMode(2, OUTPUT);
  //  digitalWrite(2, HIGH);
  //  pinMode(15, OUTPUT);
  //  digitalWrite(15, HIGH);
  pwm_init();
  leds_init();
  // OTA
  ota_setup();
  initTest();
  Testfullg();

}//setup

///////////////////////////////////////// LOOP /////////////////////////////////////////////////
void loop() {
  delay(ms);
  if (count_pwm = 0) {
    r = r + 1;
    if (r > 255) {
      count_pwm = 1;
      r = 150;
    }
  } else if (count_pwm = 1) {
    v = v + 1;
    if (v > 255) {
      count_pwm = 0;
      v = 150;
    }
  }
  led_r = ((r * r) * 1.007852);
  led_v = ((v * v) * 1.007852);
  led_b = ((b * b) * 1.007852);
  pwm_show();

  // OTA
  ota_loop();


  ///////////////////////////////////////tsl2561///////////////////////////////////////
  // Retrieve the data from the device:

  unsigned int data0, data1;

  if (light.getData(data0, data1))
  {
    // To calculate lux, pass all your settings and readings
    // to the getLux() function.
    double lux; // Resulting lux value
    boolean good; // True if neither sensor is saturated
    // Perform lux calculation:
    good = light.getLux(gain, ms, data0, data1, lux);
    //if (good) Serial.println(" (good)"); else Serial.println(" (BAD)");
    if (lux > 500) Testfullg();
  }
  else
  {
    // getData() returned false because of an I2C error, inform the user.
    byte error = light.getError();
    printError(error);
  }
}//loop


