///
/// \file mytcpserver.cpp
/// \brief Реализация TCP-сервера для математического сервиса
///

#include "mytcpserver.h"
#include <QDebug>
#include "functionsforserver.h"

///
/// \brief Конструктор TCP-сервера
/// \param parent Родительский объект
///
MyTcpServer::MyTcpServer(QObject* parent) : QObject(parent),
server(new QTcpServer(this)),
db(DB::instance()),
maxClients(100) {
}

///
/// \brief Деструктор TCP-сервера
///
/// Останавливает сервер при уничтожении объекта
///
MyTcpServer::~MyTcpServer() {
    stop();
}

///
/// \brief Запуск сервера на указанном порту
/// \param port Порт для прослушивания
/// \return true в случае успешного запуска, false в случае ошибки
///
/// Инициализирует подключение к базе данных и создает администратора по умолчанию
///
bool MyTcpServer::start(quint16 port) {
    if (!server->listen(QHostAddress::Any, port)) {
        qCritical() << "Server failed to start:" << server->errorString();
        return false;
    }

    connect(server, &QTcpServer::newConnection, this, &MyTcpServer::onNewConnection);
    qInfo() << "Server started on port" << port;

    if (!db.openDatabase()) {
        qCritical() << "Failed to open database";
        return false;
    }

    // Добавляем администратора по умолчанию, если его нет
    if (DB::instance().getUserId("admin") == -1) {
        DB::instance().registerUser("admin", "admin123", true);
    }

    return true;
}

///
/// \brief Остановка сервера
///
void MyTcpServer::stop() {
    if (server->isListening()) {
        server->close();
        qInfo() << "Server stopped";
    }
}

///
/// \brief Установка максимального количества клиентов
/// \param max Максимальное число подключений
///
void MyTcpServer::setMaxClients(int max) {
    maxClients = max;
}

///
/// \brief Обработчик нового подключения
///
/// Проверяет наличие свободных слотов для подключения.
/// При превышении лимита отправляет сообщение о перегрузке сервера.
///
void MyTcpServer::onNewConnection() {
    if (clients.size() >= maxClients) {
        QTcpSocket* client = server->nextPendingConnection();
        client->write("Server is full. Try again later.\r\n");
        client->disconnectFromHost();
        return;
    }

    while (server->hasPendingConnections()) {
        QTcpSocket* client = server->nextPendingConnection();
        clients.insert(client);
        clientBuffers[client].clear();

        connect(client, &QTcpSocket::disconnected, this, &MyTcpServer::onClientDisconnected);
        connect(client, &QTcpSocket::readyRead, this, &MyTcpServer::onReadyRead);

        emit clientConnected(client->socketDescriptor());
        client->write("Connected to Math Server. Use 'auth <username> <password>' to login.\r\n"
            "Or 'register <username> <password>' to create an account.\r\n");
    }
}

///
/// \brief Обработчик отключения клиента
///
/// Удаляет клиента из списка активных подключений
///
void MyTcpServer::onClientDisconnected() {
    QTcpSocket* client = qobject_cast<QTcpSocket*>(sender());
    if (client) {
        emit clientDisconnected(client->socketDescriptor());
        clients.remove(client);
        clientBuffers.remove(client);
        client->deleteLater();
    }
}

///
/// \brief Обработчик входящих данных
///
/// Читает данные от клиента и обрабатывает команды построчно
///
void MyTcpServer::onReadyRead() {
    QTcpSocket* client = qobject_cast<QTcpSocket*>(sender());
    if (!client) return;

    QByteArray data = client->readAll();
    clientBuffers[client] += data;

    while (clientBuffers[client].contains('\n')) {
        int pos = clientBuffers[client].indexOf('\n');
        QByteArray command = clientBuffers[client].left(pos).trimmed();
        clientBuffers[client] = clientBuffers[client].mid(pos + 1);

        if (!command.isEmpty()) {
            QString cmdStr = QString::fromUtf8(command);
            QString username = client->property("username").toString();

            if (client->property("expecting_answer").toBool()) {
                QString problem = client->property("current_problem").toString();
                QString correctAnswer = db.getProblemAnswer(problem);
                bool solved = (correctAnswer == cmdStr);

                db.recordAttempt(username, problem, solved);
                sendResponse(client, solved ? "Correct answer!\r\n" : QString("Incorrect answer! Correct was: %1\r\n").arg(correctAnswer).toUtf8());

                client->setProperty("expecting_answer", false);
                client->setProperty("current_problem", QVariant());
            }
            else {
                processCommand(client, cmdStr);
            }
        }
    }
}

///
/// \brief Обработка команды от клиента
/// \param client Сокет клиента
/// \param command Текст команды
///
void MyTcpServer::processCommand(QTcpSocket* client, const QString& command) {
    QString username = client->property("username").toString();
    FunctionsForServer funcs;
    funcs.parse(client, command, username);
}

///
/// \brief Отправка ответа клиенту
/// \param client Сокет клиента
/// \param response Текст ответа
///
void MyTcpServer::sendResponse(QTcpSocket* client, const QString& response) {
    if (client && client->state() == QAbstractSocket::ConnectedState) {
        client->write(response.toUtf8());
        client->flush();
    }
}
