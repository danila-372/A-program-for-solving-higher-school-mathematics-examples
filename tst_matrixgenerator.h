#ifndef TST_MATRIXGENERATOR_H
#define TST_MATRIXGENERATOR_H
#include <QtTest>

class MatrixGeneratorTest : public QObject
{
    Q_OBJECT
public:
    MatrixGeneratorTest() = default;

private slots:
    void testMatrixGenerationFormat();
    void testMatrixContent();
    void testMatrixSizes();
    void testMultipleGenerations();
};
#endif // TST_MATRIXGENERATOR_H
