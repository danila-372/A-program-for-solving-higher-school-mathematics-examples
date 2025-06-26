///
/// \file db.cpp
/// \brief Реализация класса для работы с базой данных
///

#include "db.h"
#include "mathparser.h"
#include <QDebug>
#include <QRandomGenerator>
#include <QDateTime>

///
/// \brief Получение экземпляра класса DB (реализация Singleton)
/// \return Ссылка на единственный экземпляр класса DB
///
DB& DB::instance() {
    static DB instance;
    return instance;
}

///
/// \brief Конструктор класса DB
/// \param parent Родительский объект в Qt-иерархии
///
DB::DB(QObject *parent) : QObject(parent) {
    db = QSqlDatabase::addDatabase("QSQLITE");
}

///
/// \brief Деструктор класса DB
/// \note Закрывает соединение с базой данных при уничтожении объекта
///
DB::~DB() {
    closeDatabase();
}

///
/// \brief Открытие базы данных и инициализация таблиц
/// \param dbName Имя файла базы данных
/// \return true в случае успешного открытия, false в случае ошибки
///
bool DB::openDatabase(const QString& dbName) {
    db.setDatabaseName(dbName);
    if (!db.open()) {
        qCritical() << "Cannot open database:" << db.lastError();
        return false;
    }

    QSqlQuery query;

    // Создание таблиц, если они не существуют
    query.exec("CREATE TABLE IF NOT EXISTS users ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "username TEXT UNIQUE NOT NULL, "
               "password TEXT NOT NULL, "
               "last_login DATETIME)");

    query.exec("CREATE TABLE IF NOT EXISTS roles ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "name TEXT UNIQUE NOT NULL)");

    query.exec("CREATE TABLE IF NOT EXISTS user_roles ("
               "user_id INTEGER NOT NULL, "
               "role_id INTEGER NOT NULL, "
               "PRIMARY KEY(user_id, role_id), "
               "FOREIGN KEY(user_id) REFERENCES users(id), "
               "FOREIGN KEY(role_id) REFERENCES roles(id))");

    query.exec("CREATE TABLE IF NOT EXISTS problems ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "problem TEXT UNIQUE NOT NULL, "
               "answer TEXT NOT NULL)");

    query.exec("CREATE TABLE IF NOT EXISTS user_stats ("
               "user_id INTEGER NOT NULL, "
               "problem_id INTEGER NOT NULL, "
               "solved INTEGER NOT NULL, "
               "attempts INTEGER DEFAULT 1, "
               "PRIMARY KEY(user_id, problem_id), "
               "FOREIGN KEY(user_id) REFERENCES users(id), "
               "FOREIGN KEY(problem_id) REFERENCES problems(id))");

    // Добавление стандартных ролей
    query.exec("INSERT OR IGNORE INTO roles (name) VALUES ('admin')");
    query.exec("INSERT OR IGNORE INTO roles (name) VALUES ('user')");

    // Создание администратора по умолчанию, если его нет
    if (!db.tables().contains("users") || getUserId("admin") == -1) {
        registerUser("admin", "admin123", true);
    }

    initSampleProblems();
    return true;
}

///
/// \brief Закрытие соединения с базой данных
///
void DB::closeDatabase() {
    if (db.isOpen()) {
        db.close();
    }
}

///
/// \brief Хеширование пароля с использованием SHA-256
/// \param password Пароль для хеширования
/// \return Хеш пароля в шестнадцатеричном формате
///
QString DB::hashPassword(const QString &password) {
    return QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex();
}

///
/// \brief Регистрация нового пользователя
/// \param username Имя пользователя
/// \param password Пароль
/// \param isAdmin Флаг администратора
/// \return true в случае успешной регистрации, false в случае ошибки
///
bool DB::registerUser(const QString &username, const QString &password, bool isAdmin) {
    QSqlQuery query;
    query.prepare("INSERT INTO users (username, password) VALUES (:username, :password)");
    query.bindValue(":username", username);
    query.bindValue(":password", hashPassword(password));

    if (!query.exec()) {
        return false;
    }

    if (isAdmin) {
        return assignRole(username, "admin");
    }
    return assignRole(username, "user");
}

///
/// \brief Получение ID задачи по её тексту
/// \param problem Текст задачи
/// \return ID задачи или -1, если задача не найдена
///
int DB::getProblemId(const QString &problem) {
    QSqlQuery query;
    query.prepare("SELECT id FROM problems WHERE problem = :problem");
    query.bindValue(":problem", problem);

    if (query.exec() && query.next()) {
        return query.value(0).toInt();
    }
    return -1;
}

///
/// \brief Аутентификация пользователя
/// \param username Имя пользователя
/// \param password Пароль
/// \return true, если аутентификация успешна, false в противном случае
///
bool DB::authenticateUser(const QString &username, const QString &password) {
    QSqlQuery query;
    query.prepare("SELECT password FROM users WHERE username = :username");
    query.bindValue(":username", username);

    bool authenticated = false;
    if (query.exec() && query.next()) {
        authenticated = (query.value(0).toString() == hashPassword(password));
    }

    if (authenticated) {
        // Обновляем время последнего входа
        QSqlQuery updateQuery;
        updateQuery.prepare("UPDATE users SET last_login = CURRENT_TIMESTAMP WHERE username = :username");
        updateQuery.bindValue(":username", username);
        updateQuery.exec();
    }

    return authenticated;
}

