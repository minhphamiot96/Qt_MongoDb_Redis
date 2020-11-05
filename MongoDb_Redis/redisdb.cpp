#include "redisdb.h"

redisDb::redisDb(const QString& uri, const int& port, const QString& pass)
{
    // Connection options
    this->connection_options.host = uri.toStdString(); // Required
    this->connection_options.port = port; // The default port is 6379
    this->connection_options.password = pass.toStdString(); // Optional. No password is default
    this->connection_options.db = 1; // Optional. Use the 0th database by default
    this->connection_options.socket_timeout = std::chrono::milliseconds(200);
}

redisDb::~redisDb()
{

}

void redisDb::connect(void)
{
    qDebug() << "Received request connect with Redis";

    try {
        this->redis = new Redis(this->connection_options);
        emit this->connect_status(true);
    } catch (const Error &e) {
        // Error
        emit this->connect_status(false);
    }
}

void redisDb::receivedMsg(const QString& msg, const QString& request)
{
    qDebug() << "Message " << msg << " with reqest " << request;
    QJsonDocument jsd = QJsonDocument::fromJson(msg.toUtf8());
    QJsonObject jso = jsd.object();

    if ((true != jso.contains("NameDB")) || ("Redis" != jso["NameDB"].toString())) return;

    if ((true == jso.contains("item")) &&
        (true == jso.contains("_id")))
    {
        // Insert data
        if ("INSERT" == request)
        {
            qDebug() << "INSERT";
            this->redis->set(QString::number(jso["_id"].toInt()).toStdString(), QString(jsd.toJson().data()).remove('\n').toStdString());
        }
        // Query data
        else if ("QUERY" == request)
        {

        }
        // Delete data
        else if ("DELETE" == request)
        {
            qDebug() << "DELETE";
            // Request delete
            this->redis->del(QString::number(jso["_id"].toInt()).toStdString());
        }
        // Get data
        else if ("GET" == request)
        {
            auto ret = this->redis->get(QString::number(jso["_id"].toInt()).toStdString());
            if (ret) {
                std::cout << *ret << std::endl;
            }
            qDebug() << "GET: " << jso["_id"].toInt() << " with mess " << ret->data();

            emit this->sendDataInRedis(QString(ret->data()), request);
        }
    }
}
