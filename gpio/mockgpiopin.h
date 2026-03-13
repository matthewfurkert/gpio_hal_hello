#ifndef MOCKGPIOPIN_H
#define MOCKGPIOPIN_H
#include "igpiopin.h"

class MockGpioPin : public IGpioPin {
public:
    void write(bool value) override { m_value = value; }
    bool read() const override { return m_value; }
    void setPinNumber(int pin) override { m_pin = pin; }
    bool setActive(bool active) override { m_active = active; return true; }

private:
    bool m_value = false;
    int m_pin = 0;
    bool m_active = false;
};

#endif // MOCKGPIOPIN_H