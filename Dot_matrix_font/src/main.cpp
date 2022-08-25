#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle(QObject::tr(""));
    w.setWindowIcon(QIcon(""));
    w.show();
    return a.exec();
}
