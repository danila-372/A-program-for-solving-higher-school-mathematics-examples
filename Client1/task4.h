///
/// \file task4.h
/// \brief Заголовочный файл класса задач по математическому анализу
///

#ifndef TASK4_H
#define TASK4_H

#include <QWidget>
#include <QTcpSocket>

namespace Ui {
class Task4;
}

///
/// \class Task4
/// \brief Класс для работы с задачами по математическому анализу
/// \details Предоставляет:
/// - Отображение задач и полей для ввода решений
/// - Проверку ответов через сервер
/// - Навигацию между окнами
///
class Task4 : public QWidget
{
    Q_OBJECT

public:
    ///
    /// \brief Конструктор класса
    /// \param[in] socket Указатель на TCP-сокет для связи с сервером
    /// \param[in] parent Родительский виджет (по умолчанию nullptr)
    ///
    explicit Task4(QTcpSocket* socket, QWidget *parent = nullptr);
    
    ///
    /// \brief Деструктор класса
    ///
    ~Task4();

signals:
    /// \brief Сигнал запроса возврата к предыдущему окну
    void backRequested();

private slots:
    /// \brief Слот обработки входящих данных от сервера
    /// \details Принимает и обрабатывает задачи и результаты проверки
    void onSocketReadyRead();
    
    /// \brief Слот обработки нажатия кнопки проверки для задачи 1
    void on_checkButton1_clicked();
    
    /// \brief Слот обработки нажатия кнопки проверки для задачи 2
    void on_checkButton2_clicked();
    
    /// \brief Слот обработки нажатия кнопки проверки для задачи 3
    void on_checkButton3_clicked();
    
    /// \brief Слот обработки нажатия кнопки проверки для задачи 4
    void on_checkButton4_clicked();
    
    /// \brief Слот обработки нажатия кнопки "Назад"
    void on_backButton_clicked();

private:
    ///
    /// \brief Метод проверки ответа по матанализу
    /// \param[in] problemIndex Индекс задачи (0-3)
    /// \param[in] answer Ответ пользователя
    ///
    void checkAnswer(int problemIndex, const QString& answer);

    /// \brief Указатель на пользовательский интерфейс
    Ui::Task4* ui;
    
    /// \brief Указатель на TCP-сокет для связи с сервером
    QTcpSocket* m_socket;
    
    /// \brief Вектор для хранения задач и правильных ответов
    /// \details Формат: QPair<текст задачи, правильный ответ>
    QVector<QPair<QString, QString>> m_problems;
};

#endif // TASK4_H
