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

    //创建地图场景和视图
    MapScene * scene = new MapScene(this);
    MapView * view = new MapView(scene,this);

    //视图填充界面中心
    this->setCentralWidget(view);

    //设置瓦片来源
    QSharedPointer<MapTileSourceOSM> osmTiles(new MapTileSourceOSM(MapTileSourceOSM::OSMTiles), &QObject::deleteLater);
    QSharedPointer<MapTileSourceGrid> gridTiles(new MapTileSourceGrid(), &QObject::deleteLater);
    QSharedPointer<MapTileSourceComposite> composite(new MapTileSourceComposite(), &QObject::deleteLater);
    composite->addSourceBottom(osmTiles);
    composite->addSourceTop(gridTiles);
    view->setTileSource(composite);

    //创建界面来管理瓦片来源
    MapTileSourceConfigurationWidget * tileConfigWidget = new MapTileSourceConfigurationWidget(composite.toWeakRef(),
                                                                                         this->ui->dockWidget);
    this->ui->dockWidget->setWidget(tileConfigWidget);
    delete this->ui->dockWidgetContents;

    this->ui->menuWindow->addAction(this->ui->dockWidget->toggleViewAction());
    this->ui->dockWidget->toggleViewAction()->setText("&Layers");

    view->setZoomLevel(12);
    view->centerOn(-112.202442, 40.9936234);

    // 在地图上绘制一个圆，用于演示MapObject
    // 此圆可以点击、拖拽，半径为5km
    MapObject * circle = new MapObjectCircle(5000, false, QColor(255, 0, 0, 100));
    circle->setLatitude(40.9936234);
    circle->setLongitude(-112.202442);
    scene->addObject(circle);
}

MainWindow::~MainWindow()
{
    delete ui;
}

