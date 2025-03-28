///
/// \file TaskView.cpp
/// \brief Реализация класса для отображения интерфейса работы с задачами
///
#include "TaskView.h"

 ///
 /// \brief Отображает экран задач
 /// \details Выводит заголовок "=== Task Screen ==="
 /// для визуального оформления интерфейса управления задачами
 ///
void TaskView::display() {
    std::cout << "=== Task Screen ===" << std::endl;
}

///
/// \brief Отображает меню администратора
/// \details Пункты меню:
/// 1. Просмотр задач
/// 2. Добавление задачи
/// 3. Удаление задачи
/// 4. Просмотр статистики пользователей (новый функционал)
///
void TaskView::showAdminMenu() {
    std::cout << "Admin Menu" << std::endl;
    std::cout << "1. View Tasks" << std::endl;
    std::cout << "2. Add Task" << std::endl;
    std::cout << "3. Remove Task" << std::endl;
    std::cout << "4. View User Statistics" << std::endl; // Новый пункт
}

///
/// \brief Запрашивает выбор задачи у пользователя
/// \return Название выбранной задачи
/// \note Использует стандартный ввод (std::cin)
///
std::string TaskView::selectTask() {
    std::string task;
    std::cout << "Select a task: ";
    std::cin >> task;
    return task;
}

///
/// \brief Отображает статистику пользователей
/// \param stats Строка с данными статистики
/// \details Форматирует вывод в виде:
/// \code
/// === User Statistics ===
/// [данные]
/// \endcode
///
void TaskView::showUserStats(const std::string& stats) {
    std::cout << "\n=== User Statistics ===\n" << stats << std::endl;
}
