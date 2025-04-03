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
 /// \details Реализует:
 /// - Установку/разрыв соединения
 /// - Отправку и прием сообщений
 /// - Выполнение команд на сервере
 ///
 /// \note Использует паттерн Singleton. Экземпляр получается через getInstance()
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
    /// \note В текущей реализации является заглушкой
    ///
    void connect();

    ///
    /// \brief Разрыв соединения с сервером
    /// \note В текущей реализации является заглушкой
    ///
    void disconnect();

    ///
    /// \brief Отправка сообщения серверу
    /// \param message Текст сообщения
    /// \return Всегда true (заглушка)
    /// \warning Реальная реализация должна возвращать статус операции
    ///
    bool sendMessage(const std::string& message);

    ///
    /// \brief Получение сообщения от сервера
    /// \return Фиктивное сообщение "Hello from server!"
    /// \note Возвращает тестовые данные для демонстрации
     ///
    std::string receiveMessage();

    ///
     /// \brief Выполнение команды на сервере
     /// \param command Текст команды
     /// \return Ответ сервера
     /// \details Последовательность операций:
     /// 1. Отправка команды через sendMessage()
     /// 2. Получение ответа через receiveMessage()
     ///
    std::string sendCommand(const std::string& command); ////

private:
    ServerConnection() {} ///< Приватный конструктор (Singleton) /// // Конструктор приватный для паттерна
    
    ///
    /// \brief Запрет копирования
    /// \param other Ссылка на копируемый объект
    ///
    ServerConnection(const ServerConnection&) = delete; // Запрет копирования
    
    ///
    /// \brief Запрет присваивания
    /// \param other Ссылка на присваиваемый объект
    /// \return Ссылка на текущий объект
    ///
    ServerConnection& operator=(const ServerConnection&) = delete; // Запрет присваивания
};

#endif // SERVER_CONNECTION_H
