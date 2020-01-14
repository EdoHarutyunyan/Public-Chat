#pragma once

#include <QMainWindow>
#include <QFileDialog>
#include <QImage>

#include "../Data/userlist.h"
#include "../Data/userdata.h"
#include "../Data/message.h"

namespace Ui {
class ChatWidget;
}

class ChatWidget : public QMainWindow
{
    Q_OBJECT
public:
    explicit ChatWidget(QWidget *parent = nullptr);

    void addContact(const UserData &username);
    void removeContact(const QString &username);
    void addContacts(const UserList &lst);
    void addMessage(const Message &msg);

    ~ChatWidget();

private slots:
    void on_SendButton_clicked();
    void on_loadImageButton_clicked();

signals:
    void messageSent(Message &msg);

private:
    Ui::ChatWidget *ui;
    QPixmap bufferedImage;
};
