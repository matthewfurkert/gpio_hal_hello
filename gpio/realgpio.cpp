#include "realgpio.h"

RealGpio::RealGpio()
{
}

RealGpio::~RealGpio()
{
#ifdef USE_REAL_DEV
    if (m_request) {
        gpiod_line_request_release(m_request);
        m_request = nullptr;
    }
    if (m_gpiodChip) {
        gpiod_chip_close(m_gpiodChip);
        m_gpiodChip = nullptr;
    }
#endif
}

#ifdef USE_REAL_DEV
bool RealGpio::setChipNumber(int chip)
{
    if (chip < 0) return false;
    if (m_chipNumber == chip) return true;
    m_chipNumber = chip;
    if (m_pinNumber >= 0 && !m_request) {
        openAndRequestLine();
        return m_request != nullptr;
    }
    return true;
}
#endif

#ifdef USE_REAL_DEV
bool RealGpio::setPinNumber(int pin)
{
    if (pin < 0) return false;
    if (pin == m_pinNumber) return true;
    m_pinNumber = pin;
    if (m_chipNumber >= 0 && !m_request) {
        openAndRequestLine();
        return m_request != nullptr;
    }
    return true;
}
#endif

#ifdef USE_REAL_DEV
bool RealGpio::write(bool value)
{
    m_currentValue = value;

    if (!m_request) {
        if (m_chipNumber >= 0 && m_pinNumber >= 0) {
            openAndRequestLine();
        }
        if (!m_request) {
            std::cerr << "RealGpio: Cannot write - chip " << m_chipNumber
                      << " pin " << m_pinNumber << " not ready yet" << std::endl;
            return false;
        }
    }

    unsigned int offset = static_cast<unsigned int>(m_pinNumber);
    int ret = gpiod_line_request_set_value(m_request, offset,
                                           value ? GPIOD_LINE_VALUE_ACTIVE : GPIOD_LINE_VALUE_INACTIVE);
    if (ret < 0) {
        std::cerr << "Failed to set GPIO " << m_chipNumber << "/" << m_pinNumber
                  << " - error: " << strerror(errno) << std::endl;
        return false;
    }
    return true;
}
#endif

#ifdef USE_REAL_DEV
std::optional<bool> RealGpio::read() const
{
    if (!m_request)
        return {};

    unsigned int offset = static_cast<unsigned int>(m_pinNumber);
    int val = gpiod_line_request_get_value(m_request, offset);
    if (val < 0) {
        std::cerr << "Failed to read GPIO " << m_chipNumber << "/" << m_pinNumber
                  << " - error: " << strerror(errno) << std::endl;
        return {};
    }
    return val == GPIOD_LINE_VALUE_ACTIVE;
}
#endif

#ifdef USE_REAL_DEV
void RealGpio::openAndRequestLine()
{
    if (m_request || m_chipNumber < 0 || m_pinNumber < 0)
        return;

    std::string chipPath = "/dev/gpiochip" + std::to_string(m_chipNumber);
    m_gpiodChip = gpiod_chip_open(chipPath.c_str());
    if (!m_gpiodChip) {
        std::cerr << "Failed to open " << chipPath << std::endl;
        return;
    }

    struct gpiod_request_config *req_cfg = gpiod_request_config_new();
    if (req_cfg)
        gpiod_request_config_set_consumer(req_cfg, "qt_gpio_hal");

    struct gpiod_line_settings *settings = gpiod_line_settings_new();
    if (!settings) {
        std::cerr << "Failed to create line settings" << std::endl;
        gpiod_chip_close(m_gpiodChip);
        m_gpiodChip = nullptr;
        return;
    }

    gpiod_line_settings_set_direction(settings, GPIOD_LINE_DIRECTION_OUTPUT);
    gpiod_line_settings_set_output_value(settings,
                                         m_currentValue ? GPIOD_LINE_VALUE_ACTIVE : GPIOD_LINE_VALUE_INACTIVE);

    struct gpiod_line_config *line_cfg = gpiod_line_config_new();
    if (!line_cfg) {
        std::cerr << "Failed to create line config" << std::endl;
        gpiod_line_settings_free(settings);
        if (req_cfg) gpiod_request_config_free(req_cfg);
        gpiod_chip_close(m_gpiodChip);
        m_gpiodChip = nullptr;
        return;
    }

    unsigned int offset = static_cast<unsigned int>(m_pinNumber);
    if (gpiod_line_config_add_line_settings(line_cfg, &offset, 1, settings) < 0) {
        std::cerr << "Failed to add line settings for pin " << m_pinNumber << std::endl;
        gpiod_line_config_free(line_cfg);
        gpiod_line_settings_free(settings);
        if (req_cfg) gpiod_request_config_free(req_cfg);
        gpiod_chip_close(m_gpiodChip);
        m_gpiodChip = nullptr;
        return;
    }

    m_request = gpiod_chip_request_lines(m_gpiodChip, req_cfg, line_cfg);

    gpiod_line_config_free(line_cfg);
    gpiod_line_settings_free(settings);
    if (req_cfg)
        gpiod_request_config_free(req_cfg);

    if (m_request) {
        std::cerr << "Real GPIO requested successfully: " << m_chipNumber
                  << "/" << m_pinNumber << std::endl;
    } else {
        std::cerr << "Failed to request GPIO line " << m_chipNumber
                  << "/" << m_pinNumber << std::endl;
        gpiod_chip_close(m_gpiodChip);
        m_gpiodChip = nullptr;
    }
}
#endif