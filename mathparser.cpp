///
/// \file mathparser.cpp
/// \brief Реализация математического парсера и генератора задач
///

#include "mathparser.h"
#include <QRandomGenerator>
#include <QDebug>

///
/// \brief Разбирает и вычисляет математическое выражение
/// \param expression Строка с математическим выражением
/// \return Пара: исходное выражение и результат вычисления или сообщение об ошибке
///
QPair<QString, QString> MathParser::parseAndCalculate(const QString &expression) {
    QString simplified = simplifyMathExpression(expression);

    try {
        double result = evaluateExpression(simplified);
        return {expression, QString::number(result, 'g', 10)};
    } catch (...) {
        return {expression, "Error: Could not calculate"};
    }
}

///
/// \brief Вычисляет значение математического выражения
/// \param expr Упрощенное математическое выражение
/// \return Результат вычисления
///
double MathParser::evaluateExpression(const QString &expr) {
    return evaluate(expr);
}

///
/// \brief Упрощает математическое выражение для последующего разбора
/// \param expr Исходное математическое выражение
/// \return Упрощенное выражение
///
QString MathParser::simplifyMathExpression(QString expr) {
    expr.remove(" ");
    expr.replace("^", "**");

    QMap<QString, QString> funcReplacements = {
        {"sqrt", "√"}, {"cbrt", "∛"}, {"log", "log10"},
        {"ln", "log"}, {"sin", "sin"}, {"cos", "cos"}, {"tan", "tan"}
    };

    for (auto it = funcReplacements.begin(); it != funcReplacements.end(); ++it) {
        expr.replace(it.key(), it.value());
    }

    return expr;
}

///
/// \brief Основной метод вычисления выражения
/// \param expr Упрощенное выражение
/// \return Результат вычисления
///
double MathParser::evaluate(const QString &expr) {
    int pos = 0;
    return parseExpression(expr, pos);
}

///
/// \brief Разбирает выражение (сложение/вычитание)
/// \param expr Упрощенное выражение
/// \param pos Текущая позиция в строке (используется для рекурсивного разбора)
/// \return Результат вычисления
///
double MathParser::parseExpression(const QString &expr, int &pos) {
    double result = parseTerm(expr, pos);

    while (pos < expr.length()) {
        QChar op = expr[pos];
        if (op != '+' && op != '-') break;

        pos++;
        double term = parseTerm(expr, pos);

        if (op == '+') result += term;
        else result -= term;
    }

    return result;
}

///
/// \brief Разбирает терм (умножение/деление)
/// \param expr Упрощенное выражение
/// \param pos Текущая позиция в строке
/// \return Результат вычисления
///
double MathParser::parseTerm(const QString &expr, int &pos) {
    double result = parseFactor(expr, pos);

    while (pos < expr.length()) {
        QChar op = expr[pos];
        if (op != '*' && op != '/') break;

        pos++;
        double factor = parseFactor(expr, pos);

        if (op == '*') result *= factor;
        else {
            if (factor == 0.0) throw std::runtime_error("Division by zero");
            result /= factor;
        }
    }

    return result;
}

///
/// \brief Разбирает фактор (скобки/функции/числа)
/// \param expr Упрощенное выражение
/// \param pos Текущая позиция в строке
/// \return Результат вычисления
///
double MathParser::parseFactor(const QString &expr, int &pos) {
    if (pos >= expr.length()) {
        throw std::runtime_error("Unexpected end of expression");
    }

    if (expr[pos] == '(') {
        pos++;
        double result = parseExpression(expr, pos);

        if (pos >= expr.length() || expr[pos] != ')') {
            throw std::runtime_error("Mismatched parentheses");
        }

        pos++;
        return result;
    }

    if (expr[pos].isLetter()) {
        int start = pos;
        while (pos < expr.length() && expr[pos].isLetter()) {
            pos++;
        }

        QString func = expr.mid(start, pos - start);

        if (pos >= expr.length() || expr[pos] != '(') {
            throw std::runtime_error("Function call without parentheses");
        }

        pos++;
        double arg = parseExpression(expr, pos);

        if (pos >= expr.length() || expr[pos] != ')') {
            throw std::runtime_error("Mismatched parentheses in function call");
        }

        pos++;

        if (func == "√" || func == "sqrt") return sqrt(arg);
        else if (func == "∛" || func == "cbrt") return cbrt(arg);
        else if (func == "log10" || func == "log") return log10(arg);
        else if (func == "ln") return log(arg);
        else if (func == "sin") return sin(arg);
        else if (func == "cos") return cos(arg);
        else if (func == "tan") return tan(arg);
        else throw std::runtime_error("Unknown function: " + func.toStdString());
    }

    return parseNumber(expr, pos);
}

