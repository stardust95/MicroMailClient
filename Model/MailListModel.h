#ifndef MAILLISTMODEL_H
#define MAILLISTMODEL_H

#include <QAbstractItemModel>
#include <QAbstractListModel>
#include <QVariant>
#include <QList>
#include <QHash>
#include <QByteArray>
#include <QtAlgorithms>
#include <QQmlEngine>
#include <QDebug>

#include "MailBody.h"


namespace Models{

    class MailListModel : public QAbstractListModel {
        Q_OBJECT
    public:

        explicit MailListModel(QObject * parent = 0) : QAbstractListModel(parent) {

        }

        ~MailListModel(){

        }


        enum MailBodyRoles{
            mail_subject = Qt::UserRole + 1,
            mail_content,
            mail_datetime,
            mail_isread,
            mail_sender,
            mail_recipients,
    //        mail_attachments
        };


        QHash<int, QByteArray> roleNames() const{
            QHash<int, QByteArray> roles;
            roles[mail_subject] = "mail_subject";
            roles[mail_content] = "mail_content";
            roles[mail_datetime] = "mail_datetime";
            roles[mail_isread] = "mail_isread";
            roles[mail_sender] = "mail_sender";
            roles[mail_recipients] =  "mail_recipients";
    //        roles[mail_attachments] = "mail_attachments";
            return roles;
        }


        int rowCount(const QModelIndex & parent = QModelIndex()) const{
            return _items.size ();
        }

        int columnCount(const QModelIndex & parent = QModelIndex()) const{
            return 1;
        }

        QVariant data(const QModelIndex &index, int role) const{

            if( !index.isValid () )
                return QVariant();
//            qDebug() << "Model data index = " << index.row () << "," << index.column () << " : " << role << "\n";
            auto item = _items.at(index.row());
            switch( role ){
                case mail_content:
                    return item->GetContent ();
                case mail_datetime:
                    return item->GetDateTime ();
                case mail_isread:
                    return item->GetIsread();
//                case mail_recipients:
//                    return item->recipients.at (0);
                case mail_sender:
                    return item->GetSender ();
                case mail_subject:
                    return item->GetSubject ();
                default:
                    return QVariant();
                }
        }

//        QVariant headerData(int section, Qt::Orientation orientation, int role) const{

//        }

        void BuildMailList(const MAILBODY_PTR_QLIST & items){
            if ( items.size () == 0 )
                return;
            this->beginInsertRows (QModelIndex(), this->rowCount (), this->rowCount () + items.size () - 1);

            foreach (auto item, items) {
                this->_items.append (item);
            }

            this->endInsertRows ();

        }


        MAILBODY_PTR_QLIST ToList() const{
            return this->_items;
        }

        Q_INVOKABLE QVariant Get(int index){
            if( index >= 0 && index < _items.size () ){
//                qDebug() << "Model data index = " << index << "," << index << " : " << role << "\n";
                return QVariant::fromValue(_items.at (index));
            }

            return QVariant();
        }

    private:

        MAILBODY_PTR_QLIST _items;

    };

}

#endif // MAILLISTMODEL_H
