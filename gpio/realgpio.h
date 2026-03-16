#ifndef REALGPIO_H
#define REALGPIO_H

#include "igpio.h"

#ifdef USE_REAL_DEV
#include <gpiod.h>
#endif

class RealGpio : public IGpio
{
public:
    explicit RealGpio();
    ~RealGpio() override;

#ifdef USE_REAL_DEV
    bool setChipNumber(int chip) override;
    bool setPinNumber(int pin) override;
    bool write(bool value) override;
    std::optional<bool> read() const override;
#endif

private:
#ifdef USE_REAL_DEV
    void openAndRequestLine();

    struct gpiod_chip *m_gpiodChip = nullptr;
    struct gpiod_line_request *m_request = nullptr;

    int m_chipNumber = -1;
    int m_pinNumber = -1;
    bool m_currentValue = false;
#endif
};

#endif // REALGPIO_H