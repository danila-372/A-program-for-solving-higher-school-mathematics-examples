///
/// \file mainwindow.cpp
/// \brief Заголовочный файл конструктор главного окна приложения
///
#include "mainwindow.h"
#include "ui_mainwindow.h"

///
/// \brief Конструктор главного окна
/// \param[in] parent Родительский виджет (может быть nullptr)
/// \details Инициализирует пользовательский интерфейс главного окна
///
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

///
/// \brief Обработчик нажатия кнопки "Назад"
/// \details Выполняет следующие действия:
/// 1. Выводит отладочное сообщение в консоль
/// 2. Закрывает текущее окно (или скрывает его)
/// 3. Генерирует сигнал backRequested() для обработки в родительском виджете
/// \note Реализация может изменяться в зависимости от логики приложения
///
void MainWindow::on_pushButton_back_clicked()
{
    qDebug() << "Back button clicked";  // Для отладки
    this->close();  // Или другая логика
    emit backRequested();
    this->hide();    // Если используете сигналы
}

///
/// \brief Деструктор главного окна
/// \details Освобождает ресурсы, занятые пользовательским интерфейсом
///
MainWindow::~MainWindow()
{
    delete ui;
}
