#ifndef AUTHMENU_H
#define AUTHMENU_H

#include <QMainWindow>
#include <QString>

namespace Ui {
class AuthMenu;
}

class AuthMenu : public QMainWindow
{
    Q_OBJECT

public:

    explicit AuthMenu(QWidget *parent = nullptr);
    ~AuthMenu();

    void setStatus(const QString &status);

signals:

    void signIn(const QString &username, const QString &password);
    void SignUpButtonClicked();

private slots:

    void on_SignButton_clicked();
    void on_CreateAccountButton_clicked();

private:
    Ui::AuthMenu *ui;
};

#endif // AUTHMENU_H
