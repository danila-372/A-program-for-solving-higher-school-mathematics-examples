///
/// \file functionsforserver.cpp
/// \rief Реализация методов обработки команд сервера
///

#include "functionsforserver.h"
#include <QDebug>

///
/// \brief Конструктор класса FunctionsForServer
/// \param parent Родительский объект Qt
///
FunctionsForServer::FunctionsForServer(QObject *parent) : QObject(parent) {}

///
/// \brief Анализирует и выполняет команду от клиента
/// \param socket Указатель на сокет подключения
/// \param command Полученная команда в формате: "<действие> [параметры]"
/// \param username Текущий пользователь (для проверки прав)
///
/// Поддерживаемые команды:
/// - register <логин> <пароль>
/// - auth <логин> <пароль>
/// - delete <логин> (требует прав администратора)
/// - solve <задача> [ответ]
/// - problems (список задач)
/// - addproblem <задача> <ответ> (требует прав администратора)
/// - updateproblem <задача> <новый_ответ> (требует прав администратора)
/// - stats <пользователь> (статистика решений)
///
void FunctionsForServer::parse(QTcpSocket *socket, const QString &command, const QString &username) {
    QStringList parts = command.split(" ");
    if (parts.isEmpty()) {
        socket->write("Unknown command\r\n");
        return;
    }

    QString action = parts[0];
    if (action == "register" && parts.size() == 3) {
        reg(socket, parts[1], parts[2]);
    } else if (action == "auth" && parts.size() == 3) {
        auth(socket, parts[1], parts[2]);
    } else if (action == "delete" && parts.size() == 2) {
        deleteUser(socket, parts[1], username);
    } else if (action == "solve" && parts.size() == 2) {
        solveMathProblem(socket, parts[1], "", username);
    } else if (action == "problems") {
        listProblems(socket);
    } else if (action == "addproblem" && parts.size() >= 3) {
        QString problem = parts[1];
        QString answer = parts.mid(2).join(" ");
        addProblem(socket, problem, answer, username);
    } else if (action == "updateproblem" && parts.size() >= 3) {
        QString problem = parts[1];
        QString newAnswer = parts.mid(2).join(" ");
        updateProblem(socket, problem, newAnswer, username);
    } else if (action == "stats" && parts.size() == 2) {
        viewUserStats(socket, parts[1], username);
    } else if (action == "userstats") {
        showUserStats(socket, username);
    } else {
        socket->write("Unknown command\r\n");
    }
}

///
/// \brief Аутентификация пользователя
/// \param socket Сокет для ответа
/// \param username Логин пользователя
/// \param password Пароль пользователя
/// \note Отправляет "Authentication successful" или "Authentication failed"
/// \note Добавлен вызов DB::updateLastLogin() при успешной аутентификации
///
void FunctionsForServer::auth(QTcpSocket *socket, const QString &username, const QString &password) {
    if (DB::instance().authenticateUser(username, password)) {
        DB::instance().updateLastLogin(username); // Обновляем время входа
        socket->write("Authentication successful\r\n");
    } else {
        socket->write("Authentication failed\r\n");
    }
}

///
/// \brief Регистрация нового пользователя
/// \param socket Сокет для ответа
/// \param username Логин для регистрации
/// \param password Пароль пользователя
/// \param isAdmin Флаг администратора (по умолчанию false)
/// \note Администраторов можно создавать только через прямое изменение БД
///
void FunctionsForServer::reg(QTcpSocket *socket, const QString &username, const QString &password, bool isAdmin) {
    if (DB::instance().registerUser(username, password, isAdmin)) {
        socket->write(QString("User %1 registered\r\n").arg(username).toUtf8());
    } else {
        socket->write("User already exists\r\n");
    }
}

///
/// \brief Удаление пользователя из системы
/// \param socket Сокет для ответа
/// \param username Удаляемый логин
/// \param currentUsername Инициатор операции
/// \note Требует прав администратора. Отправляет статус операции
///
void FunctionsForServer::deleteUser(QTcpSocket *socket, const QString &username, const QString &currentUsername) {
    if (!DB::instance().isAdmin(currentUsername)) {
        socket->write("Permission denied\r\n");
        return;
    }

    if (DB::instance().deleteUser(username)) {
        socket->write(QString("User %1 deleted\r\n").arg(username).toUtf8());
    } else {
        socket->write("User not found\r\n");
    }
}

