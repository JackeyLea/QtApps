#ifndef MAPTILESOURCECONFIGURATIONWIDGET_H
#define MAPTILESOURCECONFIGURATIONWIDGET_H

#include <QWidget>
#include <QWeakPointer>
#include <QListView>

#include "maptilesourcecomposite.h"

namespace Ui {
class MapTileSourceConfigurationWidget;
}

class MapTileSourceConfigurationWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MapTileSourceConfigurationWidget(QWeakPointer<MapTileSourceComposite> composite = QWeakPointer<MapTileSourceComposite>(),
                                              QWidget *parent = 0);
    ~MapTileSourceConfigurationWidget();

    void setComposite(QWeakPointer<MapTileSourceComposite> nComposite);

private slots:
    void handleCurrentSelectionChanged(QModelIndex,QModelIndex);
    void handleCompositeChange();
    void addOSMTileLayer();

    void on_removeSourceButton_clicked();

    void on_opacitySlider_valueChanged(int value);

    void on_moveDownButton_clicked();

    void on_moveUpButton_clicked();

private:
        void init();
    Ui::MapTileSourceConfigurationWidget *ui;
        QWeakPointer<MapTileSourceComposite> _composite;
};

#endif // MAPTILESOURCECONFIGURATIONWIDGET_H
