// RealGpioPin.h
#pragma once
#include "igpiopin.h"
#include <gpiod.h>   // only included when USE_REAL_GPIO

namespace Gpio {

class RealGpioPin : public IGpioPin
{
    Q_OBJECT
public:
    explicit RealGpioPin(unsigned int offset, QObject* parent = nullptr);
    ~RealGpioPin() override;

    bool value() const override;
    void setValue(bool value) override;
    bool isReal() const override { return true; }

private:
    struct gpiod_chip* m_chip = nullptr;
    struct gpiod_line* m_line = nullptr;
};

} // namespace Gpio
