//
/// \file task3.h
/// \brief Заголовочный файл класса задач по тригонометрии
/// 

#ifndef TASK3_H
#define TASK3_H

#include <QWidget>
#include <QTcpSocket>

namespace Ui {
class Task3;
}

///
/// \class Task3
/// \brief Класс для работы с тригонометрическими задачами
/// \details Предоставляет:
/// - Отображение тригонометрических задач и полей для ответов
/// - Проверку ответов через сервер
/// - Навигацию между окнами
///
class Task3 : public QWidget
{
    Q_OBJECT

public:
    ///
    /// \brief Конструктор класса
    /// \param[in] socket Указатель на TCP-сокет для связи с сервером
    /// \param[in] parent Родительский виджет (по умолчанию nullptr)
    ///
    explicit Task3(QTcpSocket* socket, QWidget* parent = nullptr);
    
    ///
    /// \brief Деструктор класса
    ///
    ~Task3();

signals:
    /// \brief Сигнал запроса возврата к предыдущему окну
    void backRequested();

private slots:
    /// \brief Слот обработки входящих данных от сервера
    /// \details Принимает и обрабатывает тригонометрические задачи и результаты проверки
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
    /// \brief Метод проверки тригонометрического ответа
    /// \param[in] problemIndex Индекс задачи (0-3)
    /// \param[in] answer Ответ пользователя
    ///
    void checkAnswer(int problemIndex, const QString& answer);

    /// \brief Указатель на пользовательский интерфейс
    Ui::Task3* ui;
    
    /// \brief Указатель на TCP-сокет для связи с сервером
    QTcpSocket* m_socket;
    
    /// \brief Вектор для хранения тригонометрических задач и ответов
    /// \details Формат: QPair<текст задачи, правильный ответ>
    QVector<QPair<QString, QString>> m_problems;
};

#endif // TASK3_H