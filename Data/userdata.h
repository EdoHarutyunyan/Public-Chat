#pragma once

#include "basedata.h"

class UserData : public BaseData
{
public:
    UserData() = default;
    UserData(const QString &username);

    void setUsername(const QString &username);
    void setPassword(const QString &password);
    void setType(const DataType type);

    QString getUsername() const;
    QString getPassword() const;

    virtual QDataStream& toStream(QDataStream &stream) const override;
    virtual QDataStream& fromStream(QDataStream &stream) override;
    virtual DataType type() const override;

private:
    QString username;
    QString password;
    DataType currentType;
};

// Compare order for UserData if it is a key in QMap
bool operator <(const UserData &tmp1, const UserData &tmp2);

