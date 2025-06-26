///
/// \file taskview.h
/// \brief Заголовочный файл класса представления задач
/// 

#ifndef TASKVIEW_H
#define TASKVIEW_H

#include <QWidget>
#include <QTcpSocket>

// Предварительные объявления классов задач
class Task1;
class Task2;
class Task3;
class Task4;
//class Task5;

namespace Ui {
class TaskView;
}

///
/// \class TaskView
/// \brief Класс главного окна навигации по задачам
/// \details Предоставляет:
/// - Интерфейс выбора типа задач (алгебра, геометрия и др.)
/// - Управление подключением к серверу
/// - Навигацию между разделами
///
class TaskView : public QWidget
{
    Q_OBJECT

public:
    ///
    /// \brief Конструктор класса TaskView
    /// \param[in] socket Указатель на TCP-сокет для связи с сервером
    /// \param[in] parent Родительский виджет (по умолчанию nullptr)
    ///
    explicit TaskView(QTcpSocket* socket, QWidget *parent = nullptr);
    
    ///
    /// \brief Деструктор класса TaskView
    ///
    ~TaskView();

private slots:
    /// \brief Слот обработки нажатия кнопки "Алгебра"
    void on_pushButton_task1_clicked();
    
    /// \brief Слот обработки нажатия кнопки "Геометрия"
    void on_pushButton_task2_clicked();
    
    /// \brief Слот обработки нажатия кнопки "Тригонометрия"
    void on_pushButton_task3_clicked();
    
    /// \brief Слот обработки нажатия кнопки "Матанализ"
    void on_pushButton_task4_clicked();
    
    // void on_pushButton_task5_clicked();
    
    /// \brief Слот обработки ответов сервера
    /// \note В текущей реализации не содержит функционала
    void onServerResponse();

private:
    /// \brief Указатель на пользовательский интерфейс
    Ui::TaskView *ui;
    
    /// \brief Указатель на TCP-сокет для связи с сервером
    QTcpSocket* m_socket;
    
    /// \brief Указатель на окно задач по алгебре
    Task1* m_task1Window;
    
    /// \brief Указатель на окно задач по геометрии
    Task2* m_task2Window;
    
    /// \brief Указатель на окно тригонометрических задач
    Task3* m_task3Window;
    
    /// \brief Указатель на окно задач по матанализу
    Task4* m_task4Window;
    
    // Task5* m_task5Window;
};

#endif // TASKVIEW_H
