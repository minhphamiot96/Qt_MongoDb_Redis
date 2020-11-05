#ifndef REDISDB_H
#define REDISDB_H

#include <iostream>

#include <QObject>
#include <QString>
#include <QJsonObject>
#include <QJsonDocument>

#include <sw/redis++/redis++.h>

#include <QDebug>

using namespace sw::redis;

class redisDb : public QObject
{
    Q_OBJECT
public:
    explicit redisDb(const QString& uri, const int& port, const QString& pass = "");
    ~redisDb();

public slots:
    // Request connect
    void connect(void);
    // Receive request
    void receivedMsg(const QString& msg, const QString& request);

signals:
    // Inform status connection
    void connect_status(const bool& status);

    // Send data
    void sendDataInRedis(const QString& msg, const QString& request);

private:
    ConnectionOptions connection_options;

    // Redis client
    Redis *redis;
};

#endif // REDISDB_H
