#include "signup.h"
#include "ui_signup.h"
#include <qDebug>
#include <QMovie>

SignUp::SignUp(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SignUp)
{
    ui->setupUi(this);
}

SignUp::~SignUp()
{
    delete ui;
}

void SignUp::on_SignUpButton_clicked()
{
    auto login = ui->NewLogin->text();
    auto password = ui->NewPassword->text();
    auto confirm = ui->Confirm->text();

    if (login.isEmpty() || password.isEmpty() || confirm.isEmpty())
    {
        this->setStatus("Kisat data");
        return;
    }
    if (password != confirm)
    {
        this->setStatus("password != confirm");
        return;
    }

    ui->SignUpButton->setEnabled(false);
    ui->NewLogin->setEnabled(false);
    ui->NewPassword->setEnabled(false);

    auto preloader = new QMovie(":/Image/46.gif");
    preloader->start();
    ui->signUpStatus->setMovie(preloader);

    emit signUp(login, password);
}

void SignUp::setStatus(const QString &status)
{
    ui->signUpStatus->setText(status);
    ui->SignUpButton->setEnabled(true);
    ui->NewLogin->setEnabled(true);
    ui->NewPassword->setEnabled(true);
    ui->Confirm->setEnabled(true);
}