///
/// \brief Обработка решения математической задачи
/// \param socket Сокет для взаимодействия
/// \param problem Название задачи
/// \param answer Ответ пользователя (пустой для запроса)
/// \param username Решающий пользователь
/// \note Обновляет статистику пользователя. Проверяет существование задачи.
///
void FunctionsForServer::solveMathProblem(QTcpSocket *socket, const QString &problem, const QString &answer, const QString &username) {
    QString correctAnswer = DB::instance().getProblemAnswer(problem);
    if (correctAnswer.isEmpty()) {
        socket->write("Problem not found\r\n");
    } else if (answer.isEmpty()) {
        socket->write("Enter your answer: ");
    } else {
        bool solved = (correctAnswer == answer);
        DB::instance().updateUserStats(username, problem, solved);
        if (solved) {
            socket->write("Correct answer!\r\n");
        } else {
            socket->write("Incorrect answer!\r\n");
        }
    }
}

///
/// \brief Получение списка доступных задач
/// \param socket Сокет для ответа
/// \note Формат вывода: "Available problems:\r\n<список>\r\n"
///
void FunctionsForServer::listProblems(QTcpSocket *socket) {
    QSqlQuery query("SELECT problem FROM problems");
    QString problemList = "Available problems:\r\n";
    while (query.next()) {
        problemList += query.value(0).toString() + "\r\n";
    }
    problemList += "Enter 'solve <problem>' to solve a problem.\r\n";
    socket->write(problemList.toUtf8());
}

///
/// \brief Добавление новой задачи
/// \param socket Сокет для ответа
/// \param problem Текст задачи
/// \param answer Верный ответ
/// \param currentUsername Инициатор операции
/// \note Требует прав администратора. Проверяет дубликаты.
///
void FunctionsForServer::addProblem(QTcpSocket *socket, const QString &problem, const QString &answer, const QString &currentUsername) {
    if (!DB::instance().isAdmin(currentUsername)) {
        socket->write("Permission denied\r\n");
        return;
    }

    if (DB::instance().addProblem(problem, answer)) {
        socket->write(QString("Problem added: %1\r\n").arg(problem).toUtf8());
    } else {
        socket->write("Failed to add problem\r\n");
    }
}

///
/// \brief Обновление ответа задачи
/// \param socket Сокет для ответа
/// \param problem Название задачи
/// \param newAnswer Новый верный ответ
/// \param currentUsername Инициатор операции
/// \note Требует прав администратора. Проверяет существование задачи.
///
void FunctionsForServer::updateProblem(QTcpSocket *socket, const QString &problem, const QString &newAnswer, const QString &currentUsername) {
    if (!DB::instance().isAdmin(currentUsername)) {
        socket->write("Permission denied\r\n");
        return;
    }

    if (DB::instance().updateProblem(problem, newAnswer)) {
        socket->write(QString("Problem updated: %1\r\n").arg(problem).toUtf8());
    } else {
        socket->write("Failed to update problem\r\n");
    }
}

///
/// \brief Получение статистики пользователя
/// \param socket Сокет для ответа
/// \param username Целевой пользователь
/// \param currentUsername Инициатор запроса
/// \note Требует прав администратора. Формат данных зависит от реализации DB::getUserStats()
///
void FunctionsForServer::viewUserStats(QTcpSocket *socket, const QString &username, const QString &currentUsername) {
    if (!DB::instance().isAdmin(currentUsername)) {
        socket->write("Permission denied\r\n");
        return;
    }

    QString stats = DB::instance().getUserStats(username);
    socket->write(stats.toUtf8());
}

/// \brief Обрабатывает команду userstats для просмотра общей статистики
/// \param socket Сокет для ответа
/// \param currentUsername Инициатор запроса
/// \note Требует прав администратора. Формат данных:
/// "User: <логин> | Total: <задач> | Correct: <решено> | Last Login: <время>"
///
void FunctionsForServer::showUserStats(QTcpSocket *socket, const QString &currentUsername) {
    if (!DB::instance().isAdmin(currentUsername)) {
        socket->write("Permission denied\r\n");
        return;
    }
    QString stats = DB::instance().getUserStatistics();
    socket->write(stats.toUtf8());
}
