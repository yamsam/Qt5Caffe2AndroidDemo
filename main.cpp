#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include "Caffe2VideoFilter.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    qmlRegisterType<Caffe2VideoFilter>("Caffe2VideoFilterLib", 1, 0, "Caffe2VideoFilter");
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));

    return app.exec();
}
