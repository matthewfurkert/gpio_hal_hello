#ifndef GPIOPIN_H
#define GPIOPIN_H
#include <QObject>
#include <qqmlintegration.h>
#include <QtQml/qqmlregistration.h>
#include <memory>
#include "igpiopin.h"

class GpioPin : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged FINAL)
    Q_PROPERTY(bool status READ status WRITE setStatus NOTIFY statusChanged FINAL)
    Q_PROPERTY(int pin READ pin WRITE setPin NOTIFY pinChanged FINAL)

public:
    explicit GpioPin(QObject *parent = nullptr);
    ~GpioPin() override = default;

    bool enabled() const;
    bool status() const;
    int pin() const;

public slots:
    void setEnabled(bool enabled);
    void setStatus(bool status);
    void setPin(int pin);

signals:
    void enabledChanged();
    void statusChanged();
    void pinChanged();

private:
    std::unique_ptr<IGpioPin> m_backend;
    bool m_enabled = true;
    bool m_status = false;
    int m_pin = 17;                    // default BCM17 – easy to test

    void createBackend();
};

#endif // GPIOPIN_H