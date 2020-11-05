#include "mongodb.h"

mongoDb::mongoDb(QObject *parent) {

}

mongoDb::mongoDb(const QString& uri, const int& port)
{
    if (uri.isEmpty()) {
        qDebug() << "Addr input is empty";
        return;
    }

    this->uriMongoDb = QString("mongodb:://") + uri + ":" + QString::number(port, 10);
    qDebug() << this->uriMongoDb;
}

mongoDb::~mongoDb()
{

}

void mongoDb::connect(void)
{
    qDebug() << "Received requests connection";
//    std::cout << this->uriMongoDb.toStdString() << std::endl;
    if (!this->conn)
    {
        this->conn = mongocxx::client { mongocxx::uri {
                                            // Local mongoDb
                                        },
                                        options::client()
                                    };
        if (!this->conn) {
            qDebug() << "Counld not connect to DB";
            emit this->connect_status(false);
        }
        else {
            qDebug() << "Connected to DB!";
            this->db = (mongocxx::database) conn["MyDB"];
            emit this->connect_status(true);
        }
    }
}

void mongoDb::receivedMsg(const QString& msg, const QString& request)
{
    qDebug() << "Message " << msg << " with reqest " << request;
    QJsonDocument jsd = QJsonDocument::fromJson(msg.toUtf8());
    QJsonObject jso = jsd.object();


    if ((true == jso.contains("item")) &&
        (true == jso.contains("qty")) &&
        (true == jso.contains("tags")) &&
        (true == jso.contains("dim_cm")))
    {
        if ((true != jso.contains("NameDB")) || ("MongoDb" != jso["NameDB"].toString())) return;

        // Insert data
        if ("INSERT" == request)
        {
            qDebug() << "INSERT";
            if (MongoDb_Status_t::MongoDb_OK == this->insert_Db(jsd)) {
                emit this->request_status(request);
            }
        }
        // Query data
        else if ("QUERY" == request)
        {
            if (MongoDb_Status_t::MongoDb_OK == this->query_Db(jsd)) {
                emit this->request_status(request);
            }
        }
        // Delete data
        else if ("DELETE" == request)
        {
            if (MongoDb_Status_t::MongoDb_OK == this->delete_Db(jsd)) {
                emit this->request_status(request);
            }
        }
    }
    else if (("MongoDb" == msg) && ("SHOW" == request)) {
        qDebug() << "Received requests show all data on MongoDB";
        this->show_Db();
    }
}

void mongoDb::show_Db()
{
    if (!this->conn) return;

    // Show data in MongoDb
    for (auto&&doc : this->db["inventory"].find({})) {
            std::cout << bsoncxx::to_json(doc) << std::endl;
            emit this->sendDataInMongoDB(QString(bsoncxx::to_json(doc).data()), "SHOW");
    }
}

MongoDb_Status_t mongoDb::drop_Db()
{
    this->db["inventory"].drop();
    return MongoDb_Status_t::MongoDb_OK;
}

MongoDb_Status_t mongoDb::insert_Db(const QJsonDocument& jso)
{
    using bsoncxx::builder::core;
    using bsoncxx::builder::basic::kvp;
    using bsoncxx::builder::basic::make_array;
    using bsoncxx::builder::basic::make_document;
    using bsoncxx::to_json;

    QJsonObject jsonObj = jso.object();

    try {
        this->db["inventory"].insert_one( make_document(kvp("item", jsonObj["item"].toString().toStdString()),
                                                        kvp("qty", jsonObj["qty"].toInt()),
                                                        kvp("tags", (jsonObj["tags"].toString().remove('\n').toStdString())),
                                                        kvp("dim_cm", (jsonObj["dim_cm"].toString().remove('\n').toStdString()))));
    } catch (const std::logic_error& e) {
        qDebug() << e.what();
        return MongoDb_Status_t::MongoDb_ERROR;
    }

    return  MongoDb_Status_t::MongoDb_OK;
}

MongoDb_Status_t mongoDb::query_Db(const QJsonDocument &jso)
{
    using bsoncxx::builder::basic::kvp;
    using bsoncxx::builder::basic::make_array;
    using bsoncxx::builder::basic::make_document;
    using bsoncxx::to_json;

    QJsonObject jsonObj = jso.object();

    try {
        this->db["inventory"].insert_one( make_document(kvp("item", jsonObj["item"].toString().toStdString()),
                                                        kvp("qty", jsonObj["qty"].toInt()),
                                                        kvp("tags", (jsonObj["tags"].toString().remove('\n').toStdString())),
                                                        kvp("dim_cm", (jsonObj["dim_cm"].toString().remove('\n').toStdString()))));
    } catch (const std::logic_error& e) {
        qDebug() << e.what();
        return MongoDb_Status_t::MongoDb_ERROR;
    }

    return  MongoDb_Status_t::MongoDb_OK;
}

MongoDb_Status_t mongoDb::delete_Db(const QJsonDocument &jso)
{
    using bsoncxx::builder::basic::kvp;
    using bsoncxx::builder::basic::make_array;
    using bsoncxx::builder::basic::make_document;
    using bsoncxx::to_json;

    QJsonObject jsonObj = jso.object();

    try {
        this->db["inventory"].insert_one( make_document(kvp("item", jsonObj["item"].toString().toStdString()),
                                                        kvp("qty", jsonObj["qty"].toInt()),
                                                        kvp("tags", (jsonObj["tags"].toString().remove('\n').toStdString())),
                                                        kvp("dim_cm", (jsonObj["dim_cm"].toString().remove('\n').toStdString()))));
    } catch (const std::logic_error& e) {
        qDebug() << e.what();
        return MongoDb_Status_t::MongoDb_ERROR;
    }

    return  MongoDb_Status_t::MongoDb_OK;
}
