# Отдельный файл для тестов
QT += testlib sql network
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

TARGET = matrix_generator_test

# Тестовые файлы
SOURCES += tst_matrixgenerator.cpp
HEADERS += tst_matrixgenerator.h

# Пути к исходным файлам сервера
INCLUDEPATH += ../Server1/
HEADERS += ../Server1/mathparser.h
SOURCES += ../Server1/mathparser.cpp

