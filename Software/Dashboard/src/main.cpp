#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "usbdashreceiver.h"
#include <QQuickStyle>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQuickStyle::setStyle("Basic");

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
        receiver->start(1155, 14158);
    } else {
        qWarning("No UsbDashReceiver singleton");
    }

    return app.exec();
}
