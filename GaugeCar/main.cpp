#include "gaugecar.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GaugeCar w;
    w.show();
    return a.exec();
}
