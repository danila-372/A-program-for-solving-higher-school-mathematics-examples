#include "funcforclient.h"


QString auth(QString log,QString pass)
{
    if(log=="user"&&pass=="123")
        return "user";
    else if(log=="admin"&&pass=="123")
        return "admin";
    else {
        return "error";
    }

    //ServerConnection::getInstance()::sendmessage(log,pass);
}
