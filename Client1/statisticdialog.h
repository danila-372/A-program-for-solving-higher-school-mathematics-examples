///
/// \file statisticdialog.h
/// \brief Заголовочный файл класса диалога статистики администратора
/// 

#ifndef STATISTICDIALOG_H
#define STATISTICDIALOG_H

#include <QDialog>
#include <QTcpSocket>
#include <QMessageBox>

namespace Ui {
class StatisticDialog;
}

///
/// \class StatisticDialog
/// \brief Класс диалога отображения статистики пользователей
/// \details Реализует:
/// - Паттерн Singleton для единственного экземпляра
/// - Получение и отображение статистики с сервера
/// - Автоматическое обновление данных
///
class StatisticDialog : public QDialog
{
    Q_OBJECT

public:
    ///
    /// \brief Статический метод получения экземпляра класса (Singleton)
    /// \param[in] socket Указатель на TCP-сокет (по умолчанию nullptr)
    /// \param[in] parent Родительский виджет (по умолчанию nullptr)
    /// \return Указатель на единственный экземпляр класса
    ///
    static StatisticDialog* instance(QTcpSocket* socket = nullptr, QWidget* parent = nullptr);
    
    ///
    /// \brief Метод обновления статистики
    /// \note Отправляет запрос на сервер для получения актуальных данных
    ///
    void refreshStats();

private:
    ///
    /// \brief Приватный конструктор (часть реализации Singleton)
    /// \param[in] socket Указатель на TCP-сокет
    /// \param[in] parent Родительский виджет
    ///
    explicit StatisticDialog(QTcpSocket* socket, QWidget* parent = nullptr);
    
    /// \brief Деструктор класса
    ~StatisticDialog();
    
    /// \brief Указатель на единственный экземпляр класса (Singleton)
    static StatisticDialog* m_instance;
    
    /// \brief Указатель на пользовательский интерфейс
    Ui::StatisticDialog* ui;
    
    /// \brief Указатель на TCP-сокет для связи с сервером
    QTcpSocket* m_socket;

private slots:
    ///
    /// \brief Слот обработки входящих данных от сервера
    /// \details Парсит и отображает полученную статистику
    ///
    void onSocketReadyRead();
    
    ///
    /// \brief Слот обработки нажатия кнопки обновления
    ///
    void on_pushButton_refresh_clicked();
};

#endif // STATISTICDIALOG_H
