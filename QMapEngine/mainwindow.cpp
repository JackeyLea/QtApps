#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "mapview.h"
#include "mapscene.h"
#include "maptilesourcegrid.h"
#include "maptilesourceosm.h"
#include "maptilesourcecomposite.h"
#include "maptilesourceconfigurationwidget.h"
#include "mapobjectcircle.h"
#include "mapobjectpolygon.h"

#include <QSharedPointer>
#include <QtDebug>
#include <QThread>
#include <QImage>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Setup the MapGraphics scene and view
    MapScene * scene = new MapScene(this);
    MapView * view = new MapView(scene,this);

    //The view will be our central widget
    this->setCentralWidget(view);

    //Setup some tile sources
    QSharedPointer<MapTileSourceOSM> osmTiles(new MapTileSourceOSM(MapTileSourceOSM::OSMTiles), &QObject::deleteLater);
    QSharedPointer<MapTileSourceGrid> gridTiles(new MapTileSourceGrid(), &QObject::deleteLater);
    QSharedPointer<MapTileSourceComposite> composite(new MapTileSourceComposite(), &QObject::deleteLater);
    composite->addSourceBottom(osmTiles);
    composite->addSourceTop(gridTiles);
    view->setTileSource(composite);

    //Create a widget in the dock that lets us configure tile source layers
    MapTileSourceConfigurationWidget * tileConfigWidget = new MapTileSourceConfigurationWidget(composite.toWeakRef(),
                                                                                         this->ui->dockWidget);
    this->ui->dockWidget->setWidget(tileConfigWidget);
    delete this->ui->dockWidgetContents;

    this->ui->menuWindow->addAction(this->ui->dockWidget->toggleViewAction());
    this->ui->dockWidget->toggleViewAction()->setText("&Layers");

    view->setZoomLevel(12);
    view->centerOn(-112.202442, 40.9936234);

    // Create a circle on the map to demonstrate MapGraphicsObject a bit
    // The circle can be clicked/dragged around and should be ~5km in radius
    MapObject * circle = new MapObjectCircle(5000, false, QColor(255, 0, 0, 100));
    circle->setLatitude(40.9936234);
    circle->setLongitude(-112.202442);
    scene->addObject(circle);
}

MainWindow::~MainWindow()
{
    delete ui;
}

