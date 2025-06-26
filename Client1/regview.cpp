///
/// \file regview.cpp
/// \brief Реализация класса окна регистрации
///

#include "regview.h"
#include "ui_regview.h"
#include <QMessageBox> // Добавляем для QMessageBox

///
/// \brief Конструктор окна регистрации
/// \param[in] socket Указатель на TCP-сокет для связи с сервером
/// \param[in] parent Родительский виджет
/// \details Инициализирует интерфейс и устанавливает соединения сигналов:
/// - Кнопка возврата
/// - Кнопка регистрации
///
RegView::RegView(QTcpSocket* socket, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RegView),
    m_socket(socket) // Инициализируем сокет
{
    ui->setupUi(this);
    connect(ui->pushButton_back, &QPushButton::clicked,
            this, &RegView::on_pushButton_back_clicked);
    connect(ui->pushButton_register, &QPushButton::clicked, // Добавляем соединение для кнопки регистрации
            this, &RegView::on_pushButton_register_clicked);
}

///
/// \brief Обработчик нажатия кнопки "Назад"
/// \details:
/// 1. Генерирует сигнал backRequested() для возврата к предыдущему окну
/// 2. Закрывает текущее окно
///
void RegView::on_pushButton_back_clicked()
{
    emit backRequested();
    this->close();
}

///
/// \brief Обработчик нажатия кнопки регистрации
/// \details Выполняет:
/// 1. Проверку заполнения полей логина и пароля
/// 2. Отправку данных на сервер при наличии подключения
/// 3. Вывод сообщений об ошибках при проблемах:
///    - Пустые поля
///    - Отсутствие подключения к серверу
///
void RegView::on_pushButton_register_clicked()
{
    QString login = ui->lineEdit_login->text();
    QString password = ui->lineEdit_password->text();

    if (login.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Логин и пароль не могут быть пустыми!");
        return;
    }

    if (m_socket && m_socket->state() == QAbstractSocket::ConnectedState) {
        m_socket->write(QString("register %1 %2\r\n").arg(login).arg(password).toUtf8());
    } else {
        QMessageBox::warning(this, "Ошибка", "Нет подключения к серверу!");
    }
}

///
/// \brief Деструктор окна регистрации
/// \details Освобождает ресурсы, занятые пользовательским интерфейсом
///
RegView::~RegView()
{
    delete ui;
}
