#pragma once
#include <Wire.h>
#include <Arduino.h>

extern const int NJOINTS;

#define SDA 0x12 //pin a4
#define SCL 0x13 //pin a5

#define BASE_ADDRESS 0x70

//solving Address conflicts
class I2cMultiplexer
{
private:
    I2cMultiplexer();
public:
    static I2cMultiplexer& GetInstance()
    {
        static I2cMultiplexer instance;
        return instance;
    }

    void Begin(uint);
};

