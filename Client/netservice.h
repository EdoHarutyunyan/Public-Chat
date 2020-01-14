#ifndef NETSERVICE_H
#define NETSERVICE_H

#include "authmenu.h"
#include "signup.h"
#include "chatwidget.h"

#include <QTcpSocket>
#include <QHostAddress>
#include <QTimer>

class NetService : QObject
{
    Q_OBJECT
public:

    explicit NetService(QObject *parent = nullptr);
    virtual ~NetService();

    void write(QTcpSocket *socket, const BaseData &data);

private slots:

    void onSignIn(const QString &username, const QString &password);
    void onSignUp(const QString &username, const QString &password);
    void onSignUpButtonClicked();

    void onSocketConnected();
    void onSocketDisconnected();
    void onConnectionTimeOut();
    void onSocketReadyRead();
    void onMessageSent(Message &msg);

private:
    void connectSocketSignals();
    void readAuthAnswer(QDataStream &readStream);
    void readSignUpAnswer(QDataStream &readStream);
    void readAddContactAnswer(QDataStream &readStream);
    void readUserList(QDataStream &readStream);
    void readNewConnection(QDataStream &readStream);
    void readDisconnection(QDataStream &readStream);
    void readMessage(QDataStream &readStream);
private:

    AuthMenu *menu;
    SignUp* signUp;
    ChatWidget *chat;
    QTcpSocket *socket;
    QTimer *ctimer; // connection timer
    UserData userData;
};

#endif // NETSERVICE_H
