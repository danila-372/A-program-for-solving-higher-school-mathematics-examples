///
/// \file main.cpp
/// \brief Главный файл серверного приложения
///

#include <QCoreApplication>
#include "mytcpserver.h"

///
/// \brief Точка входа в серверное приложение
/// \param argc Количество аргументов командной строки
/// \param argv Массив аргументов командной строки
/// \return Код завершения приложения
///
/// Создает и запускает TCP-сервер для математического приложения на порту 33333.
/// При успешном запуске выводит информационное сообщение о готовности сервера.
/// В случае ошибки запуска сервера возвращает код ошибки 1.
///
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    MyTcpServer server;
    if (!server.start(33333)) {
        return 1;
    }

    qInfo() << "Math server is running. Connect via telnet localhost 33333";

    return a.exec();
}

