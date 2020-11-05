#ifndef MONGODB_H
#define MONGODB_H

#include <vector>
#include <iostream>
#include <cstdint>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

#ifndef  Q_MOC_RUN
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>
#include <bsoncxx/json.hpp>
#include <bsoncxx/types.hpp>

#include <mongocxx/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/database.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/collection.hpp>
#endif

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;
using bsoncxx::to_json;
using namespace mongocxx;

typedef enum {
    MongoDb_ERROR = -1,
    MongoDb_OK = 0,
} MongoDb_Status_t;

class mongoDb : public QObject
{
    Q_OBJECT
public:
    explicit mongoDb(QObject *parent = nullptr);
    mongoDb(const QString& uri, const int& port);
    ~mongoDb();

    MongoDb_Status_t query_Db(const QJsonDocument& jso);
    MongoDb_Status_t insert_Db(const QJsonDocument& jso);
    MongoDb_Status_t delete_Db(const QJsonDocument& jso);
    MongoDb_Status_t drop_Db();

public slots:
    // Receive request connec mongo
    void connect(void);
    // Show data on DB
    void show_Db(void);
    // Handle msg received
    void receivedMsg(const QString& msg, const QString& request);

signals:
    // Connect
    void connect_status(const bool& status);
    // Response
    void request_status(const QString& request);
    // Response data in MongoDb
    void sendDataInMongoDB(const QString& msg, const QString& request);

private:
    // Uri of MongoDB
    QString uriMongoDb;
    // Database
    mongocxx::instance inst{};
    mongocxx::database db;
    mongocxx::client   conn;
};

#endif // MONGODB_H
