#pragma once

#include "basedata.h"
#include "userdata.h"

class AddContactAnswer : public BaseData
{
public:
    AddContactAnswer() = default;
    AddContactAnswer(const UserData& userData);

    virtual QDataStream& toStream(QDataStream &stream) const override;
    virtual QDataStream& fromStream(QDataStream &stream) override;
    virtual DataType type() const override;

    void setUserData(UserData&& userData);

    bool isExists() const;

private:
    UserData user;
};
