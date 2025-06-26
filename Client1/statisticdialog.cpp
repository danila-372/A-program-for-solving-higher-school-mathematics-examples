///
/// \file statisticdialog.cpp
/// \brief Реализация диалога статистики администратора
/// 

#include "statisticdialog.h"
#include "ui_statisticdialog.h"
#include <QDebug>

/// \brief Статическая переменная для хранения единственного экземпляра класса
StatisticDialog* StatisticDialog::m_instance = nullptr;

///
/// \brief Метод для получения экземпляра класса (singleton)
/// \param[in] socket Указатель на TCP-сокет
/// \param[in] parent Родительский виджет
/// \return Указатель на единственный экземпляр класса
/// \note Создает новый экземпляр при первом вызове
///
StatisticDialog* StatisticDialog::instance(QTcpSocket* socket, QWidget* parent)
{
    if (!m_instance) {
        m_instance = new StatisticDialog(socket, parent);
    }
    return m_instance;
}

///
/// \brief Конструктор диалога статистики
/// \param[in] socket Указатель на TCP-сокет для связи с сервером
/// \param[in] parent Родительский виджет
/// \details Инициализирует интерфейс и устанавливает соединения:
/// - Обработчик входящих данных от сервера
/// - Обработчик кнопки обновления статистики
///
StatisticDialog::StatisticDialog(QTcpSocket* socket, QWidget* parent) :
    QDialog(parent),
    ui(new Ui::StatisticDialog),
    m_socket(socket)
{
    ui->setupUi(this);
    setWindowTitle("Admin Statistics");

    if (m_socket) {
        connect(m_socket, &QTcpSocket::readyRead, this, &StatisticDialog::onSocketReadyRead);
    }

    connect(ui->pushButton_refresh, &QPushButton::clicked,
            this, &StatisticDialog::on_pushButton_refresh_clicked);
}

///
/// \brief Деструктор диалога статистики
/// \details Освобождает ресурсы интерфейса и сбрасывает указатель на экземпляр
///
StatisticDialog::~StatisticDialog()
{
    delete ui;
    m_instance = nullptr;
}

///
/// \brief Метод обновления статистики
/// \details Отправляет запрос на сервер для получения актуальной статистики
/// \warning Выводит сообщение об ошибке при отсутствии подключения
///
void StatisticDialog::refreshStats()
{
    if (m_socket && m_socket->state() == QAbstractSocket::ConnectedState) {
        m_socket->write("allstats\r\n");
    } else {
        QMessageBox::warning(this, "Error", "Not connected to server");
    }
}

///
/// \brief Обработчик входящих данных от сервера
/// \details Анализирует ответ сервера и отображает статистику в текстовом поле
/// \note Ожидает данные в формате: "allstats:User:...|Total:...|Correct:...|Last Login:...;;;"
///
void StatisticDialog::onSocketReadyRead()
{
    QString response = QString::fromUtf8(m_socket->readAll()).trimmed();

    if (response.startsWith("allstats:")) {
        // Format: "allstats:User: admin | Total: 0 | Correct: 0 | Last Login: 2025-06-24 12:34:56;;;"
        QString statsData = response.mid(9); // Remove "allstats:"
        QStringList users = statsData.split(";;;", Qt::SkipEmptyParts);

        QString formattedStats;
        for (const QString& user : users) {
            QStringList parts = user.split('|', Qt::SkipEmptyParts);
            if (parts.size() >= 4) {
                formattedStats += QString("User: %1\nTotal: %2\nCorrect: %3\nLast Login: %4\n\n")
                .arg(parts[0].trimmed().section(':', 1).trimmed())
                    .arg(parts[1].trimmed().section(':', 1).trimmed())
                    .arg(parts[2].trimmed().section(':', 1).trimmed())
                    .arg(parts[3].trimmed().section(':', 1).trimmed());
            }
        }

        ui->textEdit_stats->setText(formattedStats);
    }
}

///
/// \brief Обработчик нажатия кнопки обновления
/// \details Инициирует запрос актуальной статистики на сервер
///
void StatisticDialog::on_pushButton_refresh_clicked()
{
    refreshStats();
}