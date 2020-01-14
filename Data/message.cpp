#include "message.h"

Message::Message(const QString &sender,
                 const QString &message,
                 const QPixmap &image) :
    sender(sender),
    message(message),
    image(image)
{
}

QDataStream& Message::fromStream(QDataStream &stream)
{
    stream >> sender;
    stream >> message;
    stream >> image;
    return stream;
}

DataType Message::type() const
{
    return DataType::Msg;
}

QDataStream& Message::toStream(QDataStream &stream) const
{
    stream << sender;
    stream << message;
    stream << image;
    return stream;
}

void Message::setMessage(const QString &message)
{
    this->message = message;
}

void Message::setSender(const QString &sender)
{
    this->sender = sender;
}

void Message::setImage(const QPixmap &image)
{
    this->image = image;
}

QString Message::getMessage() const
{
    return message;
}

QString Message::getSender() const
{
    return sender;
}

QPixmap Message::getImage() const
{
    return image;
}
