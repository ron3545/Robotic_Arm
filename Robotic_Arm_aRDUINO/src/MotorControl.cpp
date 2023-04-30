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