#include "Multiplexer.hpp"

void I2cMultiplexer::Begin(uint8_t bus)
{
    Wire.beginTransmission(BASE_ADDRESS);
    Wire.write(bus<<1);
    
}