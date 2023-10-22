#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>


class Database: public QObject{
  Q_OBJECT

  public:
    Database();
    bool isPasswordCorrect(const std::string& login,
                           const std::string& passwordHash) const;
    bool isUserRegistered(const std::string& login) const;
    bool addUser(const std::string& login,
                  const std::string& passwordHash);
    bool addChat(const std::string& sender,
                          const std::string& message);
    bool addPrivateChat(const std::string& sender,
                            const std::string& receiver,
                            const std::string& message);
    std::string getChat() const;
    std::string getPrivateChat(const std::string& receiver)const ;
    std::list<std::string> getLogins() const;
    bool deleteUser(const std::string& login);
    bool deleteMessage(const std::string& message);
    void switchBanUser(const std::string& login);
    QSqlDatabase* getDatabase();

  signals:
    void updateDatabase();

  private:
    bool getIsBanned(const std::string& login) const;

    QSqlDatabase database_;
    QSqlQuery* query_;
};

#endif
