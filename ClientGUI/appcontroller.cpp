///
/// \file appcontroller.cpp
/// \brief Реализация класса управления приложением
///
#include "AppController.h"

///
/// \brief Конструктор контроллера приложения
/// \details Инициализирует :
/// -Указатель на модель пользователя(userModel) как nullptr
/// -Подключение к серверу через Singleton ServerConnection
///
AppController::AppController()
{
    loginView.show();
    connect(&loginView,&LoginView::auth_user_ok,&mainView,&MainWindow::show);
}

///
/// \brief Основной цикл работы приложения
/// \details Выполняет:
/// 1. Подключение к серверу
/// 2. Цикл аутентификации:
///    - Отображение формы ввода логина/пароля
///    - Проверка учетных данных
/// 3. После успешной аутентификации:
///    - Отображение меню администратора
///    - Обработка выбора пунктов меню
/// 4. При выходе - отключение от сервера
///
/// \note Для администраторского меню:
/// - Пункт 4: запрос статистики пользователей
///
void AppController::run() {
/*    serverConnection.connect();

    while (true) {
        loginView.display();
        std::string username = loginView.getUsername();
        std::string password = loginView.getPassword();

        // Попробуем аутентифицировать пользователя
        userModel = new UserModel(username, password);
        if (userModel->authenticate(username, password)) {
            std::cout << "Login successful!" << std::endl;
            serverConnection.sendMessage("User  logged in: " + username);
            taskView.display();
            taskView.showAdminMenu();
            while(true) {
                taskView.showAdminMenu();
                int choice;
                std::cin >> choice;

                 ///
                 /// \brief Обработка запроса статистики
                 /// \note При выборе пункта 4:
                 /// - Отправка команды "userstats" на сервер
                 /// - Получение и отображение статистики
                 ///
                if(choice == 4) { // Обработка выбора статистики
                    serverConnection.sendMessage("userstats");
                    std::string stats = serverConnection.receiveMessage();
                    taskView.showUserStats(stats);
                }
            }
            // Здесь можно добавить логику выбора задачи
            break; // Выход из цикла после успешного входа
        } else {
            std::cout << "Invalid credentials. Please try again." << std::endl;
        }
    }

    serverConnection.disconnect();
    delete userModel; // Освобождаем память*/
}

