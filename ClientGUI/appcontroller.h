///
/// \file appcontroller.h
/// \brief Заголовочный файл класса управления приложением
///
#ifndef APP_CONTROLLER_H
#define APP_CONTROLLER_H

#include "LoginView.h"
#include "TaskView.h"
//#include "user_model.h"
#include "mainwindow.h"
#include "connection.h"
#include <iostream>
#include <QObject>

 ///
 /// \brief Основной контроллер приложения
 /// \details Координирует взаимодействие между:
 /// - Представлениями (LoginView, TaskView)
 /// - Моделью пользователя (UserModel)
 /// - Сетевым подключением (ServerConnection)
 /// \note Реализует основной цикл работы приложения:
 /// 1. Аутентификация
 /// 2. Работа с задачами
 /// 3. Сетевое взаимодействие
 ///
class AppController : public QObject {
    Q_OBJECT
public:

    ///
    /// \brief Конструктор контроллера
    /// \details Инициализирует:
    /// - Ссылку на подключение к серверу через ServerConnection::getInstance()
    /// - Указатель на модель пользователя как nullptr
    ////
    AppController();

    ///
    /// \brief Запускает основной цикл приложения
    /// \details Реализует:
    /// - Отображение форм ввода
    /// - Аутентификацию пользователя
    /// - Взаимодействие с сервером
    /// - Управление жизненным циклом модели пользователя
    ///
    void run();

private:
    LoginView loginView;        ///< Форма для ввода логина/пароля ///
    TaskView taskView;              ///< Интерфейс работы с задачами ///
    //UserModel* userModel;           ///< Модель данных пользователя ///
    MainWindow mainView; ///< Сетевое подключение (Singleton) ///
};

#endif // APP_CONTROLLER_H
