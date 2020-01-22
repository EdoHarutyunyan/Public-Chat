#include "authmenu.h"
#include "ui_authmenu.h"
#include <qDebug>
#include <QMovie>


AuthMenu::AuthMenu(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::AuthMenu)
{
    ui->setupUi(this);
    ui->Login->setFont(QFont("Gadugi", 12));
    ui->Password->setFont(QFont("Gadugi", 12));
    ui->Password->setEchoMode(QLineEdit::Password);
}

AuthMenu::~AuthMenu()
{
    delete ui;
}

void AuthMenu::on_SignButton_clicked()
{
    auto login = ui->Login->text();
    auto password = ui->Password->text();

    if (login.isEmpty() || password.isEmpty())
    {
        this->setStatus("Invalid login/password");
        return;
    }

    ui->SignButton->setEnabled(false);
    ui->Login->setEnabled(false);
    ui->Password->setEnabled(false);

    auto preloader = new QMovie(":/Image/46.gif");
    preloader->start();
    ui->Status->setMovie(preloader);

    emit signIn(login, password);
}

void AuthMenu::setStatus(const QString &status)
{
    ui->Status->setText(status);
    ui->SignButton->setEnabled(true);
    ui->Login->setEnabled(true);
    ui->Password->setEnabled(true);
}


void AuthMenu::on_CreateAccountButton_clicked()
{
    emit SignUpButtonClicked();
}
