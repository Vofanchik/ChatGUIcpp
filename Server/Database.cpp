#include "Database.h"

namespace {
  const std::string ALL = "ALL";
}

Database::Database(): database_(QSqlDatabase::addDatabase("QSQLITE"))
{
  database_.setDatabaseName("./DataBase.db");
  database_.open();
  query_ = new QSqlQuery(database_);
  query_->exec("CREATE TABLE Users(Login TEXT, PasswordHash TEXT, Ban BOOL);");
  query_->exec("CREATE TABLE Messages(Sender TEXT, Receiver TEXT, Message TEXT);");
}

bool Database::isPasswordCorrect(const std::string& login, const std::string& passwordHash) const
{
  if (login.empty() || passwordHash.empty()){
    return false;
  }
  if (!isUserRegistered(login)){
    return false;
  }
  auto request = "Select PasswordHash from Users WHERE Login = '" +
                  QString::fromStdString(login) + "';";
  query_->exec(request);

  QString databasePasswordHash;
  while (query_->next()) {
    databasePasswordHash = query_->value(0).toString();
  }

  if (databasePasswordHash != QString::fromStdString(passwordHash) ){
    return false;
  }

  return true;
}



bool Database::isUserRegistered(const std::string& login) const
{
  if (login.empty()){
    return false;
  }

  auto request = "Select * from Users WHERE Login = '" +
                  QString::fromStdString(login) + "';";
  query_->exec(request);
  if (!query_->next()){
    return false;
  }

  return true;
}

bool Database::addUser(const std::string& login, const std::string& passwordHash)
{
  if (login.empty() || passwordHash.empty()){
    return false;
  }

  if (isUserRegistered(login)) {
    return false;
  }
  auto request = "INSERT INTO Users(Login, PasswordHash, Ban) VALUES('" +
                  QString::fromStdString(login) + "', '" +
                  QString::fromStdString(passwordHash) + "', false);";
  query_->exec(request);
  emit updateDatabase();
  return true;
}



bool Database::addChat(const std::string& sender, const std::string& message)
{
  if (message.empty()){
    return false;
  }
  if (!isUserRegistered(sender)) {
    return false;
  }
  if (getIsBanned(sender)){
    return false;
  }
  auto request = "INSERT INTO Messages(Sender, Receiver, Message) VALUES('" +
                  QString::fromStdString(sender) + "', '" +
                  QString::fromStdString(ALL) + "', '" +
                  QString::fromStdString(message) + "');";
  query_->exec(request);
  emit updateDatabase();
  return true;
}

bool Database::addPrivateChat(const std::string& sender, const std::string& receiver, const std::string& message)
{
  if (message.empty()){
    return false;
  }

  if (!isUserRegistered(sender)) {
    return false;
  }

  if (!isUserRegistered(receiver)) {
    return false;
  }

  if (getIsBanned(sender)){
    return false;
  }

  auto request = "INSERT INTO Messages(Sender, Receiver, Message) VALUES('" +
                  QString::fromStdString(sender) + "', '" +
                  QString::fromStdString(receiver) + "', '" +
                  QString::fromStdString(message) + "');";
  query_->exec(request);

  emit updateDatabase();

  return true;
}



std::string Database::getChat() const
{
  auto request = "Select * from Messages WHERE Receiver = '" +
                  QString::fromStdString(ALL) + "';";
  query_->exec(request);

  std::string messages;
  while (query_->next()) {
    auto sender = query_->value(0).toString();
    auto receiver = query_->value(1).toString();
    auto text = query_->value(2).toString();

    auto line = "<" + sender + ">: " + text + "\n";
    messages.append(line.toStdString());
  }
  return messages;
}



std::string Database::getPrivateChat(const std::string& receiver) const
{
  auto request = "Select * from Messages WHERE Receiver = '" +
                  QString::fromStdString(receiver) + "' OR Sender = '" +
                  QString::fromStdString(receiver) + "' AND Receiver NOT IN ('" +
                  QString::fromStdString(ALL) + "');";

  query_->exec(request);

  std::string messages;
  while (query_->next()) {
    auto sender = query_->value(0).toString();
    auto addressee = query_->value(1).toString();
    auto text = query_->value(2).toString();

    auto line = "<" + sender + "> -> <" + addressee + ">: " + text + "\n";
    messages.append(line.toStdString());
  }

  return messages;
}



std::list<std::string> Database::getLogins() const
{
  query_->exec("Select * from Users;");

  std::list<std::string> logins;
  while (query_->next()) {
    auto login = query_->value(0).toString();
    logins.push_back(login.toStdString());
  }
  return logins;
}



bool Database::deleteUser(const std::string& login)
{
  if (login.empty()){
    return false;
  }

  if (!isUserRegistered(login)) {
    return false;
  }

  auto request = "DELETE FROM Messages WHERE Receiver = '" +
                  QString::fromStdString(login) + "';";
  query_->exec(request);

  request.clear();
  request = "DELETE FROM Users WHERE Login = '" +
            QString::fromStdString(login) + "';";
  query_->exec(request);

  emit updateDatabase();

  return true;
}



bool Database::deleteMessage(const std::string& message)
{
  if (message.empty()){
    return false;
  }

  auto request = "DELETE FROM Messages WHERE Message = '" +
                  QString::fromStdString(message) + "';";
  query_->exec(request);

  emit updateDatabase();

  return true;
}



void Database::switchBanUser(const std::string& login)
{
  if(getIsBanned(login)){
    auto request = "UPDATE Users SET Ban = false WHERE Login = '" +
              QString::fromStdString(login) + "';";
    query_->exec(request);
  }
  else{
    auto request = "UPDATE Users SET Ban = true WHERE Login = '" +
              QString::fromStdString(login) + "';";
    query_->exec(request);
  }

  emit updateDatabase();
}

QSqlDatabase* Database::getDatabase()
{
  return &database_;
}

bool Database::getIsBanned(const std::string& login) const
{
  if (login.empty()){
    return true;
  }
  if (!isUserRegistered(login)){
    return true;
  }

  auto request = "Select Ban from Users WHERE Login = '" +
                  QString::fromStdString(login) + "';";
  query_->exec(request);

  bool isBanned = true;
  while (query_->next()) {
    isBanned = query_->value(0).toBool();
  }
  return isBanned;
}
