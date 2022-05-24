#include "gaugecar.h"
#include "gaugepercent.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GaugePercent w;
    w.show();
    return a.exec();
}
