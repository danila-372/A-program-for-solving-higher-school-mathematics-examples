///
/// \file mytcpserver.h
/// \brief Заголовочный файл класса TCP-сервера
///

#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include "db.h"
#include "functionsforserver.h"

 ///
 /// \brief Класс TCP-сервера для обработки сетевых подключений
 ///
 /// \details Обеспечивает:
 /// - Запуск сервера на порту 33333
 /// - Управление клиентскими подключениями
 /// - Маршрутизацию команд в FunctionsForServer
 /// - Отслеживание сессий пользователей
 ///
class MyTcpServer : public QObject
{
    Q_OBJECT

public:

    ///
    /// \brief Конструктор сервера
    /// \param parent Родительский объект Qt
    /// \note Инициализирует сервер, БД и тестовые данные
    ///
    explicit MyTcpServer(QObject *parent = nullptr);

    ///
    /// \brief Деструктор сервера
    /// \note Корректно завершает работу сервера и закрывает соединения
    ///
    ~MyTcpServer();

private slots:

    ///
    /// \brief Слот для обработки новых подключений
    /// \details:
    /// - Создает новый сокет
    /// - Отправляет приветственное сообщение
    /// - Настраивает обработчики данных и отключения
    ///
    void slotNewConnection();

    ///
    /// \brief Слот для обработки входящих данных
    /// \details:
    /// - Читает команды от клиента
    /// - Обрабатывает аутентификацию
    /// - Перенаправляет команды в FunctionsForServer
    ///
    void slotServerRead();

    ///
    /// \brief Слот для обработки отключения клиента
    /// \details:
    /// - Удаляет сокет из списка активных подключений
    /// - Освобождает ресурсы соединения
    ///
    void slotClientDisconnected();

private:
    QTcpServer *mTcpServer;                 ///< Основной объект TCP-сервера ///
    FunctionsForServer functions;           ///< Обработчик клиентских команд ///
    QMap<QTcpSocket*, QString> clientLogins;    ///< Карта активных сессий: сокет -> логин ///
};

#endif // MYTCPSERVER_H
