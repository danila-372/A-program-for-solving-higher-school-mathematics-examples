///
/// \file loginview.cpp
/// \brief Реализация класса формы авторизации
///

#include "loginview.h"
#include "ui_loginview.h"
#include "regview.h"
#include "taskview.h"
#include <QMessageBox>
#include <QDebug>

///
/// \brief Конструктор формы авторизации
/// \param socket Указатель на TCP-сокет для соединения с сервером
/// \param parent Родительский виджет
///
/// Инициализирует интерфейс формы и устанавливает соединения:
/// - Проверяет валидность сокета
/// - Настраивает обработчики событий сокета
/// - Подключает сигналы кнопок к слотам
///
LoginView::LoginView(QTcpSocket* socket, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginView),
    m_socket(socket),
    regWindow(nullptr)
{
    ui->setupUi(this);

    if (!m_socket) {
        QMessageBox::critical(this, "Ошибка", "Недействительный сокет");
        return;
    }

    connect(m_socket, &QTcpSocket::connected, this, &LoginView::onSocketConnected);
    connect(m_socket, &QTcpSocket::readyRead, this, &LoginView::onSocketReadyRead);
    connect(ui->pushButton_auth, &QPushButton::clicked, this, &LoginView::on_pushButton_auth_clicked);
    connect(ui->pushButton_reg, &QPushButton::clicked, this, &LoginView::on_pushButton_reg_clicked);
}

///
/// \brief Деструктор формы авторизации
///
/// Освобождает ресурсы:
/// - Удаляет UI-форму
/// - Удаляет окно регистрации (если было создано)
///
LoginView::~LoginView()
{
    delete ui;
    if (regWindow) {
        regWindow->deleteLater();
    }
}

///
/// \brief Очистка полей формы
///
void LoginView::clearForm()
{
    ui->lineEdit_log->clear();
    ui->lineEdit_pass->clear();
}

///
/// \brief Обработчик нажатия кнопки авторизации
///
/// Выполняет:
/// 1. Проверку заполнения полей
/// 2. Подключение к серверу (если не подключены)
/// 3. Отправку команды аутентификации
///
void LoginView::on_pushButton_auth_clicked()
{
    QString login = ui->lineEdit_log->text().trimmed();
    QString password = ui->lineEdit_pass->text().trimmed();

    if (login.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Логин и пароль не могут быть пустыми");
        return;
    }

    if (!m_socket || m_socket->state() != QAbstractSocket::ConnectedState) {
        m_socket->connectToHost("127.0.0.1", 33333);
        if (!m_socket->waitForConnected(3000)) {
            QMessageBox::warning(this, "Ошибка", "Не удалось подключиться к серверу");
            return;
        }
    }

    QString authCmd = QString("auth %1 %2\r\n").arg(login).arg(password);
    m_socket->write(authCmd.toUtf8());
}

///
/// \brief Обработчик нажатия кнопки регистрации
///
/// Создает и показывает форму регистрации,
/// скрывая текущую форму авторизации
///
void LoginView::on_pushButton_reg_clicked()
{
    if (!regWindow) {
        regWindow = new RegView(m_socket);
        connect(regWindow, &RegView::backRequested, this, &LoginView::handleBackFromRegistration);
    }

    clearForm();
    this->hide();
    regWindow->show();
}

///
/// \brief Обработчик возврата из формы регистрации
///
/// Показывает форму авторизации и очищает поля ввода
///
void LoginView::handleBackFromRegistration()
{
    if (regWindow) {
        regWindow->hide();
    }
    this->show();
    clearForm();
}

///
/// \brief Обработчик успешного подключения к серверу
///
void LoginView::onSocketConnected()
{
    qDebug() << "Успешное подключение к серверу";
}

///
/// \brief Обработчик получения данных от сервера
///
/// Анализирует ответ сервера и выполняет:
/// - Успешную авторизацию (переход к главному окну)
/// - Отображение ошибок аутентификации
///
void LoginView::onSocketReadyRead()
{
    QString response = QString::fromUtf8(m_socket->readAll()).trimmed();
    qDebug() << "Ответ сервера:" << response;

    if (response.contains("Authentication successful")) {
        QString username = ui->lineEdit_log->text().trimmed();
        m_socket->setProperty("username", username);

        if (response.contains("admin")) {
            emit authAdminOk();
        } else {
            emit authUserOk();
        }
        emit authSuccess();

        this->hide();
        TaskView *taskView = new TaskView(m_socket);
        taskView->show();
    }
    else if (response.contains("Invalid credentials")) {
        QMessageBox::warning(this, "Ошибка", "Неверные учетные данные");
    }
}