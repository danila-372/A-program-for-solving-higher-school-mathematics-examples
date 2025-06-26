///
/// \file task2.h
/// \brief Заголовочный файл класса задач по геометрии
/// 

#ifndef TASK2_H
#define TASK2_H

#include <QWidget>
#include <QTcpSocket>

namespace Ui {
class Task2;
}

///
/// \class Task2
/// \brief Класс для работы с задачами по геометрии
/// \details Предоставляет:
/// - Отображение задач и полей для ответов
/// - Проверку ответов через сервер
/// - Навигацию между окнами
///
class Task2 : public QWidget
{
    Q_OBJECT

public:
    ///
    /// \brief Конструктор класса
    /// \param[in] socket Указатель на TCP-сокет для связи с сервером
    /// \param[in] parent Родительский виджет (по умолчанию nullptr)
    ///
    explicit Task2(QTcpSocket* socket, QWidget *parent = nullptr);
    
    ///
    /// \brief Деструктор класса
    ///
    ~Task2();

signals:
    /// \brief Сигнал запроса возврата к предыдущему окну
    void backRequested();

private slots:
    /// \brief Слот обработки входящих данных от сервера
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
    /// \brief Метод проверки ответа
    /// \param[in] problemIndex Индекс задачи (0-3)
    /// \param[in] answer Ответ пользователя
    ///
    void checkAnswer(int problemIndex, const QString& answer);

    /// \brief Указатель на пользовательский интерфейс
    Ui::Task2 *ui;
    
    /// \brief Указатель на TCP-сокет для связи с сервером
    QTcpSocket* m_socket;
    
    /// \brief Вектор для хранения задач и правильных ответов
    /// \details Формат: QPair<текст задачи, правильный ответ>
    QVector<QPair<QString, QString>> m_problems;
};

#endif // TASK2_H
