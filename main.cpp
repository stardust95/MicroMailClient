

#include <QtGui/QGuiApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QtQml/QQmlContext>
#include <QDebug>
#include <QQmlContext>
#include <QVector>
#include <QtWebEngine/qtwebengineglobal.h>


#include "Model/MailBody.h"
#include "Model/Utils.h"
#include "Model/MailListModel.h"
#include "Model/IMAPClientSession.h"

void test(){

    std::string host = "imap.qq.com";

    std::string user = "375670450@qq.com";

//    Poco::Net::IMAPClientSession imap(host);
//    imap.load;


}



int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QtWebEngine::initialize ();

    QQmlApplicationEngine engine;
    Models::MailListModel model;

    MAILBODY_PTR_QLIST list;

//    test();

    auto m = MAILBODY_PTR::create("title1");

    list.append (m);
    list.append (MAILBODY_PTR::create("title2"));
    list.append (MAILBODY_PTR::create("title3"));
    list.append (m);
    list.append (m);
    list.append (m);
    list.append (m);
    list.append (m);
    list.append (m);
    list.append (m);

//    qDebug() << "use count = " << m.use_count() << endl;

    model.buildMailList (list);

    engine.rootContext ()->setContextProperty("mailListModel", &model);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();

}

