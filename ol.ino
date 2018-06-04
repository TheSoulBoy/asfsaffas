#include <OneWire.h> 
#include <DallasTemperature.h>
#include <Wire.h>
#include <MechaQMC5883.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeMono9pt7b.h>

Adafruit_SSD1306 display;
MechaQMC5883 qmc;

static const uint_least8_t PROGMEM yeti[] =
{ 
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07,
  0xFF, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xFF, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07,
  0xFF, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07,
  0xFF, 0xFF, 0x80, 0x00, 0x00, 0x00, 0x00, 0x07, 0xFF, 0xFF, 0x80, 0x00, 0x00, 0x00, 0x00, 0x07,
  0xFF, 0xFF, 0x80, 0x00, 0x00, 0x00, 0x00, 0x07, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07,
  0xFF, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xFF, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07,
  0xFF, 0xFE, 0x00, 0x00, 0x01, 0x80, 0x00, 0x07, 0xFF, 0xFE, 0x00, 0x00, 0x07, 0xC0, 0x00, 0x07,
  0xFF, 0xFE, 0x00, 0x00, 0x07, 0xE0, 0x00, 0x07, 0xFF, 0xFF, 0xF0, 0x00, 0x07, 0xE0, 0x00, 0x07,
  0xFF, 0xFF, 0xF0, 0x00, 0x07, 0xFF, 0xF0, 0x07, 0xFF, 0xFF, 0xFE, 0x00, 0x07, 0xFF, 0xFE, 0x07,
  0xFF, 0xFF, 0xFF, 0x00, 0x03, 0xFF, 0xFF, 0x87, 0xFF, 0xFF, 0xFF, 0x80, 0x03, 0xFF, 0xFF, 0xC7,
  0xFF, 0xFF, 0xFF, 0xC0, 0x0F, 0xFF, 0xFF, 0xC7, 0xFF, 0xFF, 0xFF, 0xC0, 0x0F, 0xFF, 0xFF, 0xE7,
  0xFF, 0xFF, 0xFF, 0xE0, 0x0F, 0xFF, 0xFF, 0xE7, 0xFF, 0xFF, 0xFF, 0xE0, 0x0F, 0xFF, 0xE1, 0xE7,
  0xFF, 0xFF, 0xFF, 0xE0, 0x0F, 0xFF, 0xE1, 0xE7, 0xFF, 0xFF, 0xFF, 0xC0, 0x3F, 0x7F, 0xE0, 0xF7,
  0xFF, 0xFF, 0xFF, 0x00, 0x3E, 0x3F, 0xE0, 0xF7, 0xFF, 0xFF, 0xFF, 0x00, 0x3E, 0x1F, 0xF1, 0xF7,
  0xFF, 0xFF, 0xFF, 0x80, 0x3C, 0x1F, 0xF1, 0x77, 0xFF, 0xFF, 0xFF, 0x80, 0x3C, 0x3F, 0xF0, 0x77,
  0xFF, 0xFF, 0xFF, 0xFC, 0x3E, 0x7F, 0xF0, 0x77, 0xFF, 0xFF, 0xFF, 0xFE, 0x3E, 0xFF, 0xFF, 0x77,
  0xFF, 0xFF, 0xFF, 0xFE, 0x3E, 0xFF, 0xFF, 0x37, 0xFF, 0xFF, 0xFF, 0xFE, 0x38, 0xFE, 0xFF, 0x87,
  0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x78, 0x3F, 0xC7, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x7C, 0x07, 0xE7,
  0xFF, 0xFF, 0xFF, 0xFF, 0x80, 0x3E, 0x01, 0xE7, 0xFF, 0xFF, 0xFF, 0xFF, 0x80, 0x3E, 0x01, 0xC7,
  0xFF, 0xFF, 0xFF, 0xFF, 0x80, 0x7E, 0x1F, 0xC7, 0xFF, 0xFF, 0xFF, 0xFF, 0x83, 0xFF, 0x0F, 0x87,
  0xFF, 0xFF, 0xFF, 0xFF, 0xF1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE3, 0x37, 0x19, 0x9B, 0x10, 0x63, 0x8E, 0xFB,
  0xCB, 0x36, 0xED, 0x9A, 0x5D, 0xDD, 0xB6, 0x73, 0xCF, 0x14, 0xE5, 0x9A, 0x7D, 0xDD, 0xB6, 0x73,
  0xE3, 0x54, 0xE5, 0x13, 0x1D, 0x9C, 0x8E, 0xAB, 0xFB, 0x44, 0xE4, 0x47, 0xDD, 0xDD, 0xAE, 0xAB,
  0xDB, 0x66, 0xEE, 0x66, 0xDD, 0xDD, 0xB6, 0xDB, 0xC3, 0x77, 0x1E, 0xEE, 0x1D, 0xE3, 0xB6, 0xDB,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};

