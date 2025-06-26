///
/// \file loginview.h
/// \brief Заголовочный файл класса формы авторизации
///

#ifndef LOGINVIEW_H
#define LOGINVIEW_H

#include <QWidget>
#include <QTcpSocket>

namespace Ui {
class LoginView;
}

class RegView; // Предварительное объявление

///
/// \brief Класс формы авторизации
///
/// Предоставляет функционал:
/// - Аутентификация пользователей
/// - Переход к форме регистрации
/// - Взаимодействие с сервером через TCP-сокет
///
class LoginView : public QWidget
{
    Q_OBJECT

public:
    ///
    /// \brief Конструктор формы авторизации
    /// \param socket Указатель на TCP-сокет для соединения с сервером
    /// \param parent Родительский виджет
    ///
    explicit LoginView(QTcpSocket* socket, QWidget *parent = nullptr);
    
    ///
    /// \brief Деструктор формы авторизации
    ///
    ~LoginView();

signals:
    ///
    /// \brief Сигнал успешной аутентификации
    ///
    void authSuccess();

    ///
    /// \brief Сигнал успешной аутентификации обычного пользователя
    ///
    void authUserOk();

    ///
    /// \brief Сигнал успешной аутентификации администратора
    ///
    void authAdminOk();

    ///
    /// \brief Сигнал запроса перехода к регистрации
    ///
    void registrationRequested();

private slots:
    ///
    /// \brief Слот обработки нажатия кнопки авторизации
    ///
    void on_pushButton_auth_clicked();

    ///
    /// \brief Слот обработки нажатия кнопки регистрации
    ///
    void on_pushButton_reg_clicked();

    ///
    /// \brief Слот обработки возврата из формы регистрации
    ///
    void handleBackFromRegistration();

    ///
    /// \brief Слот обработки успешного подключения к серверу
    ///
    void onSocketConnected();

    ///
    /// \brief Слот обработки входящих данных от сервера
    ///
    void onSocketReadyRead();

private:
    ///
    /// \brief Очистка полей формы
    ///
    void clearForm();

    Ui::LoginView *ui;        ///< Указатель на UI-форму
    QTcpSocket* m_socket;     ///< Указатель на TCP-сокет соединения
    RegView* regWindow;       ///< Указатель на форму регистрации
};

#endif // LOGINVIEW_H
