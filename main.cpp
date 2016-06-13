//#undef _DEBUG
#include "Model/MailBody.h"
#include "Model/Utils.h"
#include "Model/MailListModel.h"
#include "Model/IMAPClient.h"

#include <QtGui/QGuiApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QtQml/QQmlContext>
#include <QDebug>
#include <QQmlContext>
#include <QVector>
#include <QString>
#include <QtWebEngine/qtwebengineglobal.h>

#include<string>
#include<vector>
#include<sstream>
#include<fstream>

using namespace std;

/*
Todo:
    1. 附件
    2. 发邮件
    3. 列表搜索
*/



int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QString receiveHost  = "pop.qq.com";

    QString sendHost = "smtp.qq.com";

    QString user = "375670450@qq.com";

    QString passwd = "sftkpahwbroabhjg";


    QtWebEngine::initialize ();
    QQmlApplicationEngine engine;
    Models::MailListModel model;

    model.login (user, passwd, sendHost, receiveHost, true);

    model.doBuildMailList (3);

    model.downloadAttach (1, 1, ".");

    engine.rootContext ()->setContextProperty("mailListModel", &model);

//    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();

}