///
/// \brief Удаление пользователя
/// \param username Имя пользователя для удаления
/// \return true в случае успешного удаления, false в случае ошибки
///
bool DB::deleteUser(const QString &username) {
    QSqlQuery query;
    query.prepare("DELETE FROM users WHERE username = :username");
    query.bindValue(":username", username);
    return query.exec();
}

///
/// \brief Получение списка пользователей и их ролей
/// \return Вектор пар (имя пользователя, флаг администратора)
///
QVector<QPair<QString, bool>> DB::listUsers() {
    QVector<QPair<QString, bool>> users;
    QSqlQuery query("SELECT u.username, r.name FROM users u "
                    "LEFT JOIN user_roles ur ON u.id = ur.user_id "
                    "LEFT JOIN roles r ON ur.role_id = r.id");

    while (query.next()) {
        users.append({query.value(0).toString(), query.value(1).toString() == "admin"});
    }
    return users;
}

///
/// \brief Назначение роли пользователю
/// \param username Имя пользователя
/// \param role Название роли
/// \return true в случае успешного назначения, false в случае ошибки
///
bool DB::assignRole(const QString &username, const QString &role) {
    int userId = getUserId(username);
    int roleId = getRoleId(role);
    if (userId == -1 || roleId == -1) return false;

    QSqlQuery query;
    query.prepare("INSERT OR IGNORE INTO user_roles (user_id, role_id) VALUES (:user_id, :role_id)");
    query.bindValue(":user_id", userId);
    query.bindValue(":role_id", roleId);
    return query.exec();
}

///
/// \brief Отзыв роли у пользователя
/// \param username Имя пользователя
/// \param role Название роли
/// \return true в случае успешного отзыва, false в случае ошибки
///
bool DB::revokeRole(const QString &username, const QString &role) {
    int userId = getUserId(username);
    int roleId = getRoleId(role);
    if (userId == -1 || roleId == -1) return false;

    QSqlQuery query;
    query.prepare("DELETE FROM user_roles WHERE user_id = :user_id AND role_id = :role_id");
    query.bindValue(":user_id", userId);
    query.bindValue(":role_id", roleId);
    return query.exec();
}

///
/// \brief Проверка наличия роли у пользователя
/// \param username Имя пользователя
/// \param role Название роли
/// \return true, если пользователь имеет указанную роль, false в противном случае
///
bool DB::hasRole(const QString &username, const QString &role) {
    int userId = getUserId(username);
    int roleId = getRoleId(role);
    if (userId == -1 || roleId == -1) return false;

    QSqlQuery query;
    query.prepare("SELECT 1 FROM user_roles WHERE user_id = :user_id AND role_id = :role_id");
    query.bindValue(":user_id", userId);
    query.bindValue(":role_id", roleId);
    return query.exec() && query.next();
}

///
/// \brief Проверка, является ли пользователь администратором
/// \param username Имя пользователя
/// \return true, если пользователь администратор, false в противном случае
///
bool DB::isUserAdmin(const QString &username) {
    return hasRole(username, "admin");
}

///
/// \brief Назначение пользователю роли администратора
/// \param adminUsername Имя администратора, выполняющего операцию
/// \param targetUsername Имя пользователя для повышения
/// \return true в случае успешного назначения, false в случае ошибки
///
bool DB::promoteToAdmin(const QString &adminUsername, const QString &targetUsername) {
    if (!isUserAdmin(adminUsername)) {
        return false;
    }
    return assignRole(targetUsername, "admin");
}

///
/// \brief Получение ID пользователя по имени
/// \param username Имя пользователя
/// \return ID пользователя или -1, если пользователь не найден
///
int DB::getUserId(const QString &username) {
    QSqlQuery query;
    query.prepare("SELECT id FROM users WHERE username = :username");
    query.bindValue(":username", username);

    if (query.exec() && query.next()) {
        return query.value(0).toInt();
    }
    return -1;
}

///
/// \brief Получение ID роли по названию
/// \param roleName Название роли
/// \return ID роли или -1, если роль не найдена
///
int DB::getRoleId(const QString &roleName) {
    QSqlQuery query;
    query.prepare("SELECT id FROM roles WHERE name = :name");
    query.bindValue(":name", roleName);

    if (query.exec() && query.next()) {
        return query.value(0).toInt();
    }
    return -1;
}

///
/// \brief Добавление новой задачи
/// \param problem Текст задачи
/// \param answer Ответ к задаче (если пустой, будет вычислен автоматически)
/// \return true в случае успешного добавления, false в случае ошибки
///
bool DB::addProblem(const QString &problem, const QString &answer) {
    QString finalAnswer = answer;
    if (answer.isEmpty()) {
        auto result = MathParser::parseAndCalculate(problem);
        if (result.second.startsWith("Error")) {
            return false;
        }
        finalAnswer = result.second;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO problems (problem, answer) VALUES (:problem, :answer)");
    query.bindValue(":problem", problem);
    query.bindValue(":answer", finalAnswer
