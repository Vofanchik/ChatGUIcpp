#ifndef REQUESTHANDLER_H
#define REQUESTHANDLER_H

#include <QString>
#include "Database.h"

class RequestHandler{
  public:
    RequestHandler() = delete;
    explicit RequestHandler(Database* database);
    QString handle(int commandType, const QString& args);

  private:
    QString isPasswordCorrect(const QString& args);
    QString isUserRegistered(const QString& args);
    QString addUser(const QString& args);
    QString addChat(const QString& args);
    QString addPrivateChat(const QString& args);
    QString getChat();
    QString getPrivateChat(const QString& args);
    QString getUserLogins();
    QString deleteUser(const QString& args);

    Database* database_;
};

#endif
