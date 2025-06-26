///
/// \file taskview.cpp
/// \brief Реализация класса представления задач
/// 

#include "taskview.h"
#include "ui_taskview.h"
#include "task1.h"
#include "task2.h"
#include "task3.h"
#include "task4.h"

///
/// \brief Конструктор класса TaskView
/// \param[in] clientSocket Указатель на TCP-сокет для связи с сервером
/// \param[in] parent Родительский виджет
/// \details Инициализирует интерфейс и:
/// 1. Создает окна для каждого типа задач
/// 2. Устанавливает соединения для кнопок навигации
///
TaskView::TaskView(QTcpSocket* clientSocket, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TaskView),
    m_socket(clientSocket),
    m_task1Window(nullptr),
    m_task2Window(nullptr),
    m_task3Window(nullptr),
    m_task4Window(nullptr)
{
    ui->setupUi(this);

    // Инициализация окон с передачей сокета
    m_task1Window = new Task1(m_socket);
    m_task2Window = new Task2(m_socket);
    m_task3Window = new Task3(m_socket);
    m_task4Window = new Task4(m_socket);

    // Подключение кнопок
    connect(ui->pushButton_task1, &QPushButton::clicked, this, &TaskView::on_pushButton_task1_clicked);
    connect(ui->pushButton_task2, &QPushButton::clicked, this, &TaskView::on_pushButton_task2_clicked);
    connect(ui->pushButton_task3, &QPushButton::clicked, this, &TaskView::on_pushButton_task3_clicked);
    connect(ui->pushButton_task4, &QPushButton::clicked, this, &TaskView::on_pushButton_task4_clicked);
}

///
/// \brief Деструктор класса TaskView
/// \details Освобождает ресурсы, занятые пользовательским интерфейсом
///
TaskView::~TaskView()
{
    delete ui;
}

///
/// \brief Обработчик нажатия кнопки "Алгебра"
/// \details Создает и отображает окно с задачами по алгебре
///
void TaskView::on_pushButton_task1_clicked()
{
    Task1 *task1 = new Task1(m_socket);
    connect(task1, &Task1::backRequested, this, &TaskView::show);
    this->hide();
    task1->show();
}

///
/// \brief Обработчик нажатия кнопки "Геометрия"
/// \details Создает и отображает окно с задачами по геометрии
///
void TaskView::on_pushButton_task2_clicked()
{
    Task2 *task2 = new Task2(m_socket);
    connect(task2, &Task2::backRequested, this, &TaskView::show);
    this->hide();
    task2->show();
}

///
/// \brief Обработчик нажатия кнопки "Тригонометрия"
/// \details Создает и отображает окно с тригонометрическими задачами
///
void TaskView::on_pushButton_task3_clicked()
{
    Task3 *task3 = new Task3(m_socket);
    connect(task3, &Task3::backRequested, this, &TaskView::show);
    this->hide();
    task3->show();
}

///
/// \brief Обработчик нажатия кнопки "Матанализ"
/// \details Создает и отображает окно с задачами по матанализу
///
void TaskView::on_pushButton_task4_clicked()
{
    Task4 *task4 = new Task4(m_socket);
    connect(task4, &Task4::backRequested, this, &TaskView::show);
    this->hide();
    task4->show();
}

///
/// \brief Обработчик ответов от сервера
/// \note В текущей реализации не содержит функционала
///
void TaskView::onServerResponse()
{

}
