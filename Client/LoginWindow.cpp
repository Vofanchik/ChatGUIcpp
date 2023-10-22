#include "LoginWindow.h"
#include "ui_LoginWindow.h"

#include <QMessageBox>

#include "SHA_1_Wrapper.h"

LoginWindow::LoginWindow(QWidget* parent) :
  QDialog(parent), ui(new Ui::LoginWindow)
{
  ui->setupUi(this);
}



LoginWindow::~LoginWindow()
{
  delete ui;
}



void LoginWindow::setClient(Client* client)
{
  client_ = client;
}


void LoginWindow::handleServerResponse_IsPasswordCorrect()
{
  auto isCorrectPassword = client_->getResponse();
  disconnectNetworkSignal_IsPasswordCorrect();

  if (isCorrectPassword == "true"){
    auto login = ui->lineEdit_Login->text().toStdString();
    ui->lineEdit_Login->clear();
    ui->lineEdit_Password->clear();

    emit toChat(login);
  }
  else{
    QMessageBox::critical(this, "Error", "Password incorrect");
    return;
  }
}


void LoginWindow::on_pushButton_Registration_clicked()
{
  ui->lineEdit_Login->clear();
  ui->lineEdit_Password->clear();

  emit toRegistration();
}



void LoginWindow::on_buttonBox_accepted()
{
  auto login = ui->lineEdit_Login->text().toStdString();
  if (login.empty()){
    QString message = "Please type Login";
    QMessageBox::critical(this, "Error", message);
    return;
  }

  auto password = ui->lineEdit_Password->text().toStdString();
  if (password.empty()){
    QString message = "Please type Password";
    QMessageBox::critical(this, "Error", message);
    return;
  }


  auto hashPassword = sha_1::hash(password);
  connectNetworkSignal_IsPasswordCorrect();
  client_->requestIsPasswordCorrect(QString::fromStdString(login),
                                    QString::fromStdString(hashPassword));
}

void LoginWindow::on_buttonBox_rejected()
{
  emit close();
}


void LoginWindow::connectNetworkSignal_IsPasswordCorrect()
{
  connect(client_, &Client::dataReceived,
          this, &LoginWindow::handleServerResponse_IsPasswordCorrect);
}



void LoginWindow::disconnectNetworkSignal_IsPasswordCorrect()
{
  disconnect(client_, &Client::dataReceived,
              this, &LoginWindow::handleServerResponse_IsPasswordCorrect);
}
