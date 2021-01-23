#include "mainwindow.h"

#include <QApplication>
#include "login.h"
#include "title.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Login w;
    w.show();

    return a.exec();
}
