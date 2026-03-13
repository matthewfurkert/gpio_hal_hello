#include "realgpiopin.h"
#include <QDebug>
#include <QThread>

RealGpioPin::RealGpioPin() {}

RealGpioPin::~RealGpioPin() {
    if (m_active) unexportPin();
}

QString RealGpioPin::gpioPath() const {
    return QString("/sys/class/gpio/gpio%1").arg(m_pin);
}

bool RealGpioPin::exportPin() {
    QFile f("/sys/class/gpio/export");
    if (f.open(QIODevice::WriteOnly)) {
        f.write(QByteArray::number(m_pin));
        return true;
    }
    return false;
}

bool RealGpioPin::unexportPin() {
    QFile f("/sys/class/gpio/unexport");
    if (f.open(QIODevice::WriteOnly)) {
        f.write(QByteArray::number(m_pin));
        return true;
    }
    return false;
}

bool RealGpioPin::setDirectionOut() {
    QFile f(gpioPath() + "/direction");
    if (f.open(QIODevice::WriteOnly)) {
        f.write("out");
        return true;
    }
    return false;
}

bool RealGpioPin::setActive(bool active) {
    if (active == m_active) return true;

    if (active) {
        if (exportPin()) {
            QThread::msleep(80); // kernel needs time
            if (setDirectionOut()) {
                m_active = true;
                m_valueFile.setFileName(gpioPath() + "/value");
                m_valueFile.open(QIODevice::WriteOnly);
                return true;
            }
        }
        qWarning() << "Failed to export GPIO" << m_pin;
        return false;
    } else {
        unexportPin();
        m_active = false;
        m_valueFile.close();
        return true;
    }
}

void RealGpioPin::setPinNumber(int pin) {
    if (m_pin == pin) return;
    if (m_active) setActive(false);
    m_pin = pin;
}

void RealGpioPin::write(bool value) {
    if (!m_active || !m_valueFile.isOpen()) return;
    m_valueFile.write(value ? "1" : "0");
    m_valueFile.flush();
}

bool RealGpioPin::read() const {
    if (!m_active) return false;
    QFile f(gpioPath() + "/value");
    if (f.open(QIODevice::ReadOnly)) {
        return f.readAll().trimmed() == "1";
    }
    return false;
}