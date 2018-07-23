/////////////////////////////////////////ID/////////////////////////////////////
// ID grande = 1  moyene = 2 petite = 3
#define NODE_NUMBER 1
#define VERSION 4

/////////////////////////////////////////Adresse/////////////////////////////////////
// adresse grande = 1  moyene = 18 petite = 35
// strip   grande = 90 moyene = 78 petite = 66
#if defined(NODE_NUMBER) && NODE_NUMBER == 1
#define adr 1
#define NUM_LEDS_PER_STRIP 13
#elif defined (NODE_NUMBER) && NODE_NUMBER == 2
#define adr 18
#define NUM_LEDS_PER_STRIP 78
#elif defined (NODE_NUMBER) && NODE_NUMBER == 3
#define adr 35
#define NUM_LEDS_PER_STRIP 66
#endif

/////////////////////////////////////////Debug///////////////////////////////////////
#define DEBUG 1
//#define DEBUG_dmx 1
//#define DEBUG_STR 1

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
//#include <WiFiUdp.h>
//#include <ArtnetWifi.h>//https://github.com/rstephan/ArtnetWifi
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
bool randArray[numberOfLed];

///////////////////////////////////dmx variables////////////////////////////////////
int var = 255;
int count_pwm = 0;
unsigned long now;
int effet_etat = 0;
int last_time_effet = 0;
int type_effet = 0;
int modulo = 0;
unsigned int str_ws = 0;
unsigned int l_str;
int type_modulo = 0;
int str = 0;
int str_l = 0;
int pix_pos;
int pix_start;
int pix_center;
int pix_end;
int master;
float rrr;
float ggg;
float bbb;
float rr;
float gg;
float bb;
float r;
float g;
float b;
int smaster;
float srr;
float sgg;
float sbb;
float sr;
float sg;
float sb;
int mod;
int pix_mod;
unsigned long t_now_effet = 0;
uint8_t strob_count = 0;
uint8_t strob_count_l = 0;
unsigned long str_ws_last = 0;
unsigned long t_last_l = 0;
#define STROB_FRACTION 10
#define STROB_ON 1
#define STROB_ON_MS 30
#define STROB_FRACTION_L 10
#define STROB_ON_L 1
#define STROB_ON_MS_L 30
int l_s_count = 1;
int pix_start_v;
int ref_pix_pos;
float pix_coef;
float pix_coefi;
int M_g;
int M_g_ref;
int M_g1;
int M_g1_ref;
int M_g2;
int M_g2_ref;
int M_g3;
int M_g3_ref;
int M_g4;
int S_seuil;
int old_S_seuil = 0;
int S_count;
int a;
int led_a;
int led_b;
int led_c;
int led_master;
int led_mode;
int led_str;
float str_blind_ws = 1;
float str_blind_l = 1;

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
const boolean invert = true; // set true if common anode, false if common cathode

///////////////////////////////////// Artnet settings /////////////////////////////////////
ArtnetWifi artnet;
const int startUniverse = 0; // CHANGE FOR YOUR SETUP most software this is 1, some software send out artnet first universe as 0.

// Check if we got all universes
const int maxUniverses = numberOfChannels / 512 + ((numberOfChannels % 512) ? 1 : 0);
bool universesReceived[maxUniverses];
bool sendFrame = 1;
int previousDataLength = 0;

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
  //ConnectWifi();
  // OTA
  ota_setup();
  //artnet.begin();
  //artnet.setArtDmxCallback(onDmxFrame);
  initTest();
  Testfullg();

}//setup

///////////////////////////////////////// LOOP /////////////////////////////////////////////////
void loop() {
  delay(ms);
  var = var - 1 ;
  if (var < 230) {
    //    Serial.print("var < 200 ");
    var = 255;
    count_pwm = count_pwm + 1;
    if (count_pwm > 3) {
      count_pwm = 0;
    }
    //    Serial.print("count_pwm ");
    //    Serial.println(count_pwm);
  }
  led_master = ((var * var) * 1.007852);
  if (count_pwm == 0)  led_mode = 5;
  else if (count_pwm == 1)  led_mode = 15;
  else if (count_pwm == 2)  led_mode = 25;
  else if (count_pwm == 3)  led_mode = 35;
  else if (count_pwm == 4)  led_mode = 45;
  else if (count_pwm == 5)  led_mode = 55;
  else if (count_pwm == 6)  led_mode = 65;
  else if (count_pwm == 7)  led_mode = 75;
  else if (count_pwm == 8)  led_mode = 85;
  else if (count_pwm == 9)  led_mode = 95;
  else if (count_pwm == 10)  led_mode = 105;
  else if (count_pwm == 11)  led_mode = 115;
  else if (count_pwm == 12)  led_mode = 125;
  else if (count_pwm == 13)  led_mode = 135; 

  effet_led_mode();
  pwm_show();

  //  if (wifi_isok()) artnet.read();
  //  eff_modulo();
  //  effet();
  //  effet_led_mode();
  //  if ((millis() - lastRefresh) > REFRESH) {
  //    if (!wifi_isok())  ledBlack();//passe led noir
  //    lastRefresh = millis();
  //  }
  //  // MILLIS overflow protection
  //  if (millis() < lastRefresh) {
  //    lastRefresh = millis();
  //  }
  // OTA
  ota_loop();


  ///////////////////////////////////////tsl2561///////////////////////////////////////
  // Retrieve the data from the device:

  unsigned int data0, data1;

  if (light.getData(data0, data1))
  {
    // getData() returned true, communication was successful

    //Serial.print("data0: ");
    //Serial.print(data0);
    //Serial.print(" data1: ");
    //Serial.print(data1);

    // To calculate lux, pass all your settings and readings
    // to the getLux() function.

    double lux; // Resulting lux value
    boolean good; // True if neither sensor is saturated

    // Perform lux calculation:

    good = light.getLux(gain, ms, data0, data1, lux);

    // Print out the results:

    //Serial.print(" lux: ");
    //Serial.print(lux);
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

void printError(byte error)
// If there's an I2C error, this function will
// print out an explanation.
{
  Serial.print("I2C error: ");
  Serial.print(error, DEC);
  Serial.print(", ");

  switch (error)
  {
    case 0:
      Serial.println("success");
      break;
    case 1:
      Serial.println("data too long for transmit buffer");
      break;
    case 2:
      Serial.println("received NACK on address (disconnected?)");
      break;
    case 3:
      Serial.println("received NACK on data");
      break;
    case 4:
      Serial.println("other error");
      break;
    default:
      Serial.println("unknown error");
  }
}
