///
/// \file RegistrationView.cpp
/// \brief Реализация класса для отображения экрана регистрации
///
#include "RegistrationView.h"

 ///
  /// \brief Отображает экран регистрации нового пользователя
  /// \details Выводит заголовок "=== Registration Screen ==="
  /// для визуального оформления интерфейса регистрации
  ///
void RegistrationView::display() {
    std::cout << "=== Registration Screen ===" << std::endl;
}

///
 /// \brief Получает новое имя пользователя
 /// \return Строка с введенным именем пользователя
 /// \note Запрашивает ввод через стандартный поток ввода
 /// \warning Имя пользователя должно быть уникальным
 ///
std::string RegistrationView::getNewUsername() {
    std::string username;
    std::cout << "Enter new username: ";
    std::cin >> username;
    return username;
}

///
/// \brief Получает новый пароль пользователя
/// \return Строка с введенным паролем
/// \note Запрашивает ввод через стандартный поток ввода
/// \warning Пароль хранится/передается в открытом виде!
/// \note Для реальных систем рекомендуется:
/// - Валидация сложности пароля
/// - Шифрование при передаче
///
std::string RegistrationView::getNewPassword() {
    std::string password;
    std::cout << "Enter new password: ";
    std::cin >> password;
    return password;
}
