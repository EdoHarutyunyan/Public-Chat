#include "chatwidget.h"
#include "ui_chatwidget.h"

ChatWidget::ChatWidget(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::ChatWidget)
{
    ui->setupUi(this);
}

ChatWidget::~ChatWidget()
{
    delete ui;
}

void ChatWidget::addContact(const UserData &username)
{
    auto participant = new QListWidgetItem;
    participant->setText(username.getUsername());

    // Disable selecting participant in list widget
    participant->setFlags(participant->flags() & ~Qt::ItemIsSelectable);
    ui->Users->addItem(participant);
}

void ChatWidget::addMessage(const Message &msg)
{
    auto item = new QListWidgetItem;
    item->setText(msg.getSender() + ":" + msg.getMessage());

    // Disable selecting message in dialog
    item->setFlags(item->flags() & ~Qt::ItemIsSelectable);
    ui->Room->addItem(item);
}

void ChatWidget::addContacts(const UserList &lst)
{
    auto users = lst.getUsers();
    for (auto &user : users) this->addContact(user);
}

void ChatWidget::removeContact(const QString &username)
{
    auto itemToRemove = ui->Users->findItems(username, Qt::MatchFixedString).at(0);
    delete itemToRemove;
}

void ChatWidget::on_SendButton_clicked()
{
    auto text = ui->MessageLine->toPlainText();
    if (text.isEmpty()) return;

    /* todo: user can select font and color of his message */
    /* msg.setTextColor(...); msg.setFontColor(...); */

    Message msg("You", text);

    // If user has loaded any image, add it to Message data
    if (!bufferedImage.isNull()) msg.setImage(bufferedImage);

    // Add message to dialog widget
    addMessage(msg);

    // Clear message line and loaded image label
    ui->MessageLine->clear();

    // Send message to server
    emit messageSent(msg);

    // Clear buffered image
    bufferedImage = QPixmap();
}

void ChatWidget::on_loadImageButton_clicked()
{
    auto path = QFileDialog::getOpenFileName(this, tr("Open image"),
                                             "", tr("*.png *.jpg *.jpeg"));
    // Get image name from string
    auto imageName = path.split("/").last();

    // Load image to UI
    QImage img;
    img.load(path);
    bufferedImage.fromImage(img);
}
