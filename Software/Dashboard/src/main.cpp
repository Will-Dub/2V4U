#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "usbdashreceiver.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("Dashboard", "Main");

    auto* receiver = engine.singletonInstance<UsbDashReceiver*>("Dashboard", "UsbDashReceiver");

    if (receiver) {
        receiver->init("COM3");
    } else {
        qWarning("No UsbDashReceiver singleton");
    }

    return app.exec();
}
