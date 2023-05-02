#pragma once
#include <Arduino.h>
#include <inttypes.h>
#include "Encoder.h"
#include "Mathematics.hpp"

enum  MotorDirections 
{
    MOTOR_DIRECTION_NONE = 0,
    MOTOR_DIRECTION_FORWARD = 1,
    MOTOR_DIRECTION_REVERSE = 2
};

enum MotorType
{
    MOTORTYPE_STEPPER,
    MOTORTYPE_NORMAL
};

class MotorController
{
private:
    float current_angle;
    float target_angle;

    float speed;    

    const uint32_t* m_pins;     size_t pin_size;
    const uint32_t* m_encoder;  size_t encoder_size;

    MotorType       m_motor_type;
    MotorDirections m_direction;

    Magnetic_Encoder encoder;
    unsigned int joint_type;
public:
    //initialization of pins happens here
    MotorController(const uint32_t *pins, size_t pin_arr_size, const uint32_t* encoder_pins,
                    size_t encoder_pin_arr_size, MotorType motor_type);
    
    //this will initialize the encoders
    bool Begin(unsigned int encoder_use);

    void SetDirection(const MotorDirections& direction) { m_direction = direction;}
    void SetSpeed(float speed = 12)                     { this->speed = speed; }
    void Set_Target_Angle(float target_angle)           { this->target_angle = target_angle; }

    void Run();

private:
    float Get_CurrentAngle();
    float convertRawAngleToDegrees(unsigned int newAngle);
};