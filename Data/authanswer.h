#pragma once

#include "basedata.h"

class AuthAnswer : public BaseData
{
public:
    AuthAnswer(bool answer);
    AuthAnswer() = default;

    virtual QDataStream& toStream(QDataStream &stream) const override;
    virtual QDataStream& fromStream(QDataStream &stream) override;
    virtual DataType type() const override;

    bool isSigned() const;

    void setSigned(bool answer);

private:
    bool answer;
};
