#include <QGuiApplication>
#include <QStringList>
#include <QtQml>
#include <qqml.h>
//#include "backend.h"
//#include "dataentrymodel.h"
#include "mymodel.h"

#include <QQuickView>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    // register the type MyModel
    // under the url "org.mymodel" in version 1.0
    // under the name "MyModel"
    qmlRegisterType<MyModel>("org.mymodel", 1, 0, "MyModel");

//    qmlRegisterType<DataEntryModel>("org.model.entry", 1, 0, "DataEntryModel");

//    qmlRegisterType<BackEnd>("org.backend", 1, 0, "BackEnd");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}


