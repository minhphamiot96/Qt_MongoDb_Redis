#include "maincontroller.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Start project
    maincontroller* m_maincontroller = new maincontroller();
    m_maincontroller->start();

    return a.exec();
}