// PINS --------------------------
// the number of the mode pin
#define BUTTON_PIN 8
// the number of the trigger pin
#define TRIGGER_PIN 11
// the number of the LED (out) pin
#define LED_PIN 13
#define TEMP_PIN 10

// Bluetooth variables
#define CONNECTED -1
#define DISCONNECTED -2
// char BTState = DISCONNECTED;


boolean prevButton = false;   // previous pushbutton state
boolean prevTrigger = false;  // previous trigger state
boolean displayed = false;    // has display been already refreshed
int buttonState = 0;      // variable for reading the pushbutton status
int triggerState = 0;     // variable for reading the trigger status


OneWire oneWire(10);
DallasTemperature sensors(&oneWire);

// wait after shooting in millisecond
uint_least8_t shDelay = 5;
uint_least8_t dwell = 20;   // time the valve is open

#define CALCULATE_STUFF 60000

unsigned int shootCnt = 0;
float currentTemp = -420.0f;
uint_least32_t lastRenderTime = millis();
uint_least32_t lastEtcRenderTime = millis();

uint_least8_t mode = 0;

// cia burst mode kiekis
#define BURST_AMOUNT 3 

#define MY_SIZE 25
const int16_t pausesBetweenShots[] = 
{
  125, 125,
  250, 250, 250,
  500, 125, 125,
  250, 250, 250,
  500, 125, 125,
  250, 250, 250,
  250, 250, 125, 125,
  250, 250, 250,
  750
}; // Happy birthday ritmas
//sesioliktine - 125, astuntine - 250, ketvirtine - 500

// float lengthMultiplier = 1; // jeigu per greitai groja ritma tai padidink

uint_least8_t graphY[128];


void setup()
{
  pinMode(LED_PIN, OUTPUT);  // initialize the LED pin as an output
  pinMode(BUTTON_PIN, INPUT);  // initialize the mode pin as an input
  pinMode(TRIGGER_PIN, INPUT); // initialize the trigger pin as an input
  pinMode(TEMP_PIN, INPUT);
  
  sensors.begin(); 

  calculateEtc();
  
  Serial.begin(9600);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  Wire.begin();
  qmc.init();

  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setRotation(0);
  display.setTextWrap(false);
  display.dim(0);

  // miniature bitmap display
  display.drawBitmap(0, 0, yeti, 64, 60, WHITE);
  display.display();

  for(uint_least8_t xval = 0; xval <= 124; xval++)
  {
    uint_least8_t y = 63;
    display.drawPixel((int)xval, (int)y, WHITE);
    display.drawPixel((int)xval, (int)y - 1, WHITE);
    display.display();
    delay(1); 
  }
}


