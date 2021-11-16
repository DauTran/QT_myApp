#include <QGuiApplication>
#include <QStringList>

#include <qqml.h>
#include "backend.h"
#include "dataentrymodel.h"
#include "mymodel.h"

#include <QQuickView>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<MyModel>("org.mymodel", 1, 0, "MyModel");

    // register the type DataEntryModel
    // under the url "org.model.entry" in version 1.0
    // under the name "DataEntryModel"
    qmlRegisterType<DataEntryModel>("org.model.entry", 1, 0, "DataEntryModel");

    qmlRegisterType<BackEnd>("org.backend", 1, 0, "BackEnd");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}


