//
/// \file task3.cpp
/// \brief Реализация функционала задач по тригонометрии
/// 

#include "task3.h"
#include "ui_task3.h"
#include <QMessageBox>

///
/// \brief Конструктор класса Task3
/// \param[in] socket Указатель на TCP-сокет для связи с сервером
/// \param[in] parent Родительский виджет
/// \details Инициализирует интерфейс и:
/// 1. Устанавливает соединение для обработки входящих данных
/// 2. Запрашивает тригонометрические задачи с сервера
///
Task3::Task3(QTcpSocket* socket, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Task3),
    m_socket(socket)
{
    ui->setupUi(this);

    // Подключаем сокет
    connect(m_socket, &QTcpSocket::readyRead, this, &Task3::onSocketReadyRead);

    // Запрашиваем задачи с сервера
    m_socket->write("get_problems trigonometry\r\n");
}

///
/// \brief Деструктор класса Task3
/// \details Освобождает ресурсы, занятые пользовательским интерфейсом
///
Task3::~Task3()
{
    delete ui;
}

///
/// \brief Обработчик входящих данных от сервера
/// \details Обрабатывает два типа сообщений:
/// 1. "problems:" - список тригонометрических задач (обновляет интерфейс)
/// 2. "result:" - результат проверки ответа (отображает статус)
///
void Task3::onSocketReadyRead()
{
    QString response = QString::fromUtf8(m_socket->readAll());

    // Обработка ответа с задачами (формат: "problem1|answer1;problem2|answer2;...")
    if (response.startsWith("problems:")) {
        m_problems.clear();
        QStringList problems = response.mid(9).split(";");

        for (int i = 0; i < qMin(4, problems.size()); i++) {
            QStringList parts = problems[i].split("|");
            if (parts.size() == 2) {
                m_problems.append(qMakePair(parts[0], parts[1]));

                // Выводим задачи в соответствующие лейблы
                switch(i) {
                case 0: ui->problemLabel1->setText(parts[0]); break;
                case 1: ui->problemLabel2->setText(parts[0]); break;
                case 2: ui->problemLabel3->setText(parts[0]); break;
                case 3: ui->problemLabel4->setText(parts[0]); break;
                }
            }
        }
    }
    // Обработка ответа о правильности решения
    else if (response.startsWith("result:")) {
        QStringList parts = response.mid(7).split("|");
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
void Task3::on_checkButton1_clicked()
{
    checkAnswer(0, ui->answerLineEdit1->text());
}

///
/// \brief Обработчик нажатия кнопки проверки для задачи 2
///
void Task3::on_checkButton2_clicked()
{
    checkAnswer(1, ui->answerLineEdit2->text());
}

///
/// \brief Обработчик нажатия кнопки проверки для задачи 3
///
void Task3::on_checkButton3_clicked()
{
    checkAnswer(2, ui->answerLineEdit3->text());
}

///
/// \brief Обработчик нажатия кнопки проверки для задачи 4
///
void Task3::on_checkButton4_clicked()
{
    checkAnswer(3, ui->answerLineEdit4->text());
}

///
/// \brief Метод проверки ответа
/// \param[in] problemIndex Индекс задачи (0-3)
/// \param[in] answer Ответ пользователя
/// \details Отправляет тригонометрический ответ на сервер для проверки
///
void Task3::checkAnswer(int problemIndex, const QString& answer)
{
    if (problemIndex >= 0 && problemIndex < m_problems.size()) {
        QString command = QString("check_answer %1 %2 %3\r\n")
        .arg("trigonometry")
            .arg(problemIndex)
            .arg(answer);
        m_socket->write(command.toUtf8());
    }
}

///
/// \brief Обработчик нажатия кнопки "Назад"
/// \details Закрывает текущее окно и генерирует сигнал backRequested()
///
void Task3::on_backButton_clicked()
{
    this->close();
    emit backRequested();
}