#include "realgpiopin.h"
#include <QDebug>

namespace Gpio {

RealGpioPin::RealGpioPin(unsigned int offset, QObject* parent)
    : IGpioPin(offset, parent)
    , m_offset(offset)
{
    m_chip = gpiod_chip_open("/dev/gpiochip0"); // change to /dev/gpiochip1 /dev/gpiochip2 if needed on your STM32MP15
    if (!m_chip) {
        qWarning() << "Failed to open /dev/gpiochip0";
        return;
    }

    // === libgpiod v2: full modern setup === (unchanged)
    struct gpiod_line_settings *settings = gpiod_line_settings_new();
    gpiod_line_settings_set_direction(settings, GPIOD_LINE_DIRECTION_OUTPUT);
    gpiod_line_settings_set_output_value(settings, GPIOD_LINE_VALUE_INACTIVE);

    struct gpiod_line_config *line_cfg = gpiod_line_config_new();
    gpiod_line_config_add_line_settings(line_cfg, &m_offset, 1, settings);

    struct gpiod_request_config *req_cfg = gpiod_request_config_new();
    gpiod_request_config_set_consumer(req_cfg, "gpio_hal_hello");

    m_request = gpiod_chip_request_lines(m_chip, req_cfg, line_cfg);

    // clean up temporary objects
    gpiod_line_settings_free(settings);
    gpiod_line_config_free(line_cfg);
    gpiod_request_config_free(req_cfg);

    if (!m_request) {
        qWarning() << "Failed to request GPIO line" << offset << "as output";
        gpiod_chip_close(m_chip);
        m_chip = nullptr;
    } else {
        qDebug() << "Real GPIO" << offset << "requested successfully (libgpiod v2)";
    }
}

RealGpioPin::~RealGpioPin()
{
    if (m_request) {
        gpiod_line_request_release(m_request);
        m_request = nullptr;
    }
    if (m_chip) {
        gpiod_chip_close(m_chip);
        m_chip = nullptr;
    }
}

bool RealGpioPin::value() const
{
    if (!m_request)
        return false;

    return gpiod_line_request_get_value(m_request, m_offset) == GPIOD_LINE_VALUE_ACTIVE;
}

void RealGpioPin::setValue(bool value)
{
    if (m_request) {
        enum gpiod_line_value new_val = value ? GPIOD_LINE_VALUE_ACTIVE : GPIOD_LINE_VALUE_INACTIVE;
        if (gpiod_line_request_set_value(m_request, m_offset, new_val) < 0)
            qWarning() << "Failed to set GPIO value on line" << m_offset;
        else
            emit valueChanged(value);
    }
}

} // namespace Gpio
