#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "mongodb.h"
#include "redisdb.h"

#include <QMainWindow>
#include <QtWidgets>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void connectSignalSlot(void);
public slots:
    // Connection MongoDb
    void connection_Mongo_status(const bool& status);
    // Connection Redis
    void connection_Redis_status(const bool& status);
    // Show data
    void receivedMsgHandle(const QString& msg, const QString& request);
    // Button Show
    void showButton();
    // Send Button
    void sendButton();

signals:
    // Request connect
    void connectMongoDb(const bool& status);
    // Send request
    void sendRequest(const QString& msg, const QString& request);

private:
    // MongoDb class
    mongoDb *m_mongoDb;
    // Redis class
    redisDb *m_redisDb;

    // Ui
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
