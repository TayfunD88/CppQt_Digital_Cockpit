#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "caremulator.h"


int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    //qmlRegisterType<CarEmulator>("backend",1,0,"Simulation");
    qmlRegisterSingletonType<CarEmulator>("backend", 1, 0, "CarEmulator", keyListenerInstance);

    QQmlApplicationEngine engine;
    const QUrl url(u"qrc:/main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);




    return app.exec();
}
