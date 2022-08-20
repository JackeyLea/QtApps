#ifndef MAPTILELAYERLISTMODEL_H
#define MAPTILELAYERLISTMODEL_H

#include <QAbstractListModel>
#include <QWeakPointer>

#include "maptilesourcecomposite.h"

class MapTileLayerListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit MapTileLayerListModel(QWeakPointer<MapTileSourceComposite> composite,QObject *parent = 0);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;

signals:

public slots:

private slots:
    void handleCompositeSourcesChanged();
    void handleCompositeSourcesAdded(int index);
    void handleCompositeSourcesRemoved(int index);

private:
    QWeakPointer<MapTileSourceComposite> _composite;
};

#endif // MAPTILELAYERLISTMODEL_H
