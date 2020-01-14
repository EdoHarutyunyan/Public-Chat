#pragma once

#include <QDataStream>

enum DataType : qint8
{
    None,
    Ping,
    AuthRequest,
    AuthResponse,
    SignUpRequest,
    SignUpResponse,
    AddContactRequest,
    AddContactResponse,
    UserListResponse,
    NewConnection,
    Disconnection,
    Msg
};

class BaseData
{
public:

    explicit BaseData() = default;

    friend QDataStream &operator >> (QDataStream &stream, BaseData &data);
    friend QDataStream &operator << (QDataStream &stream, const BaseData &data);

    virtual QDataStream& toStream(QDataStream &stream) const = 0;
    virtual QDataStream& fromStream(QDataStream &stream) = 0;
    virtual DataType type() const  = 0;

    virtual ~BaseData() = default;
};

QDataStream &operator >> (QDataStream &stream, BaseData &data);
QDataStream &operator << (QDataStream &stream, BaseData &data);
