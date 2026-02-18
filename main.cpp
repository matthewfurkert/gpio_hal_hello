#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "GpioHal/gpiofactory.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    // Create the HAL pin (offset 10 is just an example - change as needed)
    Gpio::IGpioPin* gpioPin = Gpio::GpioFactory::createPin(10);

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("gpioPin", gpioPin);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("gpio_hal_hello", "Main");

    return app.exec();
}
