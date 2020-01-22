#include "addcontactanswer.h"

AddContactAnswer::AddContactAnswer(const UserData& userData) :
    user(userData)
{
}

QDataStream& AddContactAnswer::toStream(QDataStream &stream) const
{
    stream << user;
    return stream;
}

QDataStream& AddContactAnswer::fromStream(QDataStream &stream)
{
    stream >> user;
    return stream;
}

DataType AddContactAnswer::type() const
{
    return DataType::AddContactResponse;
}

bool AddContactAnswer::isExists() const
{
    return !user.getUsername().isEmpty();
}

void AddContactAnswer::setUserData(UserData&& userData)
{
    user = std::forward<UserData>(userData);
}
