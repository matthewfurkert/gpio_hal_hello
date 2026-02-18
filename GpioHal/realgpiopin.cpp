// RealGpioPin.cpp
#include "realgpiopin.h"
#include <QDebug>

#ifdef USE_REAL_GPIO
#include <gpiod.h>
#endif

namespace Gpio {

RealGpioPin::RealGpioPin(unsigned int offset, QObject* parent)
    : IGpioPin(offset, parent)
{
#ifdef USE_REAL_GPIO
    m_chip = gpiod_chip_open_by_name("gpiochip0");  // change chip name if needed
    if (!m_chip) {
        qWarning() << "Failed to open gpiochip0";
        return;
    }
    m_line = gpiod_chip_get_line(m_chip, offset);
    if (!m_line || gpiod_line_request_output(m_line, "gpio_hal_hello", 0) < 0) {
        qWarning() << "Failed to request GPIO line" << offset;
    } else {
        qDebug() << "Real GPIO" << offset << "requested (libgpiod)";
    }
#else
    qWarning() << "RealGpioPin compiled without USE_REAL_GPIO - this should never happen";
#endif
}

RealGpioPin::~RealGpioPin()
{
#ifdef USE_REAL_GPIO
    if (m_line) gpiod_line_release(m_line);
    if (m_chip) gpiod_chip_close(m_chip);
#endif
}

bool RealGpioPin::value() const
{
#ifdef USE_REAL_GPIO
    return m_line ? gpiod_line_get_value(m_line) : false;
#else
    return false;
#endif
}

void RealGpioPin::setValue(bool value)
{
#ifdef USE_REAL_GPIO
    if (m_line) {
        gpiod_line_set_value(m_line, value ? 1 : 0);
        emit valueChanged(value);
    }
#endif
}

} // namespace Gpio
