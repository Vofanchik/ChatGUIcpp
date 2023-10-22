#include "ChatWindow.h"
#include "ui_ChatWindow.h"

#include <QMessageBox>

#include <QDialog>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QListWidget>


ChatWindow::ChatWindow(QWidget* parent) :
  QWidget(parent), ui(new Ui::ChatWindow)
{
  ui->setupUi(this);

  timer_ = new QTimer(this);
  connect(timer_, &QTimer::timeout,
          this, &ChatWindow::updateMessages);
}



ChatWindow::~ChatWindow()
{
  delete ui;
}



void ChatWindow::setClient(Client* client)
{
  client_ = client;
}



void ChatWindow::connectNetworkSignal_UpdateMessage()
{
  timer_->start(50);
}



void ChatWindow::setUserLogin(const std::string& userLogin)
{
  userLogin_ = userLogin;
}



void ChatWindow::handleServerResponse_GetUsersList()
{
  auto response = client_->getResponse();
  disconnectNetworkSignal_GetUserList();
  QStringList userList = response.split("|", QString::KeepEmptyParts);

  QDialog listUsersDialog(this);
  listUsersDialog.setModal(true);
  auto layout = new QVBoxLayout();
  listUsersDialog.setLayout(layout);
  auto userListWidget = new QListWidget(&listUsersDialog);
  layout->addWidget(userListWidget);
  auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok |
                                        QDialogButtonBox::Cancel,
                                        &listUsersDialog);
  layout->addWidget(buttonBox);
  connect(buttonBox, &QDialogButtonBox::accepted,
          &listUsersDialog, &QDialog::accept);
  connect(buttonBox, &QDialogButtonBox::rejected,
          &listUsersDialog, &QDialog::reject);

  for (auto user : userList){
    userListWidget->addItem(user);
  }
  userListWidget->setCurrentRow(0);

  auto result = listUsersDialog.exec();
  if (result == QDialog::Accepted && userListWidget->currentItem()){
    auto receiver = userListWidget->currentItem()->text();
    auto message = ui->lineEdit_Message->text();
    client_->sendPrivateMessage(QString::fromStdString(userLogin_),
                                receiver,
                                message);
  }
  ui->lineEdit_Message->clear();
}



void ChatWindow::handleServerResponse_GetChat()
{
  chat_ = client_->getResponse();
  disconnectNetworkSignal_GetChat();
  connectNetworkSignal_GetPrivateChat();
  client_->requestPrivateMessage(QString::fromStdString(userLogin_));
}



void ChatWindow::handleServerResponse_GetPrivateChat()
{
  privateChat_ = client_->getResponse();
  disconnectNetworkSignal_GetPrivateChat();
  displayMessages();
}


void ChatWindow::on_pushButton_SendToAll_clicked()
{
  auto message = ui->lineEdit_Message->text().toStdString();
  if (message.empty()){
    QMessageBox::critical(this, "Error", "Please type message");
    return;
  }

  client_->sendCommonMessage(QString::fromStdString(userLogin_),
                             QString::fromStdString(message));
  ui->lineEdit_Message->clear();
}



void ChatWindow::on_pushButton_SendPrivate_clicked()
{
  auto message = ui->lineEdit_Message->text().toStdString();
  if (message.empty()){
    QMessageBox::critical(this, "Error", "Please type message");
    return;
  }

  connectNetworkSignal_GetUserList();
  client_->requestUserLogins();
}



void ChatWindow::on_pushButton_Logout_clicked()
{
  disconnectNetworkSignal_UpdateMessage();
  disconnectNetworkSignal_GetChat();
  disconnectNetworkSignal_GetPrivateChat();

  ui->lineEdit_Message->clear();

  emit toLogin();
}



void ChatWindow::on_pushButton_DeleteAccount_clicked()
{
  disconnectNetworkSignal_UpdateMessage();
  disconnectNetworkSignal_GetChat();
  disconnectNetworkSignal_GetPrivateChat();

  ui->lineEdit_Message->clear();

  client_->requestRemoveUser(QString::fromStdString(userLogin_));

  emit toLogin();
}

void ChatWindow::connectNetworkSignal_GetUserList()
{
  disconnectNetworkSignal_UpdateMessage();
  connect(client_, &Client::dataReceived,
          this, &ChatWindow::handleServerResponse_GetUsersList);
}

void ChatWindow::disconnectNetworkSignal_GetUserList()
{
  disconnect(client_, &Client::dataReceived,
            this, &ChatWindow::handleServerResponse_GetUsersList);
  connectNetworkSignal_UpdateMessage();
}



void ChatWindow::disconnectNetworkSignal_UpdateMessage()
{
  timer_->stop();
}



void ChatWindow::connectNetworkSignal_GetChat()
{
  connect(client_, &Client::dataReceived,
          this, &ChatWindow::handleServerResponse_GetChat);
}



void ChatWindow::disconnectNetworkSignal_GetChat()
{
  disconnect(client_, &Client::dataReceived,
              this, &ChatWindow::handleServerResponse_GetChat);
}



void ChatWindow::connectNetworkSignal_GetPrivateChat()
{
  connect(client_, &Client::dataReceived,
          this, &ChatWindow::handleServerResponse_GetPrivateChat);
}



void ChatWindow::disconnectNetworkSignal_GetPrivateChat()
{
  disconnect(client_, &Client::dataReceived,
            this, &ChatWindow::handleServerResponse_GetPrivateChat);
}



void ChatWindow::updateMessages()
{
  connectNetworkSignal_GetChat();
  client_->requestCommonMessage();
}



void ChatWindow::displayMessages()
{

  if (ui->textBrowser_AllMessages->toPlainText() != chat_){
    ui->textBrowser_AllMessages->setText(chat_);
  }
  if (ui->textBrowser_PrivateMessages->toPlainText() != privateChat_){
    ui->textBrowser_PrivateMessages->setText(privateChat_);
  }

  chat_.clear();
  privateChat_.clear();
}
