///
/// \file mytcpserver.cpp
/// \brief Реализация TCP-сервера для обработки клиентских подключений
///

#include "mytcpserver.h"
#include <QDebug>

///
/// \brief Конструктор TCP-сервера
/// \param parent Родительский объект Qt
/// \details
/// 1. Инициализирует TCP-сервер на порту 33333
/// 2. Подключается к SQLite БД "math_server.db"
/// 3. Создает таблицы (users, problems, user_stats), если они отсутствуют
/// 4. Добавляет тестовые данные:
///    - Администратор: логин "admin", пароль "adminpass"
///    - Примеры задач: "2+2", "3*3", "10-5"
///
/// \note При ошибке запуска сервера выводит сообщение в консоль
///
MyTcpServer::MyTcpServer(QObject *parent) : QObject(parent)
{
    mTcpServer = new QTcpServer(this);
    connect(mTcpServer, &QTcpServer::newConnection, this, &MyTcpServer::slotNewConnection);

    if (!mTcpServer->listen(QHostAddress::Any, 33333)) {
        qDebug() << "Server is not started";
    } else {
        qDebug() << "Server is started";
    }

    // Подключаемся к базе данных
    DB& db = DB::instance();
    if (!db.openDatabase("math_server.db")) {
        qDebug() << "Failed to connect to database.";
    }

    // Создаем таблицы, если их нет
    db.executeQuery("CREATE TABLE IF NOT EXISTS users ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                    "username TEXT UNIQUE NOT NULL, "
                    "password TEXT NOT NULL, "
                    "isAdmin INTEGER DEFAULT 0, "
                    "last_login DATETIME)");

    db.executeQuery("CREATE TABLE IF NOT EXISTS problems ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                    "problem TEXT UNIQUE NOT NULL, "
                    "answer TEXT NOT NULL)");

    db.executeQuery("CREATE TABLE IF NOT EXISTS user_stats ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                    "username TEXT NOT NULL, "
                    "problem TEXT NOT NULL, "
                    "solved INTEGER DEFAULT 0, "
                    "FOREIGN KEY (username) REFERENCES users(username))");

    // Добавляем администратора по умолчанию
    db.registerUser("admin", "adminpass", true);

    // Добавляем задачи по умолчанию
    db.addProblem("2+2", "4");
    db.addProblem("3*3", "9");
    db.addProblem("10-5", "5");
}

///
/// \brief Деструктор TCP-сервера
/// \note Корректно завершает работу сервера
///
MyTcpServer::~MyTcpServer()
{
    mTcpServer->close();
}

///
/// \brief Обработчик новых подключений
/// \details:
/// - Отправляет приветственное сообщение
/// - Подключает сигналы для чтения данных и отключения
///
void MyTcpServer::slotNewConnection()
{
    QTcpSocket *socket = mTcpServer->nextPendingConnection();
    socket->write("Hello! You are connected to the Math Server.\r\n");
    connect(socket, &QTcpSocket::readyRead, this, &MyTcpServer::slotServerRead);
    connect(socket, &QTcpSocket::disconnected, this, &MyTcpServer::slotClientDisconnected);
}

///
/// \brief Обработчик команды userstats
/// \details Добавлена проверка прав администратора:
/// \code
/// if (!DB::instance().isAdmin(username)) { ... }
/// \endcode
/// \note Неаутентифицированным пользователям блокируются все команды, кроме auth/register
///
void MyTcpServer::slotServerRead()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket) return;

    QString command = socket->readAll().trimmed();
    qDebug() << "Received command:" << command;

    QString username = clientLogins.value(socket, "");

    // Если команда auth, обновляем clientLogins
    QStringList parts = command.split(" ");
    if (parts.size() >= 3 && parts[0] == "auth") {
        QString username = parts[1];
        QString password = parts[2];

        if (DB::instance().authenticateUser(username, password)) {
            clientLogins[socket] = username; // уПИТБОСЕН МПЗЙО РПМШЪПЧБФЕМС
            socket->write("Authentication successful\r\n");
            return;
        } else {
            socket->write("Authentication failed\r\n");
            return;
        }
    }

    // еУМЙ РПМШЪПЧБФЕМШ ОЕ БХФЕОФЙЖЙГЙТПЧБО, ВМПЛЙТХЕН ЧУЕ ЛПНБОДЩ, ЛТПНЕ register Й auth
    if (username.isEmpty()) {
        socket->write("You need to authenticate first. Use 'auth <username> <password>'.\r\n");
        return;
    }

    // рТПЧЕТЛБ РТБЧ БДНЙОЙУФТБФПТБ ДМС ЛПНБОДЩ userstats
    if (parts[0] == "userstats") {
        if (!DB::instance().isAdmin(username)) {
            socket->write("Permission denied. Admin rights required.\r\n");
            return;
        }
    }

    functions.parse(socket, command, username);
}

///
/// \brief Обработчик отключения клиента
/// \details:
/// - Удаляет сокет из списка активных подключений
/// - Освобождает ресурсы сокета
///
void MyTcpServer::slotClientDisconnected()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    if (socket) {
        clientLogins.remove(socket);
        socket->close();
        socket->deleteLater();
    }
}
