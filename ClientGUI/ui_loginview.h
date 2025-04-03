/********************************************************************************
** Form generated from reading UI file 'loginview.ui'
**
** Created by: Qt User Interface Compiler version 6.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGINVIEW_H
#define UI_LOGINVIEW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LoginView
{
public:
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLineEdit *lineEdit_log;
    QLineEdit *lineEdit_2;
    QLineEdit *lineEdit_3;
    QLineEdit *lineEdit_4;
    QPushButton *pushButton_auth;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;

    void setupUi(QWidget *LoginView)
    {
        if (LoginView->objectName().isEmpty())
            LoginView->setObjectName("LoginView");
        LoginView->resize(400, 300);
        label = new QLabel(LoginView);
        label->setObjectName("label");
        label->setGeometry(QRect(40, 50, 63, 20));
        label_2 = new QLabel(LoginView);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(40, 90, 63, 20));
        label_3 = new QLabel(LoginView);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(40, 130, 63, 20));
        label_4 = new QLabel(LoginView);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(40, 170, 63, 20));
        lineEdit_log = new QLineEdit(LoginView);
        lineEdit_log->setObjectName("lineEdit_log");
        lineEdit_log->setGeometry(QRect(150, 50, 113, 26));
        lineEdit_2 = new QLineEdit(LoginView);
        lineEdit_2->setObjectName("lineEdit_2");
        lineEdit_2->setGeometry(QRect(150, 90, 113, 26));
        lineEdit_3 = new QLineEdit(LoginView);
        lineEdit_3->setObjectName("lineEdit_3");
        lineEdit_3->setGeometry(QRect(150, 130, 113, 26));
        lineEdit_4 = new QLineEdit(LoginView);
        lineEdit_4->setObjectName("lineEdit_4");
        lineEdit_4->setGeometry(QRect(150, 170, 113, 26));
        pushButton_auth = new QPushButton(LoginView);
        pushButton_auth->setObjectName("pushButton_auth");
        pushButton_auth->setGeometry(QRect(20, 240, 93, 29));
        pushButton_2 = new QPushButton(LoginView);
        pushButton_2->setObjectName("pushButton_2");
        pushButton_2->setGeometry(QRect(150, 240, 93, 29));
        pushButton_3 = new QPushButton(LoginView);
        pushButton_3->setObjectName("pushButton_3");
        pushButton_3->setGeometry(QRect(270, 240, 93, 29));

        retranslateUi(LoginView);

        QMetaObject::connectSlotsByName(LoginView);
    } // setupUi

    void retranslateUi(QWidget *LoginView)
    {
        LoginView->setWindowTitle(QCoreApplication::translate("LoginView", "Form", nullptr));
        label->setText(QCoreApplication::translate("LoginView", "TextLabel", nullptr));
        label_2->setText(QCoreApplication::translate("LoginView", "TextLabel", nullptr));
        label_3->setText(QCoreApplication::translate("LoginView", "TextLabel", nullptr));
        label_4->setText(QCoreApplication::translate("LoginView", "TextLabel", nullptr));
        pushButton_auth->setText(QCoreApplication::translate("LoginView", "sign in", nullptr));
        pushButton_2->setText(QCoreApplication::translate("LoginView", "sign up", nullptr));
        pushButton_3->setText(QCoreApplication::translate("LoginView", "change", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LoginView: public Ui_LoginView {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGINVIEW_H
