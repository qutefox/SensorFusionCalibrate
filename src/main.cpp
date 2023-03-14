#include "widget/mainwindow.h"

#include "app/calibrate.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Calibrate c;

    MainWindow w;
    w.show();
    return a.exec();
}
