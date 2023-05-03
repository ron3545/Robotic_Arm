#include "MotorControl.hpp"
#include <Arduino.h>

#define STEP_PIN    0
#define DIR_PIN     1

#define ENABLE_PIN  0
#define INPUT1      1
#define INPUT2      2

MotorController::MotorController(const uint32_t *pins, size_t pin_arr_size, MotorType motor_type)
    :   current_angle(0.0f), m_pins(pins), pin_size(pin_arr_size), motor_type(motor_type), direction(MOTOR_DIRECTION_NONE)
{   
    for(uint32_t i = 0; i < pin_arr_size; ++i)
        pinMode(pins[i], OUTPUT);
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

void MotorController::MoveTo()
{   
    if(motor_type == MOTORTYPE_STEPPER)
    {
        digitalWrite(m_pins[DIR_PIN], (direction == MOTOR_DIRECTION_FORWARD)? HIGH : LOW );
        while (current_angle != target_angle)
        {
            float radius;
            digitalWrite(m_pins[STEP_PIN], (direction == MOTOR_DIRECTION_FORWARD)? HIGH : LOW);
            delay(500);
            digitalWrite(m_pins[STEP_PIN], (direction == MOTOR_DIRECTION_FORWARD)? LOW : HIGH);
            delay((joint_type == ENCODER_LOCATION_JOINT1)? map(joystic_analog_val, 700, 0, 100, 1000) : Calculate_Speed(radius));
        }

        //turn off
        digitalWrite(m_pins[STEP_PIN],LOW);
        digitalWrite(m_pins[DIR_PIN], LOW);
    }
    else if(motor_type == MOTORTYPE_NORMAL)
    {
        int pmw = map(joystic_analog_val, 0, 800, 0,255);
        analogWrite(m_pins[ENABLE_PIN], pmw); //speed

        while (current_angle != target_angle)
        {
            digitalWrite(m_pins[INPUT1], (direction == MOTOR_DIRECTION_FORWARD)? HIGH : LOW);
            digitalWrite(m_pins[INPUT2], (direction == MOTOR_DIRECTION_FORWARD)? LOW : HIGH);
        }

        //turn off
        digitalWrite(m_pins[INPUT1], LOW);
        digitalWrite(m_pins[INPUT2], LOW);

    }
}

double MotorController::Calculate_ArcLength(float theta, float radius)
{
    float rad = theta * (M_PI / 180);

    return rad * radius;
}

double MotorController::Calculate_Speed(float radius)
{
    float acceleration, velocity, distance;
    distance = Calculate_ArcLength(target_angle, radius);

    float time = 1/500; //time of delay from low to high state of stepper motor
    velocity = distance / time;

    acceleration = velocity/radius;
    return map(acceleration, 500, 0, 50, 1500);
}