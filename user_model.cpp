///
/// \file user_model.cpp
/// \brief Реализация класса модели пользователя
///
#include "user_model.h"

 ///
 /// \brief Конструктор модели пользователя
 /// \param username Логин пользователя
 /// \param password Пароль пользователя
 /// \note Сохраняет учетные данные в приватных полях класса
 ///
UserModel::UserModel(const std::string& username, const std::string& password)
    : username(username), password(password) {}

///
/// \brief Проверяет подлинность учетных данных
/// \param username Логин для проверки
/// \param password Пароль для проверки
/// \return true если логин и пароль совпадают с сохраненными
/// \warning В реальной системе пароли должны сравниваться через хеши!
///
bool UserModel::authenticate(const std::string& username, const std::string& password) {
    return this->username == username && this->password == password;
}

///
/// \brief Возвращает логин пользователя
/// \return Строка с именем пользователя
///
std::string UserModel::getUsername() const {
    return username;
}

///
/// \brief Проверяет наличие прав администратора
/// \return true если пользователь является администратором
/// \note Значение по умолчанию - false. Требуется явная установка через setAdmin()
///
bool UserModel::isAdmin() const {
    return isAdminFlag;
}

///
/// \brief Устанавливает/снимает права администратора
/// \param admin Флаг административных прав
//
void UserModel::setAdmin(bool admin) {
    isAdminFlag = admin;
}
