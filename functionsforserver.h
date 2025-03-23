///
/// \file functionsforserver.h
/// \brief Заголовочный файл класса обработки клиентских команд
///

#ifndef FUNCTIONSFORSERVER_H
#define FUNCTIONSFORSERVER_H

#include <QObject>
#include <QTcpSocket>
#include "db.h"

///
/// \brief Класс для обработки сетевых команд клиентов
/// \details Обрабатывает команды через TCP-сокеты, взаимодействует с базой данных.
/// Реализует логику регистрации, аутентификации, работы с задачами и статистикой.
///
class FunctionsForServer : public QObject
{
    Q_OBJECT

public:

    ///
    /// \brief Конструктор класса
    /// \param parent Родительский объект Qt
    ///
    explicit FunctionsForServer(QObject *parent = nullptr);

    ///
    /// \brief Анализирует и выполняет команду от клиента
    /// \param socket Указатель на сокет подключения
    /// \param command Строка команды в формате "<действие> [параметры]"
    /// \param username Текущий пользователь (для проверки прав)
    ///
    /// \note Поддерживаемые команды описаны в реализации parse()
    ///
    void parse(QTcpSocket *socket, const QString &command, const QString &username);

private:

    /// \name Приватные методы обработки команд
    /// \{

    ///
    /// \brief Аутентификация пользователя
    /// \param socket Сокет для ответа
    /// \param username Логин пользователя
    /// \param password Пароль пользователя
    /// \note Обновляет время последнего входа при успешной аутентификации
    ///
    void auth(QTcpSocket *socket, const QString &username, const QString &password);

    ///
    /// \brief Регистрация нового пользователя
    /// \param isAdmin Флаг администратора (по умолчанию false)
    /// \note Пароль хранится в виде SHA-256 хеша
    ///
    void reg(QTcpSocket *socket, const QString &username, const QString &password, bool isAdmin = false);

    ///
    /// \brief Удаление пользователя
    /// \param currentUsername Инициатор операции
    /// \note Требует прав администратора у currentUsername
    ///
    void deleteUser(QTcpSocket *socket, const QString &username, const QString &currentUsername);

    ///
    /// \brief Проверка решения математической задачи
    /// \param problem Название задачи
    /// \param answer Ответ пользователя (пустой для запроса)
    /// \note Обновляет статистику пользователя
    ///
    void solveMathProblem(QTcpSocket *socket, const QString &problem, const QString &answer, const QString &username);

    ///
    /// \brief Отправка списка доступных задач
    ///
    void listProblems(QTcpSocket *socket);

    ///
    /// \brief Добавление новой задачи
    /// \param currentUsername Инициатор операции
    /// \note Требует прав администратора
    ///
    void addProblem(QTcpSocket *socket, const QString &problem, const QString &answer, const QString &currentUsername);

    ///
    /// \brief Обновление ответа задачи
    /// \param currentUsername Инициатор операции
    /// \note Требует прав администратора
    ///
    void updateProblem(QTcpSocket *socket, const QString &problem, const QString &newAnswer, const QString &currentUsername);

    ///
    /// \brief Просмотр статистики пользователей
    /// \param currentUsername Инициатор запроса
    /// \note Требует прав администратора
    ///
    void viewUserStats(QTcpSocket *socket, const QString &username, const QString &currentUsername);

    ///
    /// \brief Отображение общей статистики всех пользователей
    /// \param currentUsername Инициатор запроса
    /// \note Требует прав администратора. Формат:
    /// "User: <логин> | Total: <задач> | Correct: <решено> | Last Login: <время>"
    ///
    void showUserStats(QTcpSocket *socket, const QString &currentUsername);
    /// \}
};

#endif // FUNCTIONSFORSERVER_H
