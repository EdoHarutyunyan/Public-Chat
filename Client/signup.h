#ifndef SIGNUP_H
#define SIGNUP_H

#include <QMainWindow>

namespace Ui {
class SignUp;
}

class SignUp : public QMainWindow
{
    Q_OBJECT

public:
    explicit SignUp(QWidget *parent = nullptr);
    ~SignUp();

    void setStatus(const QString &status);

signals:
    void signUp(const QString &newUsername, const QString &password);

private slots:
     void on_SignUpButton_clicked();

private:
    Ui::SignUp *ui;
};

#endif // SIGNUP_H
