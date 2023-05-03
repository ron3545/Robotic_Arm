#include <Arduino.h>
#include <PSX.h>
#include "MotorControl.hpp"
#include "Encoder.h"

#include <Wire.h>
#include <stdarg.h>

#define BASE_ADDRESS 0x70 //TCA9548A address
#define MAX_LENGTH(...) MaxLength(__VA_ARGS__);
#define RADIUS(maxlength) map(end_effector.x, 0, 255, 0, maxlength)

const int NJOINTS = 4;

boolean Should_Exit = false;

MotorController* motors       = nullptr;
Magnetic_Encoder* encoders    = nullptr;
Magnetic_Encoder encoder;
End_Effector end_effector;

PSX psx;
PSX::PSXDATA PSXdata;
int PSXerror;

//encoder 
double Joint1, Joint2, Joint3, Joint4;


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

String joint_names[] = { "Joint 1", "Joint 2", "Joint 3", "Joint 4"};

static float MaxLength(size_t n_args, ...);
static void Controller();
static void I2C_Multiplexer(uint8_t serial_bus);
static void Destroy_ptr();

void setup()
{
  encoders = new Magnetic_Encoder[NJOINTS];
  psx.setupPins(Data, Cmd, Attention, Clock, 10);
  psx.config(PSXMODE_ANALOG);

  Serial.begin(9600);
  Wire.begin();

  unsigned int encoder_uses[] = {
    ENCODER_LOCATION_JOINT1,  //Base
    ENCODER_LOCATION_JOINT2,
    ENCODER_LOCATION_JOINT3,
    ENCODER_LOCATION_JOINT4 
  };

  bool N_encoders_Initiated[NJOINTS];
  bool is_all_encoders_initiated = false;

  SERIAL.println("Initializing");

 while(!is_all_encoders_initiated)
  {
    Magnetic_Encoder encoder;

    for(unsigned int i = 0; i < NJOINTS; ++i)
    { 
      I2C_Multiplexer(i); //offset should only starts at 1
      encoder.Set_Encoder_loc(encoder_uses[i]);
      N_encoders_Initiated[i] = encoder.Initiate();
      encoders[i] = encoder;
    }

    //check if all encoders is initiated properly
    if(N_encoders_Initiated[0] && N_encoders_Initiated[1] && N_encoders_Initiated[2] && N_encoders_Initiated[3] )
      is_all_encoders_initiated = true;
    else 
      is_all_encoders_initiated = false;
  }
  SERIAL.println("All magnetic sensors has been initialized");
  
}

float convertRawAngleToDegrees(word newAngle)
{
  /* Raw data reports 0 - 4095 segments, which is 0.087890625 of a degree */
  float retVal = newAngle * 0.087890625;
  return retVal;
}

void loop() 
{ 
  for(unsigned int i = 0; i < NJOINTS; ++i)
  {
    I2C_Multiplexer(i);
    SERIAL.println(String(convertRawAngleToDegrees(encoder.getRawAngle()),DEC));
    SERIAL.println("\n \n =======================================================================");
  }
}

void Destroy_ptr()
{
  if(motors != nullptr)
    delete[] motors;
  
  if(encoders != nullptr)
    delete[] encoders;
}

void I2C_Multiplexer(uint8_t serial_bus)
{ 
  if(serial_bus > 7) return;

  Wire.beginTransmission(BASE_ADDRESS);
  Wire.write( 1 << serial_bus);
  Wire.endTransmission();
}

void Controller()
{
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
  }
}

float MaxLength(size_t n_args, ...)
{
  float max_length = 0;

  va_list args;
  va_start(args, n_args);

  for(size_t i = 0; i < n_args; ++i)
    max_length += va_arg(args, float);
  va_end(args);

  return max_length;
}