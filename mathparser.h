///
/// \file mathparser.h
/// \brief Заголовочный файл класса для парсинга и генерации математических выражений
///

#ifndef MATHPARSER_H
#define MATHPARSER_H

#include <QString>
#include <QVector>
#include <QPair>
#include <cmath>

///
/// \brief Класс для работы с математическими выражениями
///
/// Предоставляет функционал для:
/// - Парсинга и вычисления математических выражений
/// - Генерации задач по различным категориям математики
/// - Упрощения математических выражений
///
class MathParser
{
public:
    ///
    /// \brief Разбирает и вычисляет математическое выражение
    /// \param expression Строка с математическим выражением
    /// \return Пара: исходное выражение и результат вычисления или сообщение об ошибке
    ///
    static QPair<QString, QString> parseAndCalculate(const QString &expression);

    ///
    /// \brief Вычисляет значение математического выражения
    /// \param expr Упрощенное математическое выражение
    /// \return Результат вычисления
    ///
    static double evaluateExpression(const QString &expr);

    ///
    /// \brief Упрощает математическое выражение для последующего разбора
    /// \param expr Исходное математическое выражение
    /// \return Упрощенное выражение
    ///
    static QString simplifyMathExpression(QString expr);

    ///
    /// \brief Генерирует задачи по указанной категории
    /// \param category Категория задач (algebra, calculus и т.д.)
    /// \return Вектор пар: текст задачи и ответ
    ///
    static QVector<QPair<QString, QString>> generateAdvancedProblems(const QString &category);

private:
    ///
    /// \brief Основной метод вычисления выражения
    /// \param expr Упрощенное выражение
    /// \return Результат вычисления
    ///
    static double evaluate(const QString &expr);

    ///
    /// \brief Разбирает выражение (сложение/вычитание)
    /// \param expr Упрощенное выражение
    /// \param pos Текущая позиция в строке (используется для рекурсивного разбора)
    /// \return Результат вычисления
    ///
    static double parseExpression(const QString &expr, int &pos);

    ///
    /// \brief Разбирает терм (умножение/деление)
    /// \param expr Упрощенное выражение
    /// \param pos Текущая позиция в строке
    /// \return Результат вычисления
    ///
    static double parseTerm(const QString &expr, int &pos);

    ///
    /// \brief Разбирает фактор (скобки/функции/числа)
    /// \param expr Упрощенное выражение
    /// \param pos Текущая позиция в строке
    /// \return Результат вычисления
    ///
    static double parseFactor(const QString &expr, int &pos);

    ///
    /// \brief Разбирает число из строки
    /// \param expr Упрощенное выражение
    /// \param pos Текущая позиция в строке
    /// \return Числовое значение
    ///
    static double parseNumber(const QString &expr, int &pos);

    ///
    /// \brief Генерирует задачу на операции с матрицами
    /// \return Строковое представление матричной операции
    ///
    static QString generateMatrixProblem();

    ///
    /// \brief Генерирует тригонометрическое выражение
    /// \return Строковое представление тригонометрической функции
    ///
    static QString generateTrigonometric();

    ///
    /// \brief Генерирует полином заданной степени
    /// \param degree Степень полинома
    /// \return Строковое представление полинома
    ///
    static QString generatePolynomial(int degree);

    ///
    /// \brief Генерирует интегральное выражение
    /// \return Строковое представление интеграла
    ///
    static QString generateIntegral();
};

#endif // MATHPARSER_H
