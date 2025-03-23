QT += core network sql
CONFIG += c++11 console
CONFIG -= app_bundle

SOURCES += main.cpp \
           mytcpserver.cpp \
           db.cpp \
           functionsforserver.cpp

HEADERS += mytcpserver.h \
           db.h \
           functionsforserver.h
