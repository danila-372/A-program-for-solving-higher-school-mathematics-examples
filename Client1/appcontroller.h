///
/// \file appcontroller.h
/// \brief Заголовочный файл класса управления приложением
///

#ifndef APPCONTROLLER_H
#define APPCONTROLLER_H

#include <QObject>
#include "LoginView.h"
#include "TaskView.h"
#include "MainWindow.h"
#include <QTcpSocket>
#include "statisticdialog.h"

///
/// \brief Основной контроллер приложения
///
/// Координирует взаимодействие между:
/// - Представлениями (LoginView, TaskView)
/// - Сетевым подключением через TCP-сокет
/// - Главным окном приложения
///
/// \note Реализует основной цикл работы приложения:
/// 1. Аутентификация пользователя
/// 2. Управление задачами
/// 3. Отображение статистики
///
class AppController : public QObject {
    Q_OBJECT
public:
    ///
    /// \brief Конструктор контроллера
    /// \param socket Указатель на TCP-сокет для соединения с сервером
    /// \param parent Родительский QObject
    ///
    explicit AppController(QTcpSocket* socket, QObject *parent = nullptr);
    
    ///
    /// \brief Деструктор контроллера
    ///
    ~AppController();

    ///
    /// \brief Запускает основной цикл приложения
    ///
    /// Реализует:
    /// - Отображение формы входа
    /// - Обработку аутентификации
    /// - Управление интерфейсом приложения
    ///
    void run();

private slots:
    ///
    /// \brief Слот для отображения статистики администратора
    ///
    void showAdminStats();

private:
    QTcpSocket* m_socket;          ///< Указатель на TCP-сокет соединения
    LoginView loginView;           ///< Виджет формы входа
    TaskView taskView;             ///< Виджет работы с задачами
    StatisticDialog* statsDialog;  ///< Диалог статистики (создается при необходимости)
    MainWindow mainView;           ///< Главное окно приложения
};

#endif // APP_CONTROLLER_H
