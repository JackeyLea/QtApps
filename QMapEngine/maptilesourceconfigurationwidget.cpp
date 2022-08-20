#include "maptilesourceconfigurationwidget.h"
#include "ui_maptilesourceconfigurationwidget.h"

#include "maptilelayerlistmodel.h"
#include "maptilesourcedelegate.h"

#include "maptilesourceosm.h"

#include <QtDebug>
#include <QMenu>

MapTileSourceConfigurationWidget::MapTileSourceConfigurationWidget(QWeakPointer<MapTileSourceComposite> composite, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MapTileSourceConfigurationWidget),
    _composite(composite)
{
    ui->setupUi(this);

        this->init();
}

MapTileSourceConfigurationWidget::~MapTileSourceConfigurationWidget()
{
    delete ui;
}

void MapTileSourceConfigurationWidget::setComposite(QWeakPointer<MapTileSourceComposite> nComposite)
{
    _composite = nComposite;
    this->init();
}

void MapTileSourceConfigurationWidget::handleCurrentSelectionChanged(QModelIndex current, QModelIndex previous)
{
    Q_UNUSED(previous)
    bool enableGUI = current.isValid();

    this->ui->removeSourceButton->setEnabled(enableGUI);
    this->ui->opacitySlider->setEnabled(enableGUI);

    QSharedPointer<MapTileSourceComposite> strong = _composite.toStrongRef();
    if (strong.isNull())
        return;

    this->ui->moveDownButton->setEnabled(enableGUI &&  (strong->numSources()-1) > current.row());
    this->ui->moveUpButton->setEnabled(enableGUI && 0 < current.row());

    if (enableGUI)
    {
        qreal opacityFloat = strong->getOpacity(current.row());
        this->ui->opacitySlider->setValue(opacityFloat*100);
    }
}

void MapTileSourceConfigurationWidget::handleCompositeChange()
{
    QItemSelectionModel * selModel = this->ui->listView->selectionModel();
    QModelIndex index = selModel->currentIndex();

    QSharedPointer<MapTileSourceComposite> strong = _composite.toStrongRef();
    if (strong.isNull())
        return;

    qreal opacityFloat = strong->getOpacity(index.row());
    this->ui->opacitySlider->setValue(opacityFloat*100);
}

void MapTileSourceConfigurationWidget::addOSMTileLayer()
{
    QSharedPointer<MapTileSourceComposite> composite = _composite.toStrongRef();
    if (composite.isNull())
        return;

    QSharedPointer<MapTileSourceOSM> source(new MapTileSourceOSM(MapTileSourceOSM::OSMTiles));
    composite->addSourceTop(source);
}

void MapTileSourceConfigurationWidget::on_removeSourceButton_clicked()
{
    QItemSelectionModel * selModel = this->ui->listView->selectionModel();
    QModelIndex index = selModel->currentIndex();

    QSharedPointer<MapTileSourceComposite> strong = _composite.toStrongRef();
    if (strong.isNull())
        return;

    strong->removeSource(index.row());

    selModel->clear();
}

void MapTileSourceConfigurationWidget::on_opacitySlider_valueChanged(int value)
{
    QItemSelectionModel * selModel = this->ui->listView->selectionModel();
    QModelIndex index = selModel->currentIndex();

    if (!index.isValid())
        return;

    QSharedPointer<MapTileSourceComposite> strong = _composite.toStrongRef();
    if (strong.isNull())
        return;

    qreal opacityFloat = (qreal)value / 100.0;

    strong->setOpacity(index.row(),opacityFloat);
}

void MapTileSourceConfigurationWidget::on_moveDownButton_clicked()
{
    QItemSelectionModel * selModel = this->ui->listView->selectionModel();
    QModelIndex index = selModel->currentIndex();

    if (!index.isValid())
        return;

    QSharedPointer<MapTileSourceComposite> strong = _composite.toStrongRef();
    if (strong.isNull())
        return;

    int numberOfLayers = strong->numSources();
    int currentIndex = index.row();
    int desiredIndex = qMin<int>(numberOfLayers-1,currentIndex+1);
    strong->moveSource(currentIndex,desiredIndex);
    selModel->setCurrentIndex(selModel->model()->index(desiredIndex,0),
                              QItemSelectionModel::SelectCurrent);
}

void MapTileSourceConfigurationWidget::on_moveUpButton_clicked()
{
    QItemSelectionModel * selModel = this->ui->listView->selectionModel();
    QModelIndex index = selModel->currentIndex();

    if (!index.isValid())
        return;

    QSharedPointer<MapTileSourceComposite> strong = _composite.toStrongRef();
    if (strong.isNull())
        return;

    int currentIndex = index.row();
    int desiredIndex = qMax<int>(0,currentIndex-1);
    strong->moveSource(currentIndex,desiredIndex);
    selModel->setCurrentIndex(selModel->model()->index(desiredIndex,0),
                              QItemSelectionModel::SelectCurrent);
}

void MapTileSourceConfigurationWidget::init()
{
    //Create a fancy delegate for custom drawing of our list items
    MapTileSourceDelegate * delegato = new MapTileSourceDelegate(_composite,this);

    //Create the model that watches the CompositeTileSource
    MapTileLayerListModel * model = new MapTileLayerListModel(_composite,this);

    //Set the QListView to watch the model and use the delegate
    QAbstractItemModel * oldModel = this->ui->listView->model();
    QAbstractItemDelegate * oldDelegate = this->ui->listView->itemDelegate();

    this->ui->listView->setModel(model);
    this->ui->listView->setItemDelegate(delegato);

    if (oldModel != 0)
        delete oldModel;
    if (oldDelegate != 0)
        delete oldDelegate;

    QItemSelectionModel * selModel = this->ui->listView->selectionModel();
    connect(selModel,
            SIGNAL(currentChanged(QModelIndex,QModelIndex)),
            this,
            SLOT(handleCurrentSelectionChanged(QModelIndex,QModelIndex)));


    //Build a menu of possible sources for the "add" button
    QMenu * menu = new QMenu(this->ui->addSourceButton);
    menu->addAction("OpenStreetMap Tiles", this, SLOT(addOSMTileLayer()));
    this->ui->addSourceButton->setMenu(menu);
}
