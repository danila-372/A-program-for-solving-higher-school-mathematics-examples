#include "loginview.h"
#include "ui_loginview.h"

LoginView::LoginView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LoginView)
{
    ui->setupUi(this);
}

LoginView::~LoginView()
{
    delete ui;
}

void LoginView::on_pushButton_auth_clicked()
{
    QString l = ui->lineEdit_log->text();
    if(auth(l,p)=="user")
    {
    this->close();
    emit auth_user_ok();
    }

    else if(auth(l,p)=="admin")
    {
        this->close();
        emit auth_admin_ok();
    }

    else
        clear_form();
}

