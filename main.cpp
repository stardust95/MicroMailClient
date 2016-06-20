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


int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QtWebEngine::initialize ();

    QQmlApplicationEngine engine;

    Models::MailListModel model;

    engine.rootContext ()->setContextProperty("mailListModel", &model);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();

}

