///
/// \file regview.h
/// \brief Заголовочный файл класса окна регистрации
/// 

#ifndef REGVIEW_H
#define REGVIEW_H

#include <QWidget>
#include <QTcpSocket>  // Добавляем для работы с сокетом
#include <QMessageBox> // Добавляем для QMessageBox

namespace Ui {
class RegView;
}

///
/// \class RegView
/// \brief Класс окна регистрации пользователя
/// \details Предоставляет:
/// - Форму ввода логина/пароля
/// - Взаимодействие с сервером через TCP-сокет
/// - Навигацию между окнами приложения
///
class RegView : public QWidget
{
    Q_OBJECT
public:
    ///
    /// \brief Конструктор окна регистрации
    /// \param[in] socket Указатель на TCP-сокет для соединения с сервером
    /// \param[in] parent Родительский виджет (по умолчанию nullptr)
    ///
    explicit RegView(QTcpSocket* socket, QWidget *parent = nullptr); // Изменяем конструктор
    
    /// \brief Деструктор окна регистрации
    ~RegView();

signals:
    /// \brief Сигнал запроса возврата к предыдущему окну
    void backRequested();

private slots:
    /// \brief Слот обработки нажатия кнопки "Назад"
    void on_pushButton_back_clicked();
    
    /// \brief Слот обработки нажатия кнопки регистрации
    /// \note Отправляет данные на сервер при наличии подключения
    void on_pushButton_register_clicked(); // Добавляем объявление слота

private:
    /// \brief Указатель на пользовательский интерфейс
    Ui::RegView *ui;
    
    /// \brief Указатель на TCP-сокет для связи с сервером
    QTcpSocket* m_socket; // Добавляем член для хранения сокета
};

#endif // REGVIEW_H
