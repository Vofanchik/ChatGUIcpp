#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Server.h"
#include "Database.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow{
  Q_OBJECT

  public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

  private slots:
    void on_pushButton_DeleteUser_clicked();
    void on_pushButton_DeleteMessage_clicked();
    void on_pushButton_ban_unban_clicked();
    void on_tableView_Users_clicked(const QModelIndex& index);
    void on_tableView_Messages_clicked(const QModelIndex &index);
    void updateViews();

private:
    Ui::MainWindow* ui;
    Server* server_;
    Database* database_;
    QSqlTableModel* modelUsers_;
    QSqlTableModel* modelMessages_;
    int activeUserRow_;
    int activeMessageRow_;
};
#endif
