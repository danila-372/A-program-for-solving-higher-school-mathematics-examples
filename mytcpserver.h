///
/// \file mytcpserver.h
/// \brief Заголовочный файл класса TCP-сервера для математического сервиса
///

#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QSet>
#include <QMap>
#include "db.h"

///
/// \brief Класс TCP-сервера для обработки математических запросов
///
/// Обеспечивает:
/// - Управление подключениями клиентов
/// - Обработку входящих команд
/// - Взаимодействие с базой данных
///
class MyTcpServer : public QObject
{
    Q_OBJECT
public:
    ///
    /// \brief Конструктор сервера
    /// \param parent Родительский объект
    ///
    explicit MyTcpServer(QObject *parent = nullptr);

    ///
    /// \brief Деструктор сервера
    ///
    ~MyTcpServer();

    ///
    /// \brief Запуск сервера на указанном порту
    /// \param port Номер порта для прослушивания
    /// \return true в случае успешного запуска, false при ошибке
    ///
    bool start(quint16 port);

    ///
    /// \brief Остановка сервера
    ///
    void stop();

    ///
    /// \brief Установка максимального количества клиентов
    /// \param max Максимальное число одновременных подключений
    ///
    void setMaxClients(int max);

signals:
    ///
    /// \brief Сигнал о подключении нового клиента
    /// \param descriptor Дескриптор сокета клиента
    ///
    void clientConnected(qintptr descriptor);

    ///
    /// \brief Сигнал об отключении клиента
    /// \param descriptor Дескриптор сокета клиента
    ///
    void clientDisconnected(qintptr descriptor);

private slots:
    ///
    /// \brief Слот обработки нового подключения
    ///
    void onNewConnection();

    ///
    /// \brief Слот обработки отключения клиента
    ///
    void onClientDisconnected();

    ///
    /// \brief Слот обработки входящих данных
    ///
    void onReadyRead();

private:
    ///
    /// \brief Обработка команды от клиента
    /// \param client Указатель на сокет клиента
    /// \param command Текст команды
    ///
    void processCommand(QTcpSocket* client, const QString& command);

    ///
    /// \brief Отправка ответа клиенту
    /// \param client Указатель на сокет клиента
    /// \param response Текст ответа
    ///
    void sendResponse(QTcpSocket* client, const QString& response);

    QTcpServer* server;          ///< Основной объект TCP-сервера
    QSet<QTcpSocket*> clients;   ///< Множество активных клиентских подключений
    QMap<QTcpSocket*, QByteArray> clientBuffers; ///< Буферы данных для каждого клиента
    DB& db;                      ///< Ссылка на объект базы данных
    int maxClients;              ///< Максимальное количество клиентов
};

#endif // MYTCPSERVER_H

