///
/// \file user_model.h
/// \brief Заголовочный файл класса модели пользователя
///
#ifndef USER_MODEL_H
#define USER_MODEL_H

#include <string>

 ///
 /// \brief Класс для хранения данных пользователя и управления правами доступа
 /// \details Содержит:
 /// - Логин и пароль пользователя
 /// - Флаг административных прав
 /// - Методы для аутентификации и управления правами
 ///
class UserModel {
public:
    ///
    /// \brief Конструктор модели пользователя
    /// \param username Логин пользователя
    /// \param password Пароль пользователя
    ///
    UserModel(const std::string& username, const std::string& password);
    
    ///
    /// \brief Проверяет подлинность учетных данных
    /// \param username Логин для проверки
    /// \param password Пароль для проверки
    /// \return true если логин и пароль совпадают
    /// \warning В реальной системе должно использоваться хеширование паролей!
    ///
    bool authenticate(const std::string& username, const std::string& password);
    
    ///
    /// \brief Возвращает логин пользователя
    /// \return Строка с именем пользователя
    ///
    std::string getUsername() const;
    
    /**
     * \brief Проверяет наличие прав администратора
     * \return true если пользователь имеет права администратора
     * \note По умолчанию значение false. Требуется явная установка через setAdmin()
     */
    bool isAdmin() const;
    
    ///
    /// \brief Устанавливает права администратора
    /// \param admin Флаг административных прав (true/false)
    ///
    void setAdmin(bool admin);

private:
    std::string username;   ///< Логин пользователя ///
    std::string password;   ///< Пароль пользователя (в открытом виде - только для примера!) ///
    bool isAdminFlag = false;   ///< Флаг прав администратора ///
};

#endif // USER_MODEL_H
