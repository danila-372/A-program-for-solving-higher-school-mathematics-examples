#ifndef LOGINVIEW_H
#define LOGINVIEW_H

#include <QWidget>
#include "funcforclient.h"

namespace Ui {
class LoginView;
}

class LoginView : public QWidget
{
    Q_OBJECT

public:
    explicit LoginView(QWidget *parent = nullptr);
    ~LoginView();

private slots:
    void on_pushButton_auth_clicked();

private:
    Ui::LoginView *ui;
    void clear_form();
signals:
    void auth_user_ok();
    void auth_admin_ok();
};

#endif // LOGINVIEW_H
