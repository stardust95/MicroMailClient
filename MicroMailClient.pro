TEMPLATE = app

QT += qml quick
CONFIG += c++11

SOURCES += main.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
#QML_IMPORT_PATH =

INCLUDEPATH += "E:\Libs\poco-mingw\include"
LIBS += -L"E:\Libs\poco-mingw\lib" -lPocoNet -lPocoFoundation

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    Model/Account.h \
    Model/Attachment.h \
    Model/MailClient.h \
    Model/MailBody.h \
    Model/Utils.h \
    Model/MailListModel.h
