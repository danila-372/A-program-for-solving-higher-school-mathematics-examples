///
/// \file functionsforserver.h
/// \brief Заголовочный файл класса для обработки команд сервера
///

#ifndef FUNCTIONSFORSERVER_H
#define FUNCTIONSFORSERVER_H

#include <QObject>
#include <QTcpSocket>
#include "db.h"

///
/// \brief Класс для обработки команд, получаемых сервером
///
/// Предоставляет функционал для аутентификации, работы с задачами,
/// управления пользователями и статистикой через TCP-соединение
///
class FunctionsForServer : public QObject
{
    Q_OBJECT
public:
    ///
    /// \brief Конструктор класса
    /// \param parent Родительский объект в Qt-иерархии
    ///
    explicit FunctionsForServer(QObject *parent = nullptr);

    ///
    /// \brief Основной метод обработки входящих команд
    /// \param socket Указатель на сокет клиента
    /// \param command Полученная команда
    /// \param username Имя пользователя (если аутентифицирован)
    ///
    void parse(QTcpSocket *socket, const QString &command, const QString &username);

private:
    // Аутентификация и управление пользователями

    ///
    /// \brief Аутентификация пользователя
    /// \param socket Указатель на сокет клиента
    /// \param username Имя пользователя
    /// \param password Пароль
    ///
    void auth(QTcpSocket *socket, const QString &username, const QString &password);

    ///
    /// \brief Регистрация нового пользователя
    /// \param socket Указатель на сокет клиента
    /// \param username Имя пользователя
    /// \param password Пароль
    /// \param isAdmin Флаг администратора (по умолчанию false)
    ///
    void reg(QTcpSocket *socket, const QString &username, const QString &password, bool isAdmin = false);

    ///
    /// \brief Удаление пользователя
    /// \param socket Указатель на сокет клиента
    /// \param username Имя пользователя для удаления
    /// \param currentUsername Имя текущего пользователя (для проверки прав)
    ///
    void deleteUser(QTcpSocket *socket, const QString &username, const QString &currentUsername);

    // Работа с задачами

    ///
    /// \brief Решение математической задачи
    /// \param socket Указатель на сокет клиента
    /// \param problem Текст задачи
    /// \param answer Ответ пользователя
    /// \param username Имя пользователя
    ///
    void solveMathProblem(QTcpSocket *socket, const QString &problem, const QString &answer, const QString &username);

    ///
    /// \brief Получение списка всех задач
    /// \param socket Указатель на сокет клиента
    ///
    void listProblems(QTcpSocket *socket);

    ///
    /// \brief Добавление новой задачи
    /// \param socket Указатель на сокет клиента
    /// \param problem Текст задачи
    /// \param answer Ответ к задаче
    /// \param currentUsername Имя текущего пользователя (для проверки прав)
    ///
    void addProblem(QTcpSocket *socket, const QString &problem, const QString &answer, const QString &currentUsername);

    ///
    /// \brief Обновление ответа к задаче
    /// \param socket Указатель на сокет клиента
    /// \param problem Текст задачи
    /// \param newAnswer Новый ответ
    /// \param currentUsername Имя текущего пользователя (для проверки прав)
    ///
    void updateProblem(QTcpSocket *socket, const QString &problem, const QString &newAnswer, const QString &currentUsername);

    // Работа со статистикой

    ///
    /// \brief Просмотр статистики пользователя
    /// \param socket Указатель на сокет клиента
    /// \param username Имя пользователя
    /// \param currentUsername Имя текущего пользователя (для проверки прав)
    ///
    void viewUserStats(QTcpSocket *socket, const QString &username, const QString &currentUsername);

    ///
    /// \brief Просмотр собственной статистики
    /// \param socket Указатель на сокет клиента
    /// \param username Имя пользователя
    ///
    void viewMyStats(QTcpSocket *socket, const QString &username);

    ///
    /// \brief Просмотр статистики всех пользователей
    /// \param socket Указатель на сокет клиента
    /// \param currentUsername Имя текущего пользователя (для проверки прав)
    ///
    void viewAllStats(QTcpSocket* socket, const QString& currentUsername);

    // Категории задач

    ///
    /// \brief Получение списка категорий задач
    /// \param socket Указатель на сокет клиента
    ///
    void listCategories(QTcpSocket *socket);

    ///
    /// \brief Получение задач по категории
    /// \param socket Указатель на сокет клиента
    /// \param category Название категории
    ///
    void getCategoryProblems(QTcpSocket *socket, const QString &category);

    // Администрирование

    ///
    /// \brief Повышение пользователя до администратора
    /// \param socket Указатель на сокет клиента
    /// \param targetUser Имя пользователя для повышения
    /// \param adminUser Имя администратора, выполняющего операцию
    ///
    void promoteUser(QTcpSocket *socket, const QString &targetUser, const QString &adminUser);

    ///
    /// \brief Получение списка пользователей
    /// \param socket Указатель на сокет клиента
    /// \param currentUsername Имя текущего пользователя (для проверки прав)
    ///
    void listUsers(QTcpSocket *socket, const QString &currentUsername);

    ///
    /// \brief Генерация задач по категории
    /// \param socket Указатель на сокет клиента
    /// \param category Категория задач
    /// \param currentUsername Имя текущего пользователя (для проверки прав)
    ///
    void generateProblems(QTcpSocket *socket, const QString &category, const QString &currentUsername);

    // Внутренние методы

    ///
    /// \brief Обработка запроса на получение задач
    /// \param socket Указатель на сокет клиента
    /// \param category Категория задач
    ///
    void handleGetProblems(QTcpSocket* socket, const QString& category);

    ///
    /// \brief Проверка корректности задачи
    /// \param problem Текст задачи
    /// \return true, если задача корректна, false в противном случае
    ///
    bool isValidProblem(const QString& problem);

    ///
    /// \brief Обработка проверки ответа на задачу
    /// \param socket Указатель на сокет клиента
    /// \param category Категория задачи
    /// \param problemIndex Индекс задачи
    /// \param answer Ответ пользователя
    /// \param username Имя пользователя
    ///
    void handleCheckAnswer(QTcpSocket* socket, const QString& category, int problemIndex, const QString& answer, const QString& username);
};

#endif // FUNCTIONSFORSERVER_H
