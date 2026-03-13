#ifndef REALGPIOPIN_H
#define REALGPIOPIN_H
#include "igpiopin.h"
#include <QFile>

class RealGpioPin : public IGpioPin {
public:
    RealGpioPin();
    ~RealGpioPin() override;

    void write(bool value) override;
    bool read() const override;
    void setPinNumber(int pin) override;
    bool setActive(bool active) override;

private:
    int m_pin = -1;
    bool m_active = false;
    QFile m_valueFile;

    bool exportPin();
    bool unexportPin();
    bool setDirectionOut();
    QString gpioPath() const;
};

#endif // REALGPIOPIN_H