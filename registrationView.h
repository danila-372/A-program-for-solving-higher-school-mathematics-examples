///
/// \file registrationView.h
/// \brief Заголовочный файл класса для взаимодействия с экраном регистрации
///
#ifndef REGISTRATION_VIEW_H
#define REGISTRATION_VIEW_H

#include <string>
#include <iostream>

  ///
  /// \brief Класс для работы с интерфейсом регистрации
  /// \details Отвечает за:
  /// - Отображение экрана регистрации
  /// - Получение нового логина и пароля
  /// - Передачу данных в контроллер для создания учетной записи
  ///
class RegistrationView {
public:

     ///
     /// \brief Отображает экран регистрации
     /// \details Выводит шапку с названием "=== Registration Screen ==="
     ///
    void display();

     ///
     /// \brief Запрашивает новое имя пользователя
     /// \return Введенное имя пользователя
     /// \note Использует стандартный ввод (std::cin)
     /// \warning Логин должен быть уникальным в системе
     ///
    std::string getNewUsername();

    ///
    /// \brief Запрашивает новый пароль пользователя
    /// \return Введенный пароль
    /// \warning Пароль передается в открытом виде!
    /// \note Для реальных систем требуется:
    /// - Шифрование при передаче
    /// - Валидация сложности пароля
    ///
    std::string getNewPassword();
};

#endif // REGISTRATION_VIEW_H
