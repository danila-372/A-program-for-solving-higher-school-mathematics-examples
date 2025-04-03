/********************************************************************************
** Form generated from reading UI file 'taskview.ui'
**
** Created by: Qt User Interface Compiler version 6.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TASKVIEW_H
#define UI_TASKVIEW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TaskView
{
public:
    QLabel *label;

    void setupUi(QWidget *TaskView)
    {
        if (TaskView->objectName().isEmpty())
            TaskView->setObjectName("TaskView");
        TaskView->resize(400, 300);
        label = new QLabel(TaskView);
        label->setObjectName("label");
        label->setGeometry(QRect(140, 40, 63, 20));

        retranslateUi(TaskView);

        QMetaObject::connectSlotsByName(TaskView);
    } // setupUi

    void retranslateUi(QWidget *TaskView)
    {
        TaskView->setWindowTitle(QCoreApplication::translate("TaskView", "Form", nullptr));
        label->setText(QCoreApplication::translate("TaskView", "Task", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TaskView: public Ui_TaskView {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TASKVIEW_H
