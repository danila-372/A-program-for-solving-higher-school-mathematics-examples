///
/// \file loginView.cpp
/// \brief Реализация класса для отображения экрана авторизации
///
#include "LoginView.h"

 ///
 /// \brief Отображает экран входа в систему
 /// \details Выводит заголовок "=== Login Screen ==="
 /// для визуального оформления интерфейса авторизации
 ///
void LoginView::display() {
    std::cout << "=== Login Screen ===" << std::endl;
}

///
/// \@brief Получает логин пользователя
/// \return Строка с введенным именем пользователя
/// \note Запрашивает ввод через стандартный поток ввода
///
std::string LoginView::getUsername() {
    std::string username;
    std::cout << "Enter username: ";
    std::cin >> username;
    return username;
}

///
/// \brief Получает пароль пользователя
/// \return Строка с введенным паролем
/// \note Запрашивает ввод через стандартный поток ввода
/// \warning Пароль отображается в открытом виде.
/// Для реальных систем рекомендуется использовать:
/// - Шифрование ввода
/// - Скрытие символов
///
std::string LoginView::getPassword() {
    std::string password;
    std::cout << "Enter password: ";
    std::cin >> password;
    return password;
}
