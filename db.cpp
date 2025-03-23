///
/// \file db.cpp
/// \brief Реализация методов работы с базой данных
///

#include "db.h"
#include <QDateTime>

///
/// \brief Открывает соединение с SQLite базой данных
/// \param dbName Имя файла базы данных
/// \return true при успешном подключении, false при ошибке
/// \note Выводит сообщение об ошибке в консоль при неудаче
///
bool DB::openDatabase(const QString& dbName) {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbName);
    if (!db.open()) {
        qDebug() << "Error: Could not open database." << db.lastError().text();
        return false;
    }
    return true;
}

///
/// \brief Закрывает активное соединение с базой данных
///
void DB::closeDatabase() {
    if (db.isOpen()) {
        db.close();
    }
}

///
/// \brief Выполняет произвольный SQL-запрос
/// \param queryStr Строка с SQL-запросом
/// \return true при успешном выполнении, false при ошибке
/// \note Для INSERT/UPDATE/DELETE. Не используется для выборки данных
///
bool DB::executeQuery(const QString& queryStr) {
    QSqlQuery query(db);
    if (!query.exec(queryStr)) {
        qDebug() << "Error executing query:" << query.lastError().text();
        return false;
    }
    return true;
}

///
/// \brief Регистрирует нового пользователя в системе
/// \param username Логин пользователя
/// \param password Пароль в открытом виде
/// \param isAdmin Флаг администраторских прав
/// \return true при успешной регистрации
/// \note Пароль хранится в виде SHA-256 хеша
///
bool DB::registerUser(const QString &username, const QString &password, bool isAdmin) {
    QSqlQuery query(db);
    query.prepare("INSERT INTO users (username, password, isAdmin) VALUES (:username, :password, :isAdmin)");
    query.bindValue(":username", username);
    query.bindValue(":password", hashPassword(password));
    query.bindValue(":isAdmin", isAdmin ? 1 : 0);

    if (!query.exec()) {
        qDebug() << "Error: Could not register user." << query.lastError();
        return false;
    }
    return true;
}

///
/// \brief Проверяет учетные данные пользователя
/// \param username Логин для проверки
/// \param password Пароль в открытом виде
/// \return true при совпадении хешей паролей
/// \note Сравнение происходит через хеши SHA-256
///
bool DB::authenticateUser(const QString &username, const QString &password) {
    QSqlQuery query(db);
    query.prepare("SELECT password FROM users WHERE username = :username");
    query.bindValue(":username", username);

    if (!query.exec() || !query.next()) {
        qDebug() << "Error: Could not authenticate user." << query.lastError();
        return false;
    }

    QString storedPassword = query.value(0).toString();
    return (storedPassword == hashPassword(password));
}

///
/// \brief Удаляет пользователя из системы
/// \param username Логин для удаления
/// \return true при успешном удалении
/// \note Не требует подтверждения прав. Должна быть внешняя проверка
///
bool DB::deleteUser(const QString &username) {
    QSqlQuery query(db);
    query.prepare("DELETE FROM users WHERE username = :username");
    query.bindValue(":username", username);

    if (!query.exec()) {
        qDebug() << "Error: Could not delete user." << query.lastError();
        return false;
    }
    return true;
}

///
/// \brief Добавляет новую математическую задачу
/// \param problem Текст задачи
/// \param answer Правильный ответ
/// \return true при успешном добавлении
/// \note Требует предварительной проверки прав администратора
///
bool DB::addProblem(const QString &problem, const QString &answer) {
    QSqlQuery query(db);
    query.prepare("INSERT INTO problems (problem, answer) VALUES (:problem, :answer)");
    query.bindValue(":problem", problem);
    query.bindValue(":answer", answer);

    if (!query.exec()) {
        qDebug() << "Error: Could not add problem." << query.lastError();
        return false;
    }
    return true;
}

///
/// \brief Обновляет ответ для существующей задачи
/// \param problem Название задачи
/// \param newAnswer Новый верный ответ
/// \return true при успешном обновлении
///
bool DB::updateProblem(const QString &problem, const QString &newAnswer) {
    QSqlQuery query(db);
    query.prepare("UPDATE problems SET answer = :answer WHERE problem = :problem");
    query.bindValue(":answer", newAnswer);
    query.bindValue(":problem", problem);

    if (!query.exec()) {
        qDebug() << "Error: Could not update problem." << query.lastError();
        return false;
    }
    return true;
}

