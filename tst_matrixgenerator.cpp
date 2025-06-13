#include "tst_matrixgenerator.h"
#include "../Server1/mathparser.h"
#include <QRegularExpression>
#include <QSet>
#include <QRandomGenerator>
#include <QCoreApplication>

void MatrixGeneratorTest::testMatrixGenerationFormat()
{
    QString matrix = MathParser::generateMatrixProblem();

    // Проверяем общий формат: [[матрица1]]операция[[матрица2]]
    QVERIFY2(matrix.startsWith("[[[") && matrix.contains("]]") && matrix.endsWith("]]"),
             "Matrix should be in format '[[matrix1]]operation[[matrix2]]'");

    // Проверяем наличие оператора между матрицами
    QRegularExpression opRegex(R"(\]\](?<op>\+|\*)\[\[)");
    QVERIFY2(opRegex.match(matrix).hasMatch(),
             "Matrix should contain operator '+' or '*' between matrices");
}

void MatrixGeneratorTest::testMatrixContent()
{
    QString matrix = MathParser::generateMatrixProblem();

    // Упрощенная проверка содержимого
    // 1. Проверяем, что все символы допустимы
    QRegularExpression charRegex(R"([^0-9,\[\]\+\*])");
    QVERIFY2(!charRegex.match(matrix).hasMatch(),
             "Matrix contains invalid characters");

    // 2. Извлекаем все числа
    QRegularExpression numRegex(R"(\d+)");
    QRegularExpressionMatchIterator i = numRegex.globalMatch(matrix);

    while (i.hasNext()) {
        QRegularExpressionMatch match = i.next();
        int num = match.captured().toInt();
        QVERIFY2(num >= 1 && num <= 9,
                 QString("Matrix element %1 should be between 1 and 9").arg(num).toUtf8().constData());
    }

}

void MatrixGeneratorTest::testMatrixSizes()
{
    QString matrix = MathParser::generateMatrixProblem();

    // Извлекаем первую матрицу
    int start = matrix.indexOf("[[") + 2;
    int end = matrix.indexOf("]]", start);
    QString matrix1 = matrix.mid(start, end - start);

    // Подсчитываем количество элементов
    int commaCount = matrix1.count(",");
    int bracketCount = matrix1.count("],[");

    // Проверяем размерность (2x2 или 3x3)
    bool validSize = (commaCount == 3 && bracketCount == 1) || // 2x2: [a,b],[c,d] -> 3 запятых, 1 разделитель
                     (commaCount == 8 && bracketCount == 2);    // 3x3: [a,b,c],[d,e,f],[g,h,i] -> 8 запятых, 2 разделителя

    QVERIFY2(validSize, "Matrix should be 2x2 or 3x3");
}

void MatrixGeneratorTest::testMultipleGenerations()
{
    QSet<QString> generatedMatrices;
    const int testCount = 100;

    for (int i = 0; i < testCount; i++) {
        QString matrix = MathParser::generateMatrixProblem();
        generatedMatrices.insert(matrix);
    }

    // Проверяем, что генерируются разные матрицы
    QVERIFY2(generatedMatrices.size() > testCount * 0.9,
             "Matrix generator should produce unique matrices");
}

QTEST_APPLESS_MAIN(MatrixGeneratorTest)

