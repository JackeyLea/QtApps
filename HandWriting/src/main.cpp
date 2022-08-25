#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.resize(600,400);
    w.setWindowTitle(QObject::tr("Handwriting"));
    w.setWindowIcon(QIcon(":/resources/icons/pen.svg"));
    w.setStyleSheet(loadTheme("gray"));
    w.show();

    return a.exec();
}
