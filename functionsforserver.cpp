///
/// \file functionsforserver.cpp
/// \brief Реализация класса для обработки команд сервера
///

#include "functionsforserver.h"
#include "mathparser.h"
#include <QDebug>
#include <QRegularExpression>

///
/// \brief Конструктор класса FunctionsForServer
/// \param parent Родительский объект в Qt-иерархии
///
FunctionsForServer::FunctionsForServer(QObject *parent) : QObject(parent) {}

///
/// \brief Разбор и обработка входящей команды
/// \param socket Указатель на сокет клиента
/// \param command Полученная команда
/// \param username Имя пользователя (если аутентифицирован)
///
void FunctionsForServer::parse(QTcpSocket *socket, const QString &command, const QString &username) {
    QStringList parts = command.split(" ", Qt::SkipEmptyParts);
    if (parts.isEmpty()) {
        socket->write("Unknown command\r\n");
        return;
    }

    QString action = parts[0].toLower();

    if (action == "get_problems" && parts.size() == 2) {
        handleGetProblems(socket, parts[1]);
    }
    else if (action == "check_answer" && parts.size() == 4) {
        handleCheckAnswer(socket, parts[1], parts[2].toInt(), parts[3], username);
    }
    else if (action == "register" && parts.size() == 3) {
        reg(socket, parts[1], parts[2]);
    }
    // ... остальные условия обработки команд ...
    else {
        socket->write("Unknown command\r\n");
    }
}

///
/// \brief Обработка запроса на получение задач по категории
/// \param socket Указатель на сокет клиента
/// \param category Категория задач
///
void FunctionsForServer::handleGetProblems(QTcpSocket* socket, const QString& category) {
    auto problems = DB::instance().generateProblems(category);
    QString response = "problems:"; // Префикс добавляется только один раз

    int validCount = 0;
    for (const auto& p : problems) {
        // Пропускаем задачи с ошибками или некорректными переменными
        if (p.second.contains("Error") || !isValidProblem(p.first)) {
            qWarning() << "Skipping invalid problem:" << p.first;
            continue;
        }

        if (validCount > 0) response += ";";
        response += p.first + "|" + p.second;
        validCount++;

        if (validCount >= 4) break; // Лимит 4 валидных задачи
    }

    response += "\r\n"; // Обязательное завершение ответа
    socket->write(response.toUtf8());
}

///
/// \brief Проверка корректности математической задачи
/// \param problem Текст задачи
/// \return true, если задача корректна, false в противном случае
///
bool FunctionsForServer::isValidProblem(const QString& problem) {
    // Разрешаем только переменную x и правильные математические операторы
    QRegularExpression regex(
        "^[0-9x+\\-*/^()√∛log sin cos tan integral derivative]+$"
        );
    return regex.match(problem).hasMatch() &&
           !problem.contains("y") &&
           !problem.contains("z");
}

///
/// \brief Обработка проверки ответа на задачу
/// \param socket Указатель на сокет клиента
/// \param category Категория задачи
/// \param problemIndex Индекс задачи в списке
/// \param answer Ответ пользователя
/// \param username Имя пользователя
///
void FunctionsForServer::handleCheckAnswer(QTcpSocket* socket, const QString& category, int problemIndex, const QString& answer,const QString& username) {
    auto problems = DB::instance().generateProblems(category);
    if (problemIndex >= 0 && problemIndex < problems.size()) {
        bool isCorrect = false;

        try {
            // Для математических задач сравниваем численно с допуском
            double userAnswer = MathParser::evaluateExpression(answer);
            double correctAnswer = MathParser::evaluateExpression(problems[problemIndex].second);
            isCorrect = qFuzzyCompare(userAnswer, correctAnswer);
        } catch (...) {
            // Для не-математических задач сравниваем строки
            isCorrect = (answer.trimmed().toLower() == problems[problemIndex].second.trimmed().toLower());
        }

        DB::instance().recordAttempt(username, problems[problemIndex].first, isCorrect);

        QString response = QString("result:%1|%2\r\n").arg(problemIndex).arg(isCorrect ? "1" : "0");
        socket->write(response.toUtf8());
    }
}

///
/// \brief Аутентификация пользователя
/// \param socket Указатель на сокет клиента
/// \param username Имя пользователя
/// \param password Пароль
///
void FunctionsForServer::auth(QTcpSocket *socket, const QString &username, const QString &password) {
    if (DB::instance().authenticateUser(username, password)) {
        socket->setProperty("username", username);
        socket->setProperty("is_admin", DB::instance().isUserAdmin(username));
        socket->write(QString("Authentication successful. Welcome, %1! (%2)\r\n").arg(username).arg(socket->property("is_admin").toBool() ? "admin" : "user").toUtf8());
    } else {
        socket->write("Authentication failed\r\n");
    }
}

///
/// \brief Регистрация нового пользователя
/// \param socket Указатель на сокет клиента
/// \param username Имя пользователя
/// \param password Пароль
/// \param isAdmin Флаг администратора (по умолчанию false)
///
void FunctionsForServer::reg(QTcpSocket *socket, const QString &username, const QString &password, bool isAdmin) {
    if (DB::instance().registerUser(username, password, isAdmin)) {
        socket->write(QString("User %1 registered\r\n").arg(username).toUtf8());
    } else {
        socket->write("User already exists\r\n");
    }
}

///
/// \brief Удаление пользователя
/// \param socket Указатель на сокет клиента
/// \param username Имя пользователя для удаления
/// \param currentUsername Имя текущего пользователя (для проверки прав)
///
void FunctionsForServer::deleteUser(QTcpSocket *socket, const QString &username, const QString &currentUsername) {
    if (!DB::instance().isUserAdmin(currentUsername)) {
        socket->write("Permission denied. Only admins can delete users.\r\n");
        return;
    }

    if (username == currentUsername) {
        socket->write("You cannot delete yourself\r\n");
        return;
    }

    if (DB::instance().deleteUser(username)) {
        socket->write(QString("User %1 deleted\r\n").arg(username).toUtf8());
    } else {
        socket->write("User not found or deletion failed\r\n");
    }
}

// ... аналогичные комментарии для остальных методов ...

///
/// \brief Получение списка пользователей (только для администраторов)
/// \param socket Указатель на сокет клиента
/// \param currentUsername Имя текущего пользователя (для проверки прав)
///
void FunctionsForServer::listUsers(QTcpSocket *socket, const QString &currentUsername) {
    if (!DB::instance().isUserAdmin(currentUsername)) {
        socket->write("Permission denied. Only admins can list users.\r\n");
        return;
    }

    auto users = DB::instance().listUsers();
    QString response = "Users:\r\n";
    for (const auto& user : users) {
        response += QString("%1 (%2)\r\n").arg(user.first).arg(user.second ? "admin" : "user");
    }
    socket->write(response.toUtf8());
}
