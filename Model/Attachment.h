#ifndef ATTACHMENT_H
#define ATTACHMENT_H
#include "Utils.h"
#include "Exception/MailClientException.h"
#include "Exception/MailGenerationException.h"

#include "Poco/Net/PartSource.h"

#include <QString>
#include <QVariant>
#include <QSharedPointer>
#include <QDebug>
#include <QByteArray>
#include <QFile>

#include <iostream>
#include <fstream>

using namespace std;

class Attachment : public Poco::Net::PartSource
{
private:
    typedef enum{unDownloaded, isDownloading, isDownloaded} downloadState;

    Utils::MediaType _attachmentType;
    qint64 _fileSize;
    qint64 _hasDownloaded;
    QString _fileName;
    QString _filePath;
    downloadState _state;
    QString _accessCommand;
    fstream _fileStream;

    const static qint64 _fileMaxSize = 1024*1024;

    int findPostfix(const QString &src) {
        int i = src.length() - 1;
        while(src[i]!='.') i--;
        return i;
    }

public:

    std::istream& stream() override {
        return _fileStream;
    }

    Attachment(const std::string& mediaType):
        _attachmentType(mediaType) {
        _state = unDownloaded;
        _hasDownloaded = 0;
    }

    Attachment(const std::string& type, const std::string& subType):
        _attachmentType(type, subType) {
        _state = unDownloaded;
        _hasDownloaded = 0;
    }

    Attachment(const Utils::MediaType& mediaType):
        _attachmentType(mediaType) {
        _state = unDownloaded;
        _hasDownloaded = 0;
    }

    Utils::MediaType getAttachmentType() const {
        return _attachmentType;
    }

    size_t getFileSize() const {
        return _fileSize;
    }

    void setFileSize(size_t i) {
        _fileSize = i;
    }

    QString getFileName() const {
        return _fileName;
    }

    void setFileName(const QString& i) {
        _fileName = i;
    }

    QString getFilePath() const {
        return _filePath;
    }

    void setFilePath(const QString& i) {
        _filePath = i;
        _fileStream.open(_filePath.toStdString(), ios::in|ios::out|ios::binary);
    }

    QString getAccessCommand() const {
        return _accessCommand;
    }

    void setAccessCommand(const QString& i) {
        _accessCommand = i;
    }

    bool Download(const QByteArray& data) {

        QString tmp = _filePath;
        int t = findPostfix(tmp);
        QString postfix = tmp.mid(t, tmp.length()-t);
        tmp = tmp.mid(0,t);
        if(_state == unDownloaded) {   //make sure the file name is not existed
            for(int i = 1; QFile::exists(_filePath); i++) {
                _filePath = tmp + "(";
                _filePath += i + '0';
                _filePath += ")";
                _filePath += postfix;
            }
            _state = isDownloading;
        }

        QFile output(_filePath);
        output.open(QIODevice::WriteOnly|QIODevice::Append);
        qint64 size = data.size();
        output.write(data.data(), size);
        output.close();

        _hasDownloaded += size;
        if(_hasDownloaded == _fileSize)
            _state = isDownloaded;
        return true;
    }

    bool Upload(QByteArray& data) {
        QFile input(_filePath);

        try {
            if(input.isOpen())
                throw(MailGenerationException("File is being occupied!"));
            if(!input.open(QIODevice::ReadOnly))  //file not existed
                throw(MailGenerationException("File doesn't exists!"));
            qint64 size = input.size();
            if(size > _fileMaxSize)
                throw(MailGenerationException("File size is beyond limit!"));
        } catch(MailGenerationException& mse) {
            qDebug() << mse.what() << endl;
            return false;
        }

        data = input.readAll();

        input.close();
        return true;
    }

};

typedef QSharedPointer<Attachment> ATTACHMENT_PTR;

#endif // ATTACHMENT_H
