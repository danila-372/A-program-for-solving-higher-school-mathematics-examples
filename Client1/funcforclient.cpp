///
/// \file funcforclient.cpp
/// \brief Реализация функций для клиентской части приложения
///

#include "funcforclient.h"

///
/// \brief Функция аутентификации пользователя
/// \param log Логин пользователя
/// \param pass Пароль пользователя
/// \return Строка с результатом аутентификации:
/// - "user" - успешная аутентификация обычного пользователя
/// - "admin" - успешная аутентификация администратора
/// - "error" - неверные учетные данные
///
/// \note В текущей реализации используется жестко заданные логины/пароли:
/// - user/123 - для обычного пользователя
/// - admin/123 - для администратора
/// \warning В реальной реализации следует использовать безопасное хранение и передачу учетных данных
///
QString auth(QString log, QString pass)
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
