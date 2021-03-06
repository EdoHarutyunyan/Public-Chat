#include "connectionhandler.h"

ConnectionHandler::ConnectionHandler(QObject *parent) :
    QObject(parent),
    serv(new QTcpServer(this))
{
    connect(serv, SIGNAL(newConnection()),
            this, SLOT(on_New_Connection()));
}

ConnectionHandler::~ConnectionHandler()
{
    delete serv;
}

void ConnectionHandler::startServer()
{
    serv->listen(QHostAddress::Any, 8001);
}

void ConnectionHandler::stopServer()
{
    serv->close();
    clients.clear();
}

void ConnectionHandler::on_New_Connection()
{
    auto client = serv->nextPendingConnection();
    connectSocketSignals(client);
    qDebug() << "New connection try from" << getIPv4AddrString(client) << "...";
}

void ConnectionHandler::connectSocketSignals(QTcpSocket *socket)
{
    qDebug() << "ConnectionHandler::connectSocketSignals()";
    connect(socket, SIGNAL(readyRead()),
            this, SLOT(on_Socket_Ready_Read()));

    connect(socket, SIGNAL(disconnected()),
            this, SLOT(on_Client_Disconnection()));

    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(on_Socket_Error(QAbstractSocket::SocketError)));
}

void ConnectionHandler::on_Client_Disconnection()
{
    //auto disconnectedSocket = static_cast<QTcpSocket *>(sender());
    //auto username = clients.key(disconnectedSocket);
    //clients.remove(username);
    //username.setType(Disconnection);
    //writeAboutUserDisconnection(username);
}

bool ConnectionHandler::isListening() const
{
    return serv->isListening();
}

void ConnectionHandler::on_Socket_Error(QAbstractSocket::SocketError)
{
    auto socket = static_cast<QTcpSocket *>(sender());
    qDebug() << getIPv4AddrString(socket) << socket->errorString();
}

QString ConnectionHandler::getIPv4AddrString(QTcpSocket *socket)
{
    QString IPv4IPv6 = socket->peerAddress().toString();
    QString IPv4 = IPv4IPv6.right(IPv4IPv6.size() - 7);
    return IPv4;
}

void ConnectionHandler::write(QTcpSocket *socket, const BaseData &data)
{
    QByteArray datagram;
    QDataStream writeStream(&datagram, QIODevice::WriteOnly);
    writeStream << qint16(0) << (qint8)data.type();
    writeStream << data;
    writeStream.device()->seek(0);
    writeStream << qint16(datagram.size() - sizeof(qint16));
    socket->write(datagram);
    socket->waitForBytesWritten();
}

void ConnectionHandler::on_Socket_Ready_Read()
{
    auto socket = static_cast<QTcpSocket*>(sender());
    QDataStream readStream(socket);
    qint16 sizeRead;
    qint8 type;
    readStream >> sizeRead;

    if (socket->bytesAvailable() < sizeRead)
    {
        return;
    }

    qDebug() << "ConnectionHandler::on_Socket_Ready_Read()";
    readStream >> type;
    if (type == DataType::AuthRequest)
    {
        readAuthRequest(socket);
    }
    else if (type == DataType::SignUpRequest)
    {
        readSignUpRequest(socket);
    }
    else if (type == DataType::AddContactRequest)
    {
        readAddContactRequest(socket);
    }
    else if (type == DataType::Msg)
    {
        readMessage(socket);
    }
}

void ConnectionHandler::readAuthRequest(QTcpSocket *socket)
{
    qDebug() << "ConnectionHandler::readAuthRequest()";
    QDataStream readStream(socket);
    UserData read;
    readStream >> read;

    bool isExist = false;

    for(auto it = clients.begin(); it != clients.end(); ++it)
    {
        if (it.key().getUsername() == read.getUsername() && it.key().getPassword() == read.getPassword())
        {
            isExist = true;
            writeAuthAnswer(socket, true);
        }
    }

    if (!isExist)
    {
        writeAuthAnswer(socket, false);
    }
}

void ConnectionHandler::readSignUpRequest(QTcpSocket *socket)
{
    QDataStream readStream(socket);
    UserData read;
    readStream >> read;
    qDebug() << "ConnectionHandler::readSignUpRequest()";
    read.getUsername();
    read.getPassword();

    read.setType(DataType::SignUpResponse);
    writeSignUpAnswer(socket, true);
    clients.insert(read, socket);
    //writeAboutNewConnection(read);
}

void ConnectionHandler::readAddContactRequest(QTcpSocket *socket)
{
    QDataStream readStream(socket);
    UserData read;
    readStream >> read;

}

void ConnectionHandler::writeAuthAnswer(QTcpSocket *socket, bool answer)
{
    qDebug() << "ConnectionHandler::writeAuthAnswer()";
    AuthAnswer ans(answer);
    write(socket, ans);
}

void ConnectionHandler::writeSignUpAnswer(QTcpSocket *socket, bool answer)
{
    qDebug() << "ConnectionHandler::writeSignUpAnswer()";
    SignUpAnswer ans(answer);
    write(socket, ans);
}

void ConnectionHandler::writeUserList(QTcpSocket *socket, const QList<UserData> &lst)
{
    UserList participants(lst);
    write(socket, participants);
}

void ConnectionHandler::writeAboutNewConnection(const UserData &connectedUser)
{
    for (auto &user : clients.values())
    {
       write(user, connectedUser);
    }
}

void ConnectionHandler::writeAboutUserDisconnection(const UserData &disconnectedUser)
{
    for (auto &user : clients.values())
    {
       write(user, disconnectedUser);
    }
}

void ConnectionHandler::sendOutMessage(QTcpSocket *sender, const Message &msg)
{
    auto senderUsername = clients.key(sender).getUsername();
    for (auto &user : clients.values())
        /* prevent writing message to its sender */
        if (clients.key(user).getUsername() != senderUsername)
            write(user, msg);
}

void ConnectionHandler::readMessage(QTcpSocket *socket)
{
    QDataStream readStream(socket);
    Message read;
    readStream >> read;
    sendOutMessage(socket, read);
}
