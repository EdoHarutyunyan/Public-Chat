#include "signupanswer.h"

SignUpAnswer::SignUpAnswer(bool ans) :
    answer(ans)
{
}

QDataStream& SignUpAnswer::toStream(QDataStream &stream) const
{
    stream << answer;
    return stream;
}

QDataStream& SignUpAnswer::fromStream(QDataStream &stream)
{
    stream >> answer;
    return stream;
}

DataType SignUpAnswer::type() const
{
    return DataType::SignUpResponse;
}

bool SignUpAnswer::isSuccess() const
{
    return answer;
}

void SignUpAnswer::setAnswer(const bool ans)
{
    this->answer = ans;
}
