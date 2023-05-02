#include "MotorControl.hpp"
#include <Arduino.h>

MotorController::MotorController(const uint32_t *pins, size_t pin_arr_size, const uint32_t* encoder_pins, size_t encoder_pin_arr_size, MotorType motor_type)
    :   current_angle(0.0f), speed(10.0f), m_pins(pins), pin_size(pin_arr_size), m_encoder(encoder_pins), 
        encoder_size(encoder_pin_arr_size), m_motor_type(motor_type), m_direction(MOTOR_DIRECTION_NONE)
{
    if(m_motor_type == MOTORTYPE_NORMAL)
    {
        for(uint32_t i = 0; i < pin_arr_size; ++i)
        {
            if(i == 0)
                pinMode(pins[i], OUTPUT); //enable pin for speed control
            else
                pinMode(pins[i], INPUT);
        }
    }

    else if(m_motor_type == MOTORTYPE_STEPPER)
    {
        for(uint32_t i = 0; i < pin_arr_size; ++i)
            pinMode(pins[i], OUTPUT);
    }

}

 bool MotorController::Begin(unsigned int encoder_use)
 {
    encoder.Set_Encoder_loc(encoder_use);
    if(encoder.Initiate())
        return true;
    return false;
 }

 float MotorController::Get_CurrentAngle()
 {
    current_angle = convertRawAngleToDegrees(encoder.getRawAngle());
    return current_angle;
 }

float MotorController::convertRawAngleToDegrees(unsigned int newAngle)
{
  /* Raw data reports 0 - 4095 segments, which is 0.087890625 of a degree */
  float retVal = newAngle * 0.087890625;
  return retVal;
}

void MotorController::Run()
{
    if(joint_type == ENCODER_LOCATION_JOINT1)
    {
        
    }
}