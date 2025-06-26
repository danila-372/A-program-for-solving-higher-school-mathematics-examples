///
/// \file task1.cpp
/// \brief Реализация функционала задач по алгебре
/// 

#include "task1.h"
#include "ui_task1.h"
#include <QMessageBox>
#include <QDebug>

///
/// \brief Конструктор класса Task1
/// \param[in] socket Указатель на TCP-сокет для связи с сервером
/// \param[in] parent Родительский виджет
/// \details Инициализирует интерфейс и:
/// 1. Устанавливает соединение для обработки входящих данных
/// 2. Запрашивает задачи по алгебре с сервера
///
Task1::Task1(QTcpSocket* socket, QWidget *parent)
    : QWidget(parent),
    ui(new Ui::Task1),
    m_socket(socket)
{
    ui->setupUi(this);

    connect(m_socket, &QTcpSocket::readyRead, this, &Task1::onSocketReadyRead);

    // Запрашиваем задачи с сервера
    m_socket->write("get_problems algebra\r\n");
}

///
/// \brief Деструктор класса Task1
/// \details Освобождает ресурсы, занятые пользовательским интерфейсом
///
Task1::~Task1()
{
    delete ui;
}

///
/// \brief Обработчик входящих данных от сервера
/// \details Выполняет:
/// 1. Чтение данных из сокета
/// 2. Разделение на отдельные сообщения
/// 3. Обработку каждого сообщения
/// 4. Сохранение неполных сообщений в буфер
///
void Task1::onSocketReadyRead()
{
    QByteArray data = m_socket->readAll();
    qDebug() << "Received data:" << data;

    m_buffer += QString::fromUtf8(data);

    QStringList messages = m_buffer.split("\r\n", Qt::SkipEmptyParts);

    // Обрабатываем все сообщения, кроме последнего (возможно неполного)
    for (int i = 0; i < messages.size() - 1; ++i) {
        processMessage(messages[i]);
    }

    // Последнее сообщение сохраняем в буфер
    m_buffer = messages.isEmpty() ? "" : messages.last();
}

///
/// \brief Обработчик сообщений от сервера
/// \param[in] response Полученное сообщение от сервера
/// \details Обрабатывает два типа сообщений:
/// 1. "problems:" - список задач (обновляет интерфейс)
/// 2. "result:" - результат проверки ответа (отображает статус)
///
void Task1::processMessage(const QString& response)
{
    qDebug() << "Processing message:" << response;

    if (response.startsWith("problems:")) {
        m_problems.clear();
        QStringList problems = response.mid(QString("problems:").length()).split(";");

        for (int i = 0; i < qMin(4, problems.size()); i++) {
            QStringList parts = problems[i].split("|");
            if (parts.size() == 2) {
                m_problems.append(qMakePair(parts[0], parts[1]));

                switch(i) {
                case 0: ui->problemLabel1->setText(parts[0]); break;
                case 1: ui->problemLabel2->setText(parts[0]); break;
                case 2: ui->problemLabel3->setText(parts[0]); break;
                case 3: ui->problemLabel4->setText(parts[0]); break;
                }
            }
        }
    }
    else if (response.startsWith("result:")) {
        QStringList parts = response.mid(QString("result:").length()).split("|");
        if (parts.size() == 2) {
            int taskNum = parts[0].toInt();
            bool isCorrect = parts[1] == "1";

            QLabel* resultLabel = nullptr;
            switch(taskNum) {
            case 0: resultLabel = ui->resultLabel1; break;
            case 1: resultLabel = ui->resultLabel2; break;
            case 2: resultLabel = ui->resultLabel3; break;
            case 3: resultLabel = ui->resultLabel4; break;
            }

            if (resultLabel) {
                resultLabel->setText(isCorrect ? "✓ Верно" : "✗ Неверно");
                resultLabel->setStyleSheet(isCorrect ? "color: green;" : "color: red;");
            }
        }
    }
}

///
/// \brief Обработчик нажатия кнопки проверки для задачи 1
///
void Task1::on_checkButton1_clicked()
{
    checkAnswer(0, ui->answerLineEdit1->text());
}

///
/// \brief Обработчик нажатия кнопки проверки для задачи 2
///
void Task1::on_checkButton2_clicked()
{
    checkAnswer(1, ui->answerLineEdit2->text());
}

///
/// \brief Обработчик нажатия кнопки проверки для задачи 3
///
void Task1::on_checkButton3_clicked()
{
    checkAnswer(2, ui->answerLineEdit3->text());
}

///
/// \brief Обработчик нажатия кнопки проверки для задачи 4
///
void Task1::on_checkButton4_clicked()
{
    checkAnswer(3, ui->answerLineEdit4->text());
}

///
/// \brief Метод проверки ответа
/// \param[in] problemIndex Индекс задачи (0-3)
/// \param[in] answer Ответ пользователя
/// \details Отправляет ответ на сервер для проверки
///
void Task1::checkAnswer(int problemIndex, const QString& answer)
{
    if (problemIndex >= 0 && problemIndex < m_problems.size()) {
        QString command = QString("check_answer algebra %1 %2\r\n")
        .arg(problemIndex)
            .arg(answer);
        m_socket->write(command.toUtf8());
    }
}

///
/// \brief Обработчик нажатия кнопки "Назад"
/// \details Закрывает текущее окно и генерирует сигнал backRequested()
///
void Task1::on_backButton_clicked()
{
    this->close();
    emit backRequested();
}
