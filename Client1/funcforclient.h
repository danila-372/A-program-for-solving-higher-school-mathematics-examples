///
/// \file funcforclient.h
/// \brief Заголовочный файл с функциями для клиентской части
///

#ifndef FUNCFORCLIENT_H
#define FUNCFORCLIENT_H

#include <QString>
#include "connection.h"

///
/// \brief Функция аутентификации пользователя
/// \param log Логин пользователя
/// \param pass Пароль пользователя
/// \return Результат аутентификации:
/// - "user" для обычного пользователя
/// - "admin" для администратора
/// - "error" при ошибке аутентификации
///
QString auth(QString, QString);

#endif // FUNCFORCLIENT_H
