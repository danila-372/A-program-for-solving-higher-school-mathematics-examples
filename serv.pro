QT += core network sql
CONFIG -= app_bundle
CONFIG += c++11 console

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        db.cpp \
        functionsforserver.cpp \
        main.cpp \
        mathparser.cpp \
    mytcpserver.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    db.h \
    functionsforserver.h \
    mathparser.h \
    mytcpserver.h
