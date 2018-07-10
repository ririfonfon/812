
int test_led_niv = 50;

void pwm_show() {
#ifdef DEBUG
  Serial.println("pwm_show()");
#endif
  ledcWrite(ledChannela, led_a);
  ledcWrite(ledChannelb, led_b);
  ledcWrite(ledChannelc, led_c);
}//pw_show

void leds_show() {
  digitalLeds_updatePixels(strands[0]);
  digitalLeds_updatePixels(strands[1]);
  digitalLeds_updatePixels(strands[2]);
  digitalLeds_updatePixels(strands[3]);
  ledcWrite(ledChannela, led_a);
  ledcWrite(ledChannelb, led_b);
  ledcWrite(ledChannelc, led_c);
}//leds_show

void ledBlack() {
#ifdef DEBUG
  Serial.println("ledBlack()");
#endif
  for (int i = 0 ; i < NUM_LEDS_PER_STRIP ; i++) {
    strands[0]->pixels[i] = pixelFromRGB(0, 0, 0);
    strands[1]->pixels[i] = pixelFromRGB(0, 0, 0);
    strands[2]->pixels[i] = pixelFromRGB(0, 0, 0);
    strands[3]->pixels[i] = pixelFromRGB(0, 0, 0);
    led_a = 0;
    led_b = 0;
    led_c = 0;
  }//for i
  leds_show();
}//ledBlack

void Testfull() {
#ifdef DEBUG
  Serial.println("Testfull()");
#endif

  for (int i = 0 ; i < NUM_LEDS_PER_STRIP ; i++) {
    strands[0]->pixels[i] = pixelFromRGB(255, 255, 255);
    strands[1]->pixels[i] = pixelFromRGB(255, 255, 255);
    strands[2]->pixels[i] = pixelFromRGB(255, 255, 255);
    strands[3]->pixels[i] = pixelFromRGB(255, 255, 255);
  }//for i
  leds_show();
}//testfull

void Testfullg() {
#ifdef DEBUG
  Serial.println("Testfullg()");
#endif

  for (int i = 0 ; i < 13 ; i++) {
    if (i != 12) {
      //#ifdef DEBUG
      //      Serial.println("(i != 12)");
      //      Serial.print("i = ");
      //      Serial.println(i);
      //#endif
      strands[0]->pixels[i] = pixelFromRGB(255, 255, 255);
      strands[0]->pixels[i - 1] = pixelFromRGB(0, 0, 0);
      strands[1]->pixels[i] = pixelFromRGB(255, 255, 255);
      strands[2]->pixels[i] = pixelFromRGB(255, 255, 255);
      strands[3]->pixels[i] = pixelFromRGB(255, 255, 255);
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
      leds_show();
      delay(50);
    }
    else if (i = 12) {
      //#ifdef DEBUG
      //      Serial.println("(i = 12)");
      //      Serial.print("(i = )");
      //      Serial.println(i);
      //#endif
      strands[0]->pixels[i - 1] = pixelFromRGB(0, 0, 0);
      for (int j = 12 ; j < NUM_LEDS_PER_STRIP ; j++) {
        strands[0]->pixels[j] = pixelFromRGB(255, 255, 255);
      }
      var = var - 1 ;
  if (var < 200) {
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
      leds_show();
      delay(50);
      ledBlack();
//      delay(50);
    }
  }//for i
  leds_show();
}//testfullg

void initTest() {
#ifdef DEBUG
  Serial.println("initTest()");
#endif
  ledBlack();
  leds_show();
  delay(500);
  for (int i = 0 ; i < NUM_LEDS_PER_STRIP ; i++) {
    strands[0]->pixels[i] = pixelFromRGB(test_led_niv, 0, 0);
    strands[1]->pixels[i] = pixelFromRGB(test_led_niv, 0, 0);
    strands[2]->pixels[i] = pixelFromRGB(test_led_niv, 0, 0);
    strands[3]->pixels[i] = pixelFromRGB(test_led_niv, 0, 0);
  }//for i
  leds_show();

  for (int i = 0 ; i < NUM_LEDS_PER_STRIP ; i++) {
    strands[0]->pixels[i] = pixelFromRGB(0, test_led_niv, 0);
    strands[1]->pixels[i] = pixelFromRGB(0, test_led_niv, 0);
    strands[2]->pixels[i] = pixelFromRGB(0, test_led_niv, 0);
    strands[3]->pixels[i] = pixelFromRGB(0, test_led_niv, 0);
  }//for i
  leds_show();

  for (int i = 0 ; i < NUM_LEDS_PER_STRIP ; i++) {
    strands[0]->pixels[i] = pixelFromRGB(0, 0, test_led_niv);
    strands[1]->pixels[i] = pixelFromRGB(0, 0, test_led_niv);
    strands[2]->pixels[i] = pixelFromRGB(0, 0, test_led_niv);
    strands[3]->pixels[i] = pixelFromRGB(0, 0, test_led_niv);
  }//for i
  leds_show();
  ledBlack();
}//initest

void gpioSetup(int gpioNum, int gpioMode, int gpioVal) {
#if defined(ARDUINO) && ARDUINO >= 100
  pinMode (gpioNum, gpioMode);
  digitalWrite (gpioNum, gpioVal);
#elif defined(ESP_PLATFORM)
  gpio_num_t gpioNumNative = static_cast<gpio_num_t>(gpioNum);
  gpio_mode_t gpioModeNative = static_cast<gpio_mode_t>(gpioMode);
  gpio_pad_select_gpio(gpioNumNative);
  gpio_set_direction(gpioNumNative, gpioModeNative);
  gpio_set_level(gpioNumNative, gpioVal);
#endif
}// gpioSetup


void leds_init() {
  for (int k = 0; k < NUM_STRIPS; k++) {
    STRANDS[k] = {  .rmtChannel = k, .gpioNum = PINS[k], .ledType = LED_WS2812B_V3, .brightLimit = 32,
                    .numPixels = NUM_LEDS_PER_STRIP, .pixels = nullptr, ._stateVars = nullptr
                 };
    gpioSetup(PINS[k], OUTPUT, LOW);
  }
  int STRANDCNT = sizeof(STRANDS) / sizeof(STRANDS[0]);
  if (digitalLeds_initStrands(STRANDS, STRANDCNT)) {
#ifdef DEBUG
    Serial.println("Init FAILURE: halting");
    delay(1000);
#endif
    ESP.restart();
  }
  for (int i = 0; i < STRANDCNT; i++) {
    strand_t * pStrand = &STRANDS[i];
  }
#ifdef DEBUG
  Serial.println("Init complete");
#endif
}//leds_init