void loop()
{
  displayed = false;
// STATE READING
  buttonState = digitalRead(BUTTON_PIN);
  triggerState = digitalRead(TRIGGER_PIN);

// MODE SWITCH
  if (buttonState == HIGH && !prevButton)
  {
    // cycle modes 0 to 4
    if (++mode == 5)
    {
      mode = 0;
    }

    // prints current mode to console
    Serial.print("Master Mode: ");
    Serial.println(mode);

    // prevents multiple switching
    prevButton = true;
  }
  else if (buttonState != HIGH)
  {
    prevButton = false;
  }


// SHOTING
  if (triggerState == HIGH)
  {
    switch (mode)
    {
      case 0:
        break;
      case 1:
        semi(!prevTrigger);
        break;
      case 2:
        full();
        break;
      case 3:
        burst(BURST_AMOUNT, !prevTrigger);
        break;
      case 4:
        rythmic();
        break;
    }
    prevTrigger = true;
  }
  else
  {
    prevTrigger = false;
  }

// BLUETOOTH
/*  if (Serial.available() > 0)
  {
    int16_t data = Serial.read();

    Serial.print("READ: ");
    Serial.println(data);

    if (data == CONNECTED)
    {
      BTState
      Serial.println("Connected!");
    }
    else if (data == DISCONNECTED)
    {
      Serial.println("Disconnected!");
    }
    if (0 < data && data < 51)
    {
      dwell = (uint_least8_t)data;
      Serial.print("dwell val: ");
      Serial.println(dwell);
    }
    else if (50 < data && data < 101)
    {
      shDelay = (uint_least8_t)data - 50;
      Serial.print("Delay val: ");
      Serial.println(shDelay);      
    }
  }*/

// DISPLAY UPDATES
  /*display.setCursor(0, 0);
  display.println(RateOfFire());
  
  calculateEtc();
  ShootAndModeDraw(shootCnt, 
    mode == 0 ? "Safe" :
    mode == 1 ? "Semi" :
    mode == 2 ? "Auto" :
    mode == 3 ? "Burst" :
    mode == 4 ? "Rhythmic" : "");

  GraphDraw();
  
  {
    int x, y, z, azimuth;
    qmc.read(&x, &y, &z, &azimuth);
    GraphUpdate(azimuth);
  }  
  
  if (!displayed)
  {
    display.display();
  }*/

// SLAVE COMMANDS
  static uint_least8_t pMode = 8;

  Wire.beginTransmission(9);

  if (pMode != mode)
  {
    Wire.write(mode);
    pMode = mode;
  }
  
  Wire.endTransmission();
}


void semi(boolean released)
{
  if (released)
  {
    shoot();
  }
  else
  {
    Serial.println("wait"); 
  }
}


void full()
{
  shoot();
}


void burst(int16_t n, boolean released)
{
  if (released)
  {
    for (int16_t i = 0; i < n; ++i) 
    {
      shoot();
    }
  }
  else
  {
    Serial.println("wait"); 
  }
}


void rythmic()
{
  int pauseCount = 0;
  while(pauseCount < MY_SIZE)
  {
    shoot();
    delay(pausesBetweenShots[pauseCount]);
    pauseCount++;
  }
}


void shoot()
{
  delay(shDelay);
  digitalWrite(LED_PIN, HIGH);
  delay(dwell);
  digitalWrite(LED_PIN, LOW);
  shootCnt++;
  ShootAndModeDraw(shootCnt, 
    mode == 0 ? "Safe" :
    mode == 1 ? "Semi" :
    mode == 2 ? "Auto" :
    mode == 3 ? "Burst" :
    mode == 4 ? "Rhythmic" : "");

  display.display();
  displayed = true;
  
  calculateEtc();

  Serial.print("Shot count: ");
  Serial.println(shootCnt);
}


void calculateEtc()
{
  float currentTime = millis();
  boolean isCacheInvalid = currentTime - lastEtcRenderTime > CALCULATE_STUFF || currentTemp == -420;
  boolean isButtonPinTriggered = digitalRead(BUTTON_PIN) == HIGH;
  if (isCacheInvalid && !isButtonPinTriggered)
  {
    currentTemp = calcTemp();
    lastEtcRenderTime = millis();
  }
}


float calcTemp()
{
  sensors.requestTemperatures();
  return sensors.getTempCByIndex(0);
}


void GraphUpdate(int &yPos)
{
  for (uint_least8_t i = 0; i < 127; ++i)
  {
    graphY[(int16_t)i] = graphY[(int16_t)i + 1];
  }
  graphY[127] = yPos;
}


void GraphDraw()
{
  for (uint_least8_t i = 0; i < 128; ++i)
  {
    display.drawPixel((int16_t)i, 
      64 - (int16_t)graphY[i] * 32 / 360, 
      WHITE);
  }
}


void ShootAndModeDraw(unsigned int &shCnt, const char* text)
{
  char string[10];
  dtostrf(shCnt, 3, 0, string);
  display.clearDisplay();
  display.setFont(&FreeMono9pt7b);
  display.setTextSize(0);

  // Print text
  display.setCursor(0, 10);
  display.println(text);

  display.setFont(&FreeMono9pt7b);  // Set a custom font
  
  // Print variable with left alignment:
  display.setCursor(64, 10);
  display.println(shCnt);
      
  display.setCursor(0, 30);
  display.println(currentTemp);
}


uint_least8_t RateOfFire()
{
  return 1000 / (dwell + shDelay);
}