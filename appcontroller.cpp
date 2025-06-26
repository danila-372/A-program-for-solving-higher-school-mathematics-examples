///
/// \file appcontroller.cpp
/// \brief Реализация класса управления приложением
///

#include "AppController.h"

///
/// \brief Конструктор контроллера приложения
/// \param socket Указатель на TCP-сокет для соединения с сервером
/// \param parent Родительский QObject
/// 
/// Инициализирует:
/// - Указатель на сокет соединения
/// - Виджеты интерфейса (логин, задачи, статистика)
/// - Соединяет сигналы успешной авторизации с соответствующими слотами
///
AppController::AppController(QTcpSocket* socket, QObject *parent) :
    QObject(parent),
    m_socket(socket),
    loginView(m_socket),
    taskView(m_socket, nullptr),
    statsDialog(nullptr)
{
    connect(&loginView, &LoginView::authSuccess, &mainView, &MainWindow::show);
    connect(&loginView, &LoginView::authAdminOk, this, &AppController::showAdminStats);
}

///
/// \brief Деструктор контроллера приложения
///
AppController::~AppController()
{
    // Деструктор
}

///
/// \brief Основной цикл работы приложения
/// 
/// Выполняет:
/// 1. Отображение формы входа
/// 2. Ожидание успешной аутентификации
/// 
/// \note Закомментированный код содержит логику:
/// - Подключения к серверу
/// - Цикла аутентификации
/// - Отображения меню администратора
/// - Обработки запросов статистики
///
void AppController::run() {
    loginView.show();
    /* Пример закомментированной логики работы:
    serverConnection.connect();

    while (true) {
        loginView.display();
        std::string username = loginView.getUsername();
        std::string password = loginView.getPassword();

        userModel = new UserModel(username, password);
        if (userModel->authenticate(username, password)) {
            // Логика после успешного входа
            break;
        }
    }
    serverConnection.disconnect();
    delete userModel; */
}

///
/// \brief Отображение статистики администратора
/// 
/// Создает и показывает диалог статистики, если он еще не создан,
/// затем обновляет и отображает статистические данные
///
void AppController::showAdminStats()
{
    if (!statsDialog) {
        statsDialog = StatisticDialog::instance(m_socket);
    }
    statsDialog->refreshStats();
    statsDialog->show();
}
