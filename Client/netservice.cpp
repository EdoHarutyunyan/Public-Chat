#include "netservice.h"

#include "../Data/authanswer.h"
#include "../Data/userdata.h"
#include "../Data/userlist.h"
#include "../Data/signupanswer.h"
#include <QMessageBox>

#include <cassert>

NetService::NetService(QObject *parent)
    : QObject(parent),
    menu(new AuthMenu()),
    signUp(new SignUp()),
    chat(new ChatWidget()),
    socket(new QTcpSocket(this))
    //ctimer(new QTimer(this))
{
    connectSocketSignals();

    socket->connectToHost(QHostAddress::LocalHost, 8001);
    //ctimer->start(10000);

    chat->setVisible(false);
    signUp->setVisible(false);

    connect(menu, SIGNAL(signIn(QString, QString)),
            this, SLOT(onSignIn(QString, QString)));

    connect(menu, SIGNAL(SignUpButtonClicked()),
            this, SLOT(onSignUpButtonClicked()));

    connect(signUp, SIGNAL(signUp(QString, QString)),
            this, SLOT(onSignUp(QString, QString)));

    //connect(ctimer, SIGNAL(timeout()),
      //      this, SLOT(onConnectionTimeOut()));

    connect(chat, SIGNAL(messageSent(Message&)),
            this, SLOT(onMessageSent(Message&)));
}

NetService::~NetService()
{
    delete menu;
    delete signUp;
    delete chat;
}

void NetService::connectSocketSignals()
{
    connect(socket, SIGNAL(connected()), this, SLOT(onSocketConnected()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(onSocketDisconnected()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(onSocketReadyRead()));
}

void NetService::onSignIn(const QString &username, const QString &password)
{
    userData.setUsername(username);
    userData.setPassword(password);
    userData.setType(DataType::AuthRequest);

    write(socket, userData);
}

void NetService::onSignUp(const QString &username, const QString &password)
{
    userData.setUsername(username);
    userData.setPassword(password);
    userData.setType(DataType::SignUpRequest);

    write(socket, userData);
}

void NetService::onSocketConnected()
{
    menu->show();
}

void NetService::onSocketDisconnected()
{
    socket->abort();
}

void NetService::onConnectionTimeOut()
{
    socket->abort();
    menu->setStatus("Connection timeout.");
}

void NetService::onSignUpButtonClicked()
{
    menu->close();
    signUp->show();
}

void NetService::readAuthAnswer(QDataStream &readStream)
{
    AuthAnswer ans;
    readStream >> ans;

    if (!ans.isSigned())
    {
        menu->setStatus("Authentication error.");
        socket->abort();
        return;
    }

    menu->close();
    chat->show();
}

void NetService::readSignUpAnswer(QDataStream &readStream)
{
    SignUpAnswer signUpAnswer;
    readStream >> signUpAnswer;

    if (!signUpAnswer.isSuccess())
    {
        signUp->setStatus("Registration failed.");
        socket->abort();
        return;
    }

    signUp->close();
    menu->show();
}

void NetService::readAddContactAnswer(QDataStream &readStream)
{
    SignUpAnswer signUpAnswer;
    readStream >> signUpAnswer;

    if (!signUpAnswer.isSuccess())
    {
        signUp->setStatus("Registration failed.");
        socket->abort();
        return;
    }

    signUp->close();
    menu->show();
}


void NetService::readUserList(QDataStream &readStream)
{
    UserList lst;
    readStream >> lst;
    chat->addContacts(lst);
}

void NetService::readNewConnection(QDataStream &readStream)
{
    UserData user;
    readStream >> user;
    chat->addContact(user);
}

void NetService::readDisconnection(QDataStream &readStream)
{
    UserData user;
    readStream >> user;
    chat->removeContact(user.getUsername());
}

void NetService::readMessage(QDataStream &readStream)
{
    Message msg;
    readStream >> msg;
    chat->addMessage(msg);
}

void NetService::write(QTcpSocket *socket, const BaseData &data)
{
    QByteArray datagram;
    QDataStream writeStream(&datagram, QIODevice::WriteOnly);
    writeStream << qint16(0) << (qint8)data.type();
    writeStream << data;
    writeStream.device()->seek(0);
    writeStream << qint16(datagram.size() - sizeof(qint16)); // writing data size
    socket->write(datagram);
    socket->waitForBytesWritten();
}

void NetService::onSocketReadyRead()
{
    auto socket = static_cast<QTcpSocket *>(sender());
    QDataStream readStream(socket);
    qint16 sizeRead;
    qint8 type;

    readStream >> sizeRead;
    if (socket->bytesAvailable() < sizeRead)
    {
       return;
    }

    readStream >> type;

    switch (type)
    {
    case DataType::AuthResponse:
        readAuthAnswer(readStream);
        break;
    case DataType::SignUpResponse:
        readSignUpAnswer(readStream);
        break;
    case DataType::AddContactResponse:
        readAddContactAnswer(readStream);
        break;
    case DataType::UserListResponse:
        readUserList(readStream);
        break;
    case DataType::NewConnection:
        readNewConnection(readStream);
        break;
    case DataType::Disconnection:
        readDisconnection(readStream);
        break;
    case DataType::Msg:
        readMessage(readStream);
        break;
    default:
        assert(false);
        break;
    }
}

void NetService::onMessageSent(Message &msg)
{
    msg.setSender(userData.getUsername());
    write(socket, msg);
}
