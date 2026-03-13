#include "gpiopin.h"
#include <QDebug>

// === Backend selection ===
#ifdef USE_REAL_DEV
#include "realgpiopin.h"
#else
#include "mockgpiopin.h"
#endif

GpioPin::GpioPin(QObject *parent)
    : QObject(parent)
{
#ifdef USE_REAL_DEV
    qDebug() << "=== GpioPin MODE: REAL GPIO (Hardware/sysfs) ===";
    createBackend();
#else
    qDebug() << "=== GpioPin MODE: MOCK (Simulation) ===";
    m_backend = std::make_unique<MockGpioPin>();
#endif

    if (m_backend) {
        m_backend->setPinNumber(m_pin);
        m_backend->setActive(m_enabled);
    }
}

void GpioPin::createBackend()
{
#ifdef USE_REAL_DEV
    m_backend = std::make_unique<RealGpioPin>();
#endif
}

// ============== Getters ==============
bool GpioPin::enabled() const { return m_enabled; }
bool GpioPin::status() const { return m_status; }
int GpioPin::pin() const { return m_pin; }

// ============== Setters (emit only on change) ==============
void GpioPin::setEnabled(bool enabled)
{
    if (m_enabled == enabled) return;
    m_enabled = enabled;
    if (m_backend) m_backend->setActive(enabled);
    emit enabledChanged();
}

void GpioPin::setStatus(bool status)
{
    if (m_status == status || !m_enabled || !m_backend) return;
    m_backend->write(status);
    m_status = status;
    emit statusChanged();
}

void GpioPin::setPin(int pin)
{
    if (pin == m_pin) return;
    if (pin < 0 || pin > 63) {
        qWarning() << "Invalid GPIO pin (must be 0-63):" << pin;
        return;
    }
    m_pin = pin;
    if (m_backend) m_backend->setPinNumber(pin);
    emit pinChanged();
}