#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>
#include <QDebug>
#include <QQmlContext>
#include <QVector>

#include "Model/MailBody.h"
#include "Model/Utils.h"
#include "Model/MailListModel.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    Models::MailListModel model;

    MAILBODY_PTR_QLIST list;

    auto m = MAILBODY_PTR::create();

    list.append (m);
    list.append (m);
    list.append (m);
    list.append (m);

//    qDebug() << "use count = " << m.use_count() << endl;

    model.BuildMailList (list);

    engine.rootContext ()->setContextProperty("mailListModel", &model);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();

}


