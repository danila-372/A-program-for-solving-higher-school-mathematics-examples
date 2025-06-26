///
/// \file connection.h
/// \brief Заголовочный файл класса для управления сетевым подключением
///

#ifndef SERVER_CONNECTION_H
#define SERVER_CONNECTION_H

#include <string>
#include <iostream>

///
/// \brief Класс для работы с сетевым подключением (Singleton)
///
/// Реализует функционал:
/// - Установка и разрыв соединения с сервером
/// - Отправка и получение сообщений
/// - Выполнение команд на сервере
///
/// \note Использует паттерн Singleton для гарантии единственного экземпляра
///
class ServerConnection {
public:
    ///
    /// \brief Получение единственного экземпляра класса
    /// \return Ссылка на экземпляр ServerConnection
    ///
    static ServerConnection& getInstance();

    ///
    /// \brief Установка соединения с сервером
    /// \note Текущая реализация является заглушкой
    ///
    void connect();

    ///
    /// \brief Разрыв соединения с сервером
    /// \note Текущая реализация является заглушкой
    ///
    void disconnect();

    ///
    /// \brief Отправка сообщения серверу
    /// \param message Текст сообщения для отправки
    /// \return Всегда возвращает true (временная заглушка)
    /// \warning Реальная реализация должна возвращать статус операции
    ///
    bool sendMessage(const std::string& message);

    ///
    /// \brief Получение сообщения от сервера
    /// \return Тестовое сообщение "Hello from server!"
    /// \note Текущая реализация возвращает фиктивные данные
    ///
    std::string receiveMessage();

    ///
    /// \brief Выполнение команды на сервере
    /// \param command Текст команды для выполнения
    /// \return Ответ сервера
    /// \details Последовательность выполнения:
    /// 1. Отправка команды через sendMessage()
    /// 2. Получение ответа через receiveMessage()
    ///
    std::string sendCommand(const std::string& command);

private:
    ///
    /// \brief Приватный конструктор
    ///
    /// Обеспечивает реализацию паттерна Singleton
    ///
    ServerConnection() {}

    ///
    /// \brief Запрет копирования
    /// \param other Ссылка на копируемый объект
    ///
    ServerConnection(const ServerConnection&) = delete;

    ///
    /// \brief Запрет оператора присваивания
    /// \param other Ссылка на присваиваемый объект
    /// \return Ссылка на текущий объект
    ///
    ServerConnection& operator=(const ServerConnection&) = delete;
};

#endif // SERVER_CONNECTION_H