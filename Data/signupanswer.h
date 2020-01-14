#pragma once

#include "basedata.h"

class SignUpAnswer : public BaseData
{
public:
    explicit SignUpAnswer(bool ans);
    SignUpAnswer() = default;

    virtual QDataStream& toStream(QDataStream &stream) const override;
    virtual QDataStream& fromStream(QDataStream &stream) override;
    virtual DataType type() const override;

    bool isSuccess() const;

    void setAnswer(bool ans);

private:
    bool answer;
};
