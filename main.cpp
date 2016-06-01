//#undef _DEBUG
#include "Model/MailBody.h"
#include "Model/Utils.h"
#include "Model/MailListModel.h"
#include "Model/IMAPClient.h"
#include "Model/Account.h"
#include "Model/Attachment.h"

#include "Exception/mailclientexception.h"
#include "Exception/MailGenerationException.h"
#include "Exception/MailReceiveException.h"
#include "Exception/MailSendException.h"

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
#include<QtDebug>
#include<QObject>

using namespace std;



QString host = "imap.qq.com";

QString user = "375670450@qq.com";

QString passwd = "";


void test(){
    //cout << test << endl;
    Poco::Net::IMAPClientSession imap (host.toStdString ());

    //cin >> passwd;

    imap.login (user.toStdString (), passwd.toStdString ());

    ofstream output ("output.txt");

    Poco::Net::IMAPClientSession::FolderInfoVec folders;

    Poco::Net::IMAPClientSession::MessageInfoVec msgs;

    std::vector<std::string> uids;

    imap.listFolders ("", folders);

    for ( auto fd : folders ) {
        cout << fd.name << endl;

        imap.listMessages (fd.name, uids);

        imap.getMessages (fd.name, uids, msgs);

        std::string msg;

        for ( auto info : msgs) {
            //if( info.parts.childs.size() <= 3 )
                //continue;

            cout << info.subject << endl;
            imap.loadMessage (fd.name, info);
            output << msg << endl << "===============" << endl;
        }
        break;
    }

    return ;
}



int main(int argc, char *argv[])
{
//    Account a;

//    Attachment b;

//    QGuiApplication app(argc, argv);

//    QtWebEngine::initialize ();

//    QQmlApplicationEngine engine;
//    Models::MailListModel model;

//    MAILBODY_PTR_QLIST list;

////    model.login (user, passwd, host, "143");

////    for(int i=1; i<=10; i++)
////        list.push_back (MAILBODY_PTR::create("Subject" + QString::number (i)));

////    model.buildMailList (list);

//    engine.rootContext ()->setContextProperty("mailListModel", &model);

//    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

//    return app.exec();

//    QString s("E:/oopProject/MicroMailClient-UML-1.5.pdf");
//    QFile f(s);
//    f.open(QIODevice::ReadOnly);
//    cout<<f.isOpen();

//    Attachment a("abc");
//    a.setFilePath("E:/oopProject/test.txt");
//    QByteArray b("123");

//    a.Download(b);
//    b = "456";
//    a.Download(b);
    Account a;

    a.AddContact("aebcytd");
    a.AddContact("abdde");
    a.AddContact("aced");
    a.AddContact("vcxrdwa");
    a.AddContact("abcd");
    a.AddContact("abd");
    a.AddContact("acd");

    a.AddContact("bdefa");

    a.AddContact("adsf");
    a.AddContact("ertfggg");
    QStringList src = a.getContacts();

    QStringList result = Utils::ListContacts("abc", src);
    for(auto i = result.begin(); i != result.end(); i++)
        cout << i->toStdString() <<endl;
}





