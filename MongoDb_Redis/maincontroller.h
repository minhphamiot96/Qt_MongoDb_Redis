#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include "mongodb.h"
#include "mainwindow.h"

class maincontroller
{
public:
    explicit maincontroller();
    ~maincontroller();

    void start();

    // Connect signal and slot
    void signalsAndSlots(void);

private:
    // MongoDb class
    mongoDb* m_mongoDb;

    //  MainWindow
    MainWindow* m_ui;
};

#endif // MAINCONTROLLER_H