///
/// \brief Получает правильный ответ для задачи
/// \param problem Название задачи
/// \return Ответ в виде строки или пустая строка при ошибке
///
QString DB::getProblemAnswer(const QString &problem) {
    QSqlQuery query(db);
    query.prepare("SELECT answer FROM problems WHERE problem = :problem");
    query.bindValue(":problem", problem);

    if (!query.exec() || !query.next()) {
        qDebug() << "Error: Could not get problem answer." << query.lastError();
        return "";
    }

    return query.value(0).toString();
}

///
/// \brief Формирует статистику решенных задач пользователя
/// \param username Логин пользователя
/// \return Форматированная строка вида "Задача: Статус\r\n..."
///
QString DB::getUserStats(const QString &username) {
    QSqlQuery query(db);
    query.prepare("SELECT problem, solved FROM user_stats WHERE username = :username");
    query.bindValue(":username", username);

    if (!query.exec()) {
        qDebug() << "Error: Could not get user stats." << query.lastError();
        return "";
    }

    QString stats;
    while (query.next()) {
        QString problem = query.value(0).toString();
        bool solved = query.value(1).toBool();
        stats += problem + ": " + (solved ? "Solved" : "Not solved") + "\r\n";
    }

    if (stats.isEmpty()) {
        stats = "No stats available for user " + username + "\r\n";
    }

    return stats;
}

///
/// \brief Обновляет статистику решения задач
/// \param username Логин пользователя
/// \param problem Название задачи
/// \param solved Флаг успешного решения
/// \return true при успешном обновлении
/// \note Использует INSERT OR REPLACE для обновления записей
///
bool DB::updateUserStats(const QString &username, const QString &problem, bool solved) {
    QSqlQuery query(db);
    query.prepare("INSERT OR REPLACE INTO user_stats (username, problem, solved) "
                  "VALUES (:username, :problem, :solved)");
    query.bindValue(":username", username);
    query.bindValue(":problem", problem);
    query.bindValue(":solved", solved ? 1 : 0);

    if (!query.exec()) {
        qDebug() << "Error: Could not update user stats." << query.lastError();
        return false;
    }
    return true;
}

///
/// \brief Проверяет наличие администраторских прав
/// \param username Логин пользователя
/// \return true если пользователь является администратором
///
bool DB::isAdmin(const QString &username) {
    QSqlQuery query(db);
    query.prepare("SELECT isAdmin FROM users WHERE username = :username");
    query.bindValue(":username", username);

    if (!query.exec() || !query.next()) {
        qDebug() << "Error: Could not check admin status." << query.lastError();
        return false;
    }

    return query.value(0).toBool();
}

///
/// \brief Генерирует SHA-256 хеш строки
/// \param password Исходный пароль
/// \return Хеш в HEX-формате (64 символа)
/// \note Внутренний метод. Не должен вызываться напрямую
///
QString DB::hashPassword(const QString &password) {
    QByteArray hash = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256);
    return QString(hash.toHex());
}

///
/// \brief Обновляет время последнего входа пользователя
/// \param username Логин пользователя
/// \return true при успешном обновлении
/// \note Устанавливает значение CURRENT_TIMESTAMP в поле last_login
///
bool DB::updateLastLogin(const QString &username) {
    QSqlQuery query(db);
    query.prepare("UPDATE users SET last_login = CURRENT_TIMESTAMP WHERE username = :username");
    query.bindValue(":username", username);
    return query.exec();
}

///
/// \brief Формирует общую статистику всех пользователей
/// \return Форматированная строка с данными:
///
/// Формат:
/// User: <логин> | Total: <всего задач> | Correct: <решено> | Last Login: <время>
///
/// \note Использует LEFT JOIN между таблицами users и user_stats
///
QString DB::getUserStatistics() {
    QSqlQuery query(db);
    query.prepare(
        "SELECT u.username, "
        "COUNT(us.problem) as total_tasks, "
        "SUM(us.solved) as correct_tasks, "
        "u.last_login "
        "FROM users u "
        "LEFT JOIN user_stats us ON u.username = us.username "
        "GROUP BY u.username"
        );

    if (!query.exec()) {
        qDebug() << "Error getting statistics:" << query.lastError();
        return "";
    }

    QString stats = "User Statistics:\r\n";
    while (query.next()) {
        QString username = query.value(0).toString();
        int total = query.value(1).toInt();
        int correct = query.value(2).toInt();
        QString lastLogin = query.value(3).toDateTime().toString("yyyy-MM-dd HH:mm:ss");

        stats += QString("User: %1 | Total: %2 | Correct: %3 | Last Login: %4\r\n")
                     .arg(username, QString::number(total), QString::number(correct), lastLogin);
    }
    return stats;
}

