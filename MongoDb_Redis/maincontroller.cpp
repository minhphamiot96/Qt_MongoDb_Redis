#include "maincontroller.h"

maincontroller::maincontroller()
{
    // MongoDb
//    this->m_mongoDb = new mongoDb("127.0.0.1", 27017);

    // Ui
    this->m_ui = new MainWindow();
}

maincontroller::~maincontroller()
{
    // Do nothing
}

void maincontroller::start()
{
    // Show
    this->m_ui->show();

//    if (MongoDb_Status_t::MongoDb_OK != this->m_mongoDb->connect())
//    {
//        qDebug() << "Please check link connect with mongoDb";
//    }
}

void maincontroller::signalsAndSlots(void)
{
//    connect(this->m_ui->ui.)
}
