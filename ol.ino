#include <OneWire.h> 
#include <DallasTemperature.h>
#include <Wire.h>
#include <MechaQMC5883.h>
#include <Adafruit_GFX.h>

MechaQMC5883 qmc;

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

// Slave Flags
/*
#define DISPLAY 1 << 1
#define FORCE_DISPLAY 1 << 2
*/

boolean prevButton = false;   // previous pushbutton state
boolean prevTrigger = false;  // previous trigger state
int buttonState = 0;      // variable for reading the pushbutton status
int triggerState = 0;     // variable for reading the trigger status

OneWire oneWire(10);
DallasTemperature sensors(&oneWire);

uint_least8_t shDelay = 5;  // wait after shooting in millisecond
uint_least8_t dwell = 20;   // time the valve is open

#define CALCULATE_STUFF 60000

uint shootCnt = 0;
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


void setup()
{
  pinMode(LED_PIN, OUTPUT);  // initialize the LED pin as an output
  pinMode(BUTTON_PIN, INPUT);  // initialize the mode pin as an input
  pinMode(TRIGGER_PIN, INPUT); // initialize the trigger pin as an input
  pinMode(TEMP_PIN, INPUT);
  
  sensors.begin(); 

  calculateEtc();
  
  Serial.begin(9600);
  Wire.begin();
  qmc.init();
}


void loop()
{
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
  ShootAndModeDraw(shootCnt, mode);

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

  if (pMode != mode)
  {
    Wire.beginTransmission(2);
    Wire.write(mode);
    pMode = mode;
    Wire.endTransmission();
  }
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
    for (int16_t i = 0; i < n; i++)
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
  /*
Forced display
  ShootAndModeDraw(shootCnt, mode);

  display.display();
  displayed = true;
  */
  calculateEtc();

  Serial.print("Shot count: ");
  Serial.println(shootCnt);
}


void calculateEtc()
{
  static boolean first = true;
  float currentTime = millis();
  boolean isCacheInvalid = currentTime - lastEtcRenderTime > CALCULATE_STUFF || first;
  boolean isButtonPinTriggered = digitalRead(BUTTON_PIN) == HIGH;
  if (isCacheInvalid && !isButtonPinTriggered)
  {
    first = false;

    // Send Temp
    // sensors.requestTemperatures();
    // currentTemp = sensors.getTempCByIndex(0);
    
    lastEtcRenderTime = millis();
  }
}


void GraphUpdate(int &yPos)
{
  // Send new position (azimuth)
}


void ShootAndModeDraw(uint &shCnt, uint_least8_t &mode)
{
  // Send shCnt and mode to slave
}


uint_least8_t RateOfFire()
{
  return 1000 / (dwell + shDelay);
}