///
/// \brief Разбирает число из строки
/// \param expr Упрощенное выражение
/// \param pos Текущая позиция в строке
/// \return Числовое значение
///
double MathParser::parseNumber(const QString &expr, int &pos) {
    int startPos = pos;
    bool isNegative = false;

    if (expr[pos] == '-') {
        isNegative = true;
        pos++;
    }

    while (pos < expr.length() && (expr[pos].isDigit() || expr[pos] == '.')) {
        pos++;
    }

    QString numStr = expr.mid(startPos, pos - startPos);
    bool ok;
    double num = numStr.toDouble(&ok);

    if (!ok) throw std::runtime_error("Invalid number: " + numStr.toStdString());
    return isNegative ? -num : num;
}

///
/// \brief Генерирует задачи по указанной категории
/// \param category Категория задач (algebra, calculus и т.д.)
/// \return Вектор пар: текст задачи и ответ
///
QVector<QPair<QString, QString>> MathParser::generateAdvancedProblems(const QString &category) {
    QVector<QPair<QString, QString>> problems;
    QRandomGenerator *rg = QRandomGenerator::global();

    if (category == "algebra") {
        problems.append({generatePolynomial(2), ""});
        problems.append({generatePolynomial(3), ""});
        problems.append({QString("(%1)/(%2)").arg(generatePolynomial(2)).arg(generatePolynomial(1)), ""});
    }
    else if (category == "calculus") {
        problems.append({generateIntegral(), ""});
        problems.append({"derivative(" + generatePolynomial(3) + ")", ""});
    }
    else if (category == "trigonometry") {
        problems.append({generateTrigonometric(), ""});
    }
    else if (category == "matrices") {
        problems.append({generateMatrixProblem(), ""});
    }

    for (auto &problem : problems) {
        auto result = parseAndCalculate(problem.first);
        problem.second = result.second;
    }

    return problems;
}

///
/// \brief Генерирует полином заданной степени
/// \param degree Степень полинома
/// \return Строковое представление полинома
///
QString MathParser::generatePolynomial(int degree) {
    QRandomGenerator *rg = QRandomGenerator::global();
    QString polynomial;
    QString var = "x";

    for (int i = degree; i >= 0; i--) {
        int coeff = rg->bounded(1, 10);
        if (i < degree && coeff > 0) polynomial += "+";

        if (i > 1) polynomial += QString("%1%2^%3").arg(coeff).arg(var).arg(i);
        else if (i == 1) polynomial += QString("%1%2").arg(coeff).arg(var);
        else polynomial += QString::number(coeff);
    }

    return polynomial;
}

///
/// \brief Генерирует тригонометрическое выражение
/// \return Строковое представление тригонометрической функции
///
QString MathParser::generateTrigonometric() {
    QRandomGenerator *rg = QRandomGenerator::global();
    QStringList funcs = {"sin", "cos", "tan"};
    QString func = funcs[rg->bounded(0, funcs.size())];
    QString arg = QString("%1*(x+%2)").arg(QRandomGenerator::global()->bounded(1, 5)).arg(QRandomGenerator::global()->bounded(1, 5));
    return func + "(" + arg + ")";
}

///
/// \brief Генерирует задачу на операции с матрицами
/// \return Строковое представление матричной операции
///
QString MathParser::generateMatrixProblem() {
    QRandomGenerator *rg = QRandomGenerator::global();
    int size = rg->bounded(2, 4);
    QString matrix1, matrix2;

    for (int i = 0; i < size; i++) {
        matrix1 += "[";
        matrix2 += "[";
        for (int j = 0; j < size; j++) {
            matrix1 += QString::number(rg->bounded(1, 10));
            matrix2 += QString::number(rg->bounded(1, 10));
            if (j < size-1) {
                matrix1 += ",";
                matrix2 += ",";
            }
        }
        matrix1 += "]";
        matrix2 += "]";
        if (i < size-1) {
            matrix1 += ",";
            matrix2 += ",";
        }
    }

    QStringList ops = {"+", "*"};
    QString op = ops[rg->bounded(0, ops.size())];

    return QString("[[%1]]%2[[%3]]").arg(matrix1).arg(op).arg(matrix2);
}

///
/// \brief Генерирует интегральное выражение
/// \return Строковое представление интеграла
///
QString MathParser::generateIntegral() {
    QRandomGenerator *rg = QRandomGenerator::global();
    QString func = QString("%1*x^%2 + %3").arg(rg->bounded(1, 5)).arg(rg->bounded(1, 3)).arg(rg->bounded(1, 5));
    return QString("integral(%1) dx").arg(generatePolynomial(2));
}

