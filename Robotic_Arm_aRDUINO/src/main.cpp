#include <Arduino.h>
#include <PSX.h>
#include "MotorControl.hpp"
#include "Encoder.h"

#include <Wire.h>

#ifdef ARDUINO_SAMD_VARIANT_COMPLIANCE
  #define SERIAL SerialUSB
  #define SYS_VOL   3.3
#else
  #define SERIAL Serial
  #define SYS_VOL   5
#endif

const int NJOINTS = 4;

boolean Should_Exit = false;

//PS2 Controller
const uint8_t Data      = 12;
const uint8_t Cmd       = 11;
const uint8_t Attention = 10;
const uint8_t Clock     = 13;

/*
    MOTORTYPE_STEPPER
    index 0 = step pin; index 1 = dir pin
*/
const uint32_t base[]   = {3, 6};
const uint32_t link1[]  = {4, 7};

/*
    MOTORTYPE_NORMAL
    index 0 = Enable pin; index 1 = input1; index 2 = input2;
*/   
const uint32_t link2[]  = {11, 12, 10};
const uint32_t link3[]  = {9, 8, 7};

MotorController* motors   = nullptr;
End_Effector end_effector;

Magnetic_Encoder magnetic_encoder;
PSX psx;

PSX::PSXDATA PSXdata;
int PSXerror;


void setup()
{
  psx.setupPins(Data, Cmd, Attention, Clock, 10);
  psx.config(PSXMODE_ANALOG);
  SERIAL.begin(9600);
}


void loop() {
  // Read controller state
  PSXerror = psx.read(PSXdata);

  // Check if the read was successful
  if(PSXerror == PSXERROR_SUCCESS) {
      // Print the joystick states
      Serial.print("JoyLeft: X: ");
      Serial.print(PSXdata.JoyLeftX);
      Serial.print(", Y: ");
      Serial.print(PSXdata.JoyLeftY);
      Serial.print(", JoyRight: X: ");
      Serial.print(PSXdata.JoyRightX);
      Serial.print(", Y: ");
      Serial.print(PSXdata.JoyRightY);

      //Print the button states
      Serial.print(", Buttons: ");
      if(PSXdata.buttons & PSXBTN_LEFT) {
        Serial.print("Left, ");
      }
      if(PSXdata.buttons & PSXBTN_DOWN) {
        Serial.print("Down, ");
      }
      if(PSXdata.buttons & PSXBTN_RIGHT) {
        Serial.print("Right, ");
      }
      if(PSXdata.buttons & PSXBTN_UP) {
        Serial.print("Up, ");
      }
      if(PSXdata.buttons & PSXBTN_START) {
        Serial.print("Start, ");
      }
      if(PSXdata.buttons & PSXBTN_SELECT) {
        Serial.print("Select, ");
      }
      if(PSXdata.buttons & PSXBTN_SQUARE) {
        Serial.print("Square, ");
      }
      if(PSXdata.buttons & PSXBTN_CROSS) {
        Serial.print("Cross, ");
      }
      if(PSXdata.buttons & PSXBTN_CIRCLE) {
        Serial.print("Circle, ");
      }
      if(PSXdata.buttons & PSXBTN_TRIANGLE) {
        Serial.print("Triangle, ");
      }
      if(PSXdata.buttons & PSXBTN_R1) {
        Serial.print("R1, ");
      }
      if(PSXdata.buttons & PSXBTN_L1) {
        Serial.print("L1, ");
      }
      if(PSXdata.buttons & PSXBTN_R2) {
        Serial.print("R2, ");
      }
      if(PSXdata.buttons & PSXBTN_L2) {
        Serial.print("L2, ");
      }

      Serial.println("");
  } else {
    Serial.println("No success reading data. Check connections and timing.");
  }

  delay(10);
}
