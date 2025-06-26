///
/// \file mainwindow.h
/// \brief Заголовочный файл класса главного окна приложения
/// 

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

///
/// \class MainWindow
/// \brief Класс главного окна приложения
/// \details Предоставляет основное окно интерфейса с функционалом возврата назад
///
class MainWindow : public QMainWindow
{
    Q_OBJECT

signals:
    /// \brief Сигнал возврата к окну авторизации
    void backToLogin();  // Сигнал для возврата

    /// \brief Сигнал запроса возврата
    /// \details Используется для уведомления о необходимости вернуться на предыдущий экран
    void backRequested();

private slots:
    /// \brief Слот обработки нажатия кнопки "Назад"
    void on_pushButton_back_clicked();

public:
    ///
    /// \brief Конструктор главного окна
    /// \param[in] parent Родительский виджет (по умолчанию nullptr)
    ///
    MainWindow(QWidget *parent = nullptr);

    /// \brief Деструктор главного окна
    ~MainWindow();

private:
    /// \brief Указатель на пользовательский интерфейс
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
