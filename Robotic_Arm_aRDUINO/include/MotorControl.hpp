#pragma once
#include <Arduino.h>
#include <inttypes.h>
#include "Encoder.h"
#include "Mathematics.hpp"

#ifdef ARDUINO_SAMD_VARIANT_COMPLIANCE
  #define SERIAL SerialUSB
  #define SYS_VOL   3.3
#else
  #define SERIAL Serial
  #define SYS_VOL   5
#endif

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
    float speed;    

    const uint32_t* m_pins;     size_t pin_size;
    const uint32_t* m_encoder;  size_t encoder_size;

    MotorType       m_motor_type;
    End_Effector    m_end_effector_pos;
    MotorDirections m_direction;

public:
    //initialization of pins happens here
    MotorController(const uint32_t *pins, size_t pin_arr_size, const uint32_t* encoder_pins,
                    size_t encoder_pin_arr_size, MotorType motor_type);

    void SetDirection(const MotorDirections& direction) { m_direction = direction;}
    void SetSpeed(float speed)                          { this->speed = speed; }
    void Set_Position(End_Effector end)                 { m_end_effector_pos = end; } 

    void Run();

private:
    const float Get_CurrentPosition() const { return current_angle; }
    
};