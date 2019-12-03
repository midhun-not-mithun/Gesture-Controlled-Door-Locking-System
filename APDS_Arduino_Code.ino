 /****************************************************************
 3.3V         VCC              Power
 GND          GND              Ground
 A4           SDA              I2C Data
 A5           SCL              I2C Clock
 2            INT              Interrupt

Resources:
Include Wire.h and SparkFun_APDS-9960.h

Development environment specifics:
Written in Arduino 1.0.5
Tested with SparkFun Arduino Pro Mini 3.3V

This code is beerware; if you see me (or any other SparkFun
employee) at the local, and you've found our code helpful, please
buy us a round!

Distributed as-is; no warranty is given.
****************************************************************/

#include <Wire.h>
#include <SparkFun_APDS9960.h>
#include <Servo.h>

Servo myservo;

// Pins
#define APDS9960_INT    2 // Needs to be an interrupt pin

// Constants

// Global Variables
SparkFun_APDS9960 apds = SparkFun_APDS9960();
int isr_flag = 0;
int sub_flag1=0;
int sub_flag2=0;
int sub_flag3=0;
int sub_flag4=0;
int sub_flag5=0;
int sub_flag6=0;
int master_flag=0;


void setup() {

  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
  // Set interrupt pin as input
  pinMode(APDS9960_INT, INPUT);

  // Initialize Serial port
  Serial.begin(9600);
  Serial.println();
  Serial.println(F("--------------------------------"));
  Serial.println(F("SparkFun APDS-9960 - GestureTest"));
  Serial.println(F("--------------------------------"));
 
  // Initialize interrupt service routine
  attachInterrupt(0, interruptRoutine, FALLING);

  // Initialize APDS-9960 (configure I2C and initial values)
  if ( apds.init() ) {
    Serial.println(F("APDS-9960 initialization complete"));
  } else {
    Serial.println(F("Something went wrong during APDS-9960 init!"));
  }
 
  // Start running the APDS-9960 gesture sensor engine
  if ( apds.enableGestureSensor(true) ) {
    Serial.println(F("Gesture sensor is now running"));
  } else {
    Serial.println(F("Something went wrong during gesture sensor init!"));
  }
}

void loop() {
  if( isr_flag == 1 ) {
    detachInterrupt(0);
    handleGesture();
    isr_flag = 0;
    attachInterrupt(0, interruptRoutine, FALLING);
  }
}

void interruptRoutine() {
  isr_flag = 1;
}

void handleGesture() {
    if ( apds.isGestureAvailable() ) {
    switch ( apds.readGesture() ) {
      case DIR_UP:
        Serial.println("UP");
        sub_flag1=1;
        break;
      case DIR_DOWN:
        Serial.println("DOWN");
        sub_flag2=1;
        break;
      case DIR_LEFT:
        Serial.println("LEFT");
        sub_flag3=1;
        break;
      case DIR_RIGHT:
        Serial.println("RIGHT");
        sub_flag4=1;
        break;
      case DIR_NEAR:
        Serial.println("NEAR");
        sub_flag5=1;
        break;
      case DIR_FAR:
        Serial.println("FAR");
        sub_flag6=1;
        break;
      default:
        Serial.println("NONE");
    }
  }
  //if( (sub_flag1==1) && (sub_flag2==1) && (sub_flag3==1) && (sub_flag4==1) && (sub_flag5==1) && (sub_flag6==1) )
  if( (sub_flag3==1) && (sub_flag4==1) )
  {
    master_flag=1;
  }
  if (master_flag==1)
  {
    myservo.write(180);
    //delay(1000);
    myservo.write(180);
    delay(1000);
  }
 
}
//baud rate=9600
