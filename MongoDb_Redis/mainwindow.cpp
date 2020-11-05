#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->ui->comboBox_item->addItems(QStringList{"Item1", "Item2", "Item3", "Item4"});
    this->ui->Database_Select->addItems(QStringList{"MongoDb", "Redis"});
    this->ui->Function_DB->addItems(QStringList{"INSERT", "QUERY", "DELETE"});

    // MongoDb
    this->m_mongoDb = new mongoDb("127.0.0.1", 27017);

    // RedisDb
    this->m_redisDb = new redisDb("127.0.0.1", 6379);

    // Connect signal
    this->connectSignalSlot();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connectSignalSlot(void)
{
    if ((nullptr == this->m_mongoDb) && (nullptr == this->ui)) return;

    QObject::connect(this->ui->MongoDb_button,
                     &QAbstractButton::pressed,
                     this->m_mongoDb,
                     &mongoDb::connect);

    QObject::connect(this->m_mongoDb,
                     &mongoDb::connect_status,
                     this,
                     &MainWindow::connection_Mongo_status
                     );

    QObject::connect(this->ui->Show_Button,
                     &QAbstractButton::pressed,
                     this,
                     &MainWindow::showButton);

    QObject::connect(this->m_mongoDb,
                     &mongoDb::sendDataInMongoDB,
                     this,
                     &MainWindow::receivedMsgHandle);
    QObject::connect(this->m_redisDb,
                     &redisDb::sendDataInRedis,
                     this,
                     &MainWindow::receivedMsgHandle);

    QObject::connect(this->ui->Redis_button,
                     &QAbstractButton::pressed,
                     this->m_redisDb,
                     &redisDb::connect);

    QObject::connect(this->m_redisDb,
                     &redisDb::connect_status,
                     this,
                     &MainWindow::connection_Redis_status);

    QObject::connect(this->ui->Send_button,
                     &QAbstractButton::pressed,
                     this,
                     &MainWindow::sendButton);

    QObject::connect(this,
                     &MainWindow::sendRequest,
                     this->m_mongoDb,
                     &mongoDb::receivedMsg);

    QObject::connect(this,
                     &MainWindow::sendRequest,
                     this->m_redisDb,
                     &redisDb::receivedMsg);
}

void MainWindow::showButton()
{
    QJsonObject jso;
    jso["item"] = QString(this->ui->comboBox_item->currentText().data());
    jso["_id"] = this->ui->lineEdit_id->text().toInt();
    jso["NameDB"] = QString(this->ui->Database_Select->currentText().data());
    QJsonDocument jsd(jso);

    if ("MongoDb" == this->ui->Database_Select->currentText().toStdString())
    {
        emit this->sendRequest(QString(this->ui->Database_Select->currentText().data()), "SHOW");
    }
    else if ("Redis" == this->ui->Database_Select->currentText().toStdString())
    {
        emit this->sendRequest(QString(jsd.toJson().data()).remove('\n'), "GET");
    }
}

void MainWindow::sendButton()
{
    qDebug() << "Send button pressed";

    QJsonObject jso;
    jso["item"] = QString(this->ui->comboBox_item->currentText().data());
    jso["qty"] = this->ui->lineEdit_qty->text().toInt();
    jso["tags"] = QString(this->ui->lineEdit_tags->text().toStdString().data());
    jso["dim_cm"] = QString(this->ui->lineEdit_dsim->text().toStdString().data());
    jso["_id"] = this->ui->lineEdit_id->text().toInt();
    jso["NameDB"] = QString(this->ui->Database_Select->currentText().data());

    QJsonDocument jsd(jso);

    if ("MongoDb" == this->ui->Database_Select->currentText().toStdString())
    {
        emit this->sendRequest(QString(jsd.toJson().data()).remove('\n'), QString(this->ui->Function_DB->currentText().data()));
    }
    else if ("Redis" == this->ui->Database_Select->currentText().toStdString())
    {
        emit this->sendRequest(QString(jsd.toJson().data()).remove('\n'), QString(this->ui->Function_DB->currentText().data()));
    }
}

void MainWindow::connection_Mongo_status(const bool& status)
{
    if (true == status) {
        this->ui->MongoDb_button->setText("Connected");
    }
    else
    {
        this->ui->MongoDb_button->setText("Disconnected");
    }
}

void MainWindow::connection_Redis_status(const bool& status)
{
    if (true == status) {
        this->ui->Redis_button->setText("Connected");
    }
    else {
        this->ui->Redis_button->setText("Disconnected");
    }
}

void MainWindow::receivedMsgHandle(const QString& msg, const QString& request)
{
    qDebug() << "Mess received: " << msg << " with request " << request;
    if ((request == "SHOW") || (request == "GET")) {
        this->ui->textBrowser_show->append(msg);
    }
}
