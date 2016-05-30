#ifndef ATTACHMENT_H
#define ATTACHMENT_H
#include "Poco/Net/PartSource.h"
#include "Utils.h"
#include <qstring.h>
#include <iostream>

class Attachment : public Poco::Net::PartSource
{
    Utils::MediaType _attachmentType;
    size_t _fileSize;
    QString _fileName;
    QString _accessCommand;

public:
    Attachment(const std::string& mediaType):
        _attachmentType(mediaType) {}

    Attachment(const std::string& type, const std::string& subType):
        _attachmentType(type, subType) {}

    Attachment(const Utils::MediaType& mediaType):
        _attachmentType(mediaType) {}

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

    void setFileName(QString i) {
        _fileName = i;
    }

    QString getAccessCommand() const {
        return _accessCommand;
    }

    void setAccessCommand(QString i) {
        _accessCommand = i;
    }

    bool Download(QByteArray a) {

    }

};

#endif // ATTACHMENT_H
