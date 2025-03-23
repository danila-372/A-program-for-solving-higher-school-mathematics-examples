///
/// \file db.h
/// \brief Заголовочный файл класса для работы с базой данных
///

#ifndef DB_H
#define DB_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QCryptographicHash>
///
/// \brief Класс-синглтон для управления базой данных
///
/// \details Реализует:
/// - Подключение/отключение SQLite БД
/// - Регистрацию и аутентификацию пользователей
/// Управление математическими задачами
/// - Статистику решений
///
/// \note Использует паттерн Singleton. Все методы работы с БД требуют предварительного вызова openDatabase()
///

///
/// \brief The DB class
///
class DB : public QObject
{
    Q_OBJECT

public:

    ///
    /// \brief Получение единственного экземпляра класса
    /// \return Ссылка на объект DB
    ///
    static DB& instance() {
        static DB instance;
        return instance;
    }

    /// \name Управление подключением
    /// \{
    bool openDatabase(const QString& dbName);
    void closeDatabase();
    bool executeQuery(const QString& queryStr);
    /// \}

    /// \name Работа с пользователями
    /// \{
    bool registerUser(const QString &username, const QString &password, bool isAdmin = false);
    bool authenticateUser(const QString &username, const QString &password);
    bool deleteUser(const QString &username);
    /// \}

    /// \name Управление задачами
    /// \{
    bool addProblem(const QString &problem, const QString &answer);
    bool updateProblem(const QString &problem, const QString &newAnswer);
    QString getProblemAnswer(const QString &problem);
    /// \}

    /// \name Статистика решений
    /// \{
    QString getUserStats(const QString &username);
    bool updateUserStats(const QString &username, const QString &problem, bool solved);
    /// \}

    ///
    /// \brief Проверка администраторских прав
    /// \param username Логин пользователя
    /// \return true если пользователь имеет права администратора
    ///
    bool isAdmin(const QString &username);

    ///
    /// \brief Обновляет время последнего входа пользователя
    /// \param username Логин пользователя
    /// \return true при успешном обновлении
    /// \note Использует SQL-функцию CURRENT_TIMESTAMP
    ///
    bool updateLastLogin(const QString &username);

    ///
    /// \brief Формирует общую статистику всех пользователей
    /// \return Строка с данными в формате:
    /// "User: <логин> | Total: <задач> | Correct: <решено> | Last Login: <время>"
    /// \note Использует LEFT JOIN между таблицами users и user_stats
    ///
    QString getUserStatistics();

private:
    DB() {}     ///< Приватный конструктор (singleton) ///
    ~DB() {
        closeDatabase();
    }

    DB(const DB&) = delete;             ///< Запрет копирования ///
    DB& operator=(const DB&) = delete;  ///< Запрет присваивания ///

    ///
    /// \brief Генерация хеша пароля
    /// \param password Исходный пароль
    /// \return SHA-256 хеш в HEX-формате
    ///
    QString hashPassword(const QString &password);

    QSqlDatabase db;    ///< Объект соединения с базой данных ///
};

#endif // DB_H
