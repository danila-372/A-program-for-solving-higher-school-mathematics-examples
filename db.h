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
#include <QCryptographicHash>
#include <QVector>
#include <QPair>
#include <QDateTime>

///
/// \brief Класс для работы с базой данных приложения
/// \note Реализует паттерн Singleton
///
class DB : public QObject
{
    Q_OBJECT
public:
    ///
    /// \brief Получение экземпляра класса (реализация Singleton)
    /// \return Ссылка на единственный экземпляр класса
    ///
    static DB& instance();

    ///
    /// \brief Открытие базы данных
    /// \param dbName Имя файла базы данных (по умолчанию "server.db")
    /// \return true в случае успешного открытия, false в случае ошибки
    ///
    bool openDatabase(const QString& dbName = "server.db");

    ///
    /// \brief Закрытие соединения с базой данных
    ///
    void closeDatabase();

    ///
    /// \brief Инициализация примеров задач
    ///
    void initSampleProblems();

    ///
    /// \brief Очистка тестовых данных
    /// \note Сохраняет только администратора и системные данные
    ///
    void clearTestData();

    // Управление пользователями

    ///
    /// \brief Регистрация нового пользователя
    /// \param username Имя пользователя
    /// \param password Пароль
    /// \param isAdmin Флаг администратора (по умолчанию false)
    /// \return true в случае успешной регистрации, false в случае ошибки
    ///
    bool registerUser(const QString &username, const QString &password, bool isAdmin = false);

    ///
    /// \brief Аутентификация пользователя
    /// \param username Имя пользователя
    /// \param password Пароль
    /// \return true, если аутентификация успешна, false в противном случае
    ///
    bool authenticateUser(const QString &username, const QString &password);

    ///
    /// \brief Удаление пользователя
    /// \param username Имя пользователя для удаления
    /// \return true в случае успешного удаления, false в случае ошибки
    ///
    bool deleteUser(const QString &username);

    ///
    /// \brief Получение списка пользователей
    /// \return Вектор пар (имя пользователя, флаг администратора)
    ///
    QVector<QPair<QString, bool>> listUsers();

    ///
    /// \brief Получение ID пользователя
    /// \param username Имя пользователя
    /// \return ID пользователя или -1, если пользователь не найден
    ///
    int getUserId(const QString &username);

    // Управление ролями

    ///
    /// \brief Назначение роли пользователю
    /// \param username Имя пользователя
    /// \param role Название роли
    /// \return true в случае успешного назначения, false в случае ошибки
    ///
    bool assignRole(const QString &username, const QString &role);

    ///
    /// \brief Отзыв роли у пользователя
    /// \param username Имя пользователя
    /// \param role Название роли
    /// \return true в случае успешного отзыва, false в случае ошибки
    ///
    bool revokeRole(const QString &username, const QString &role);

    ///
    /// \brief Проверка наличия роли у пользователя
    /// \param username Имя пользователя
    /// \param role Название роли
    /// \return true, если пользователь имеет указанную роль, false в противном случае
    ///
    bool hasRole(const QString &username, const QString &role);

    ///
    /// \brief Проверка, является ли пользователь администратором
    /// \param username Имя пользователя
    /// \return true, если пользователь администратор, false в противном случае
    ///
    bool isUserAdmin(const QString &username);

    ///
    /// \brief Назначение пользователю роли администратора
    /// \param adminUsername Имя администратора, выполняющего операцию
    /// \param targetUsername Имя пользователя для повышения
    /// \return true в случае успешного назначения, false в случае ошибки
    ///
    bool promoteToAdmin(const QString &adminUsername, const QString &targetUsername);

    // Управление задачами

    ///
    /// \brief Добавление новой задачи
    /// \param problem Текст задачи
    /// \param answer Ответ к задаче (необязательный параметр)
    /// \return true в случае успешного добавления, false в случае ошибки
    ///
    bool addProblem(const QString &problem, const QString &answer = "");

    ///
    /// \brief Обновление ответа к задаче
    /// \param problem Текст задачи
    /// \param newAnswer Новый ответ
    /// \return true в случае успешного обновления, false в случае ошибки
    ///
    bool updateProblem(const QString &problem, const QString &newAnswer);

    ///
    /// \brief Получение списка всех задач
    /// \return Вектор пар (текст задачи, ответ)
    ///
    QVector<QPair<QString, QString>> getProblems();

    ///
    /// \brief Получение ответа к задаче
    /// \param problem Текст задачи
    /// \return Ответ к задаче или пустую строку, если задача не найдена
    ///
    QString getProblemAnswer(const QString &problem);

    ///
    /// \brief Генерация задач по категории
    /// \param category Категория задач
    /// \return Вектор пар (текст задачи, ответ)
    ///
    QVector<QPair<QString, QString>> generateProblems(const QString &category);

    ///
    /// \brief Получение случайной задачи по категории
    /// \param category Категория задачи
    /// \return Текст задачи или пустую строку, если задачи не найдены
    ///
    QString getRandomProblem(const QString &category);

    // Статистика

    ///
    /// \brief Запись попытки решения задачи
    /// \param username Имя пользователя
    /// \param problem Текст задачи
    /// \param solved Флаг успешного решения
    /// \return true в случае успешной записи, false в случае ошибки
    ///
    bool recordAttempt(const QString &username, const QString &problem, bool solved);

    ///
    /// \brief Обновление статистики пользователя
    /// \param username Имя пользователя
    /// \param problem Текст задачи
    /// \param solved Флаг успешного решения
    /// \return true в случае успешного обновления, false в случае ошибки
    ///
    bool updateUserStats(const QString &username, const QString &problem, bool solved);

    ///
    /// \brief Получение статистики пользователя
    /// \param username Имя пользователя
    /// \return Строка с подробной статистикой пользователя
    ///
    QString getUserStats(const QString &username);

    ///
    /// \brief Получение статистики всех пользователей
    /// \return Строка с общей статистикой пользователей
    ///
    QString getUserStatistics();

private:
    ///
    /// \brief Приватный конструктор (реализация Singleton)
    /// \param parent Родительский объект в Qt-иерархии
    ///
    DB(QObject *parent = nullptr);

    ///
    /// \brief Деструктор
    ///
    ~DB();

    // Запрет копирования
    DB(const DB&) = delete;
    DB& operator=(const DB&) = delete;

    ///
    /// \brief Хеширование пароля
    /// \param password Пароль для хеширования
    /// \return Хеш пароля
    ///
    QString hashPassword(const QString &password);

    ///
    /// \brief Получение ID задачи
    /// \param problem Текст задачи
    /// \return ID задачи или -1, если задача не найдена
    ///
    int getProblemId(const QString &problem);

    ///
    /// \brief Получение ID роли
    /// \param roleName Название роли
    /// \return ID роли или -1, если роль не найдена
    ///
    int getRoleId(const QString &roleName);

    QSqlDatabase db; ///< Объект для работы с базой данных
};

#endif // DB_H