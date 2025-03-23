///
/// \file main.cpp
/// \brief Точка выхода серверного приложения
///

#include <QCoreApplication>
#include "mytcpserver.h"

///
/// \brief Основная функция приложения
/// \param argc Количество аргументов командной строки
/// \param argv Массив аргументов командной строки
/// \return Код завершения приложения
///
/// \details
/// 1. Инициализирует Qt-приложение через QCoreApplication
/// 2. Создает экземпляр TCP-сервера (MyTcpServer)
/// 3. Запускает главный цикл обработки событий
/// \note Для корректной работы требует предварительной настройки БД
///
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    MyTcpServer server;
    return a.exec();
}
