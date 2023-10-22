#include "RequestHandler.h"


namespace{
  enum Command{
    IS_PASSWORD_CORRECT,
    IS_USER_REGISTERED,
    ADD_USER,
    ADD_CHAT,
    ADD_PRIVATE_CHAT,
    GET_CHAT,
    GET_PRIVATE_CHAT,
    GET_USER_LOGINS,
    DELETE_USER
  };
}

RequestHandler::RequestHandler(Database* database): database_(database)
{

}

QString RequestHandler::handle(int commandType, const QString& args)
{
  QString response;
  switch(commandType){
    case IS_PASSWORD_CORRECT:
      response = isPasswordCorrect(args);
      break;
    case IS_USER_REGISTERED:
      response = isUserRegistered(args);
      break;
    case ADD_USER:
      response = addUser(args);
      break;
    case ADD_CHAT:
      response = addChat(args);
      break;
    case ADD_PRIVATE_CHAT:
      response = addPrivateChat(args);
      break;
    case GET_CHAT:
      response = getChat();
      break;
    case GET_PRIVATE_CHAT:
      response = getPrivateChat(args);
      break;
    case GET_USER_LOGINS:
      response = getUserLogins();
      break;
    case DELETE_USER:
      response = deleteUser(args);
      break;
    default:
      break;
  }
  return response;
}


QString RequestHandler::isPasswordCorrect(const QString& args)
{
  QStringList listArgs = args.split("|", QString::KeepEmptyParts);
  const std::string login = listArgs.at(0).toStdString();
  const std::string passwordHash = listArgs.at(1).toStdString();

  auto isOk = database_->isPasswordCorrect(login, passwordHash);
  QString response = "false";
  if (isOk){
    response = "true";
  }
  return response;
}



QString RequestHandler::isUserRegistered(const QString& args)
{
  QStringList listArgs = args.split("|", QString::KeepEmptyParts);
  const std::string login = listArgs.at(0).toStdString();
  auto isOk = database_->isUserRegistered(login);
  QString response = "false";
  if (isOk){
    response = "true";
  }
  return response;
}



QString RequestHandler::addUser(const QString& args)
{
  QStringList listArgs = args.split("|", QString::KeepEmptyParts);
  const std::string login = listArgs.at(0).toStdString();
  const std::string passwordHash = listArgs.at(1).toStdString();

  auto isOk = database_->addUser(login, passwordHash);
  QString response = "false";
  if (isOk){
    response = "true";
  }
  return response;
}



QString RequestHandler::addChat(const QString& args)
{
  QStringList listArgs = args.split("|", QString::KeepEmptyParts);
  const std::string sender = listArgs.at(0).toStdString();
  const std::string message = listArgs.at(1).toStdString();

  auto isOk = database_->addChat(sender, message);
  QString response = "false";
  if (isOk){
    response = "true";
  }
  return response;
}



QString RequestHandler::addPrivateChat(const QString& args)
{
  QStringList listArgs = args.split("|", QString::KeepEmptyParts);
  const std::string sender = listArgs.at(0).toStdString();
  const std::string receiver = listArgs.at(1).toStdString();
  const std::string message = listArgs.at(2).toStdString();

  auto isOk = database_->addPrivateChat(sender, receiver, message);
  QString response = "false";
  if (isOk){
    response = "true";
  }
  return response;
}



QString RequestHandler::getChat()
{
  auto messages = database_->getChat();
  auto response = QString::fromStdString(messages);
  return response;
}



QString RequestHandler::getPrivateChat(const QString& args)
{
  QStringList listArgs = args.split("|", QString::KeepEmptyParts);
  const std::string receiver = listArgs.at(0).toStdString();

  auto messages = database_->getPrivateChat(receiver);
  auto response = QString::fromStdString(messages);
  return response;
}



QString RequestHandler::getUserLogins()
{
  auto users = database_->getLogins();
  QString response;
  for (const auto& user : users){
    response += QString::fromStdString(user) + "|";
  }
  return response;
}



QString RequestHandler::deleteUser(const QString& args)
{
  QStringList listArgs = args.split("|", QString::KeepEmptyParts);
  const std::string login = listArgs.at(0).toStdString();

  auto isOk = database_->deleteUser(login);
  QString response = "false";
  if (isOk){
    response = "true";
  }

  return response;
}
