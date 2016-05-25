#ifndef ACCOUNT_H
#define ACCOUNT_H
#include <QList>
#include <QSharedPointer>

#include<iostream>

class Account{



};


typedef QSharedPointer<Account> ACCOUNT_PTR;
typedef QList<ACCOUNT_PTR> ACCOUNT_PTR_QLIST;



#endif // ACCOUNT_H


