// GpioFactory.h
#pragma once
#include "igpiopin.h"

namespace Gpio {

class GpioFactory
{
public:
    static IGpioPin* createPin(unsigned int offset = 10);
};

} // namespace Gpio
