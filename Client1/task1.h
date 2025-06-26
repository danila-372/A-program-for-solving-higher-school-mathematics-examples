///
/// \file task1.h
/// \brief Заголовочный файл класса задач по алгебре
/// 

#ifndef TASK1_H
#define TASK1_H

#include <QWidget>
#include <QTcpSocket>
#include <QVector>
#include <QPair>

namespace Ui {
class Task1;
}

///
/// \class Task1
/// \brief Класс для работы с задачами по алгебре
/// \details Предоставляет:
/// - Отображение задач и полей для ответов
/// - Проверку ответов через сервер
/// - Навигацию между окнами
///
class Task1 : public QWidget
{
    Q_OBJECT

public:
    ///
    /// \brief Конструктор класса
    /// \param[in] socket Указатель на TCP-сокет для связи с сервером
    /// \param[in] parent Родительский виджет (по умолчанию nullptr)
    ///
    explicit Task1(QTcpSocket* socket, QWidget* parent = nullptr);
    
    ///
    /// \brief Деструктор класса
    ///
    ~Task1() override;

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
    
    ///
    /// \brief Метод обработки сообщения от сервера
    /// \param[in] response Полученное сообщение
    ///
    void processMessage(const QString& response);

    /// \brief Указатель на пользовательский интерфейс
    Ui::Task1* ui;
    
    /// \brief Указатель на TCP-сокет для связи с сервером
    QTcpSocket* m_socket;
    
    /// \brief Список текущих задач и их решений
    QVector<QPair<QString, QString>> m_problems;
    
    /// \brief Буфер для хранения неполных сообщений
    QString m_buffer;
};

#endif // TASK1_H
