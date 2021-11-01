#include <QGuiApplication>
#include <QStringList>

#include <qqml.h>
#include "backend.h"
#include "dataentrymodel.h"
#include "mymodel.h"

#include <QQuickView>

//void test()
//{
//    QDir dir("A:/Training Life Good");
//    QStringList dataList;

////    foreach (QFileInfo var, dir.entryInfoList()) {
////        qDebug() << var.fileName();
////       qDebug() << "Path" << var.filePath();
////        dataList.append(var.fileName());
////    }
////    for(int i = 0; i < 5; ++i)
////    {
//        dataList.append("Item ");
//        dataList.append("Item 2");
//        dataList.append("Item 3");
//        dataList.append("Item 4");
////    }

//    QQuickView view;

//    QQmlContext *ctxt = view.rootContext();
//    ctxt->setContextProperty("myModel", QVariant::fromValue(dataList));

//}

int main( int argc, char** argv ) {
    QGuiApplication app( argc, argv );

    MyModel model;
    DataEntryModel dataEntryModel;

    QQuickView view;
    view.setResizeMode(QQuickView::SizeRootObjectToView);

    QQmlContext *context = view.engine()->rootContext();
    context->setContextProperty("myModel", &model);
//    context->setContextProperty("DataEntryModel", &dataEntryModel);

    view.setSource(QUrl("qrc:main.qml"));
//    view.show();

    return app.exec();
}

//int main1(int argc, char *argv[])
//{
//    QGuiApplication app(argc, argv);


//    QQmlApplicationEngine engine;

//    qmlRegisterType<MyModel>("org.mymodel", 1, 0, "MyModel");

//    // register the type DataEntryModel
//    // under the url "org.model.entry" in version 1.0
//    // under the name "DataEntryModel"
//    qmlRegisterType<DataEntryModel>("org.model.entry", 1, 0, "DataEntryModel");

//    qmlRegisterType<BackEnd>("org.backend", 1, 0, "BackEnd");

//    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

//    return app.exec();
//}


