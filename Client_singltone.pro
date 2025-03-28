QT = core

CONFIG += c++17 cmdline

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        RegistrationView.cpp \
        TaskView.cpp \
        appcontroller.cpp \
        connection.cpp \
        loginView.cpp \
        main.cpp \
        user_model.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    TaskView.h \
    appcontroller.h \
    connection.h \
    loginView.h \
    registrationView.h \
    user_model.h
