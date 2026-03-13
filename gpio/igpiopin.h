#ifndef IGPIOPIN_H
#define IGPIOPIN_H

class IGpioPin {
public:
    virtual ~IGpioPin() = default;
    virtual void write(bool value) = 0;
    virtual bool read() const = 0;
    virtual void setPinNumber(int pin) = 0;
    virtual bool setActive(bool active) = 0;   // export/unexport + direction
};

#endif // IGPIOPIN_H