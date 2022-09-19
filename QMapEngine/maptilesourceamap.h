#ifndef MAPTILESOURCEAMAP_H
#define MAPTILESOURCEAMAP_H

#include "maptilesource.h"

#include <QSet>
#include <QHash>
#include <QNetworkReply>

class QNetworkReply;

class MapTileSourceAMap : public MapTileSource
{
    Q_OBJECT
public:
    explicit MapTileSourceAMap();
    virtual ~MapTileSourceAMap();

    virtual QPointF ll2qgs(const QPointF& ll, quint8 zoomLevel) const;

    virtual QPointF qgs2ll(const QPointF& qgs, quint8 zoomLevel) const;

    virtual quint64 tilesOnZoomLevel(quint8 zoomLevel) const;

    virtual quint16 tileSize() const;

    virtual quint8 minZoomLevel(QPointF ll);

    virtual quint8 maxZoomLevel(QPointF ll);

    virtual QString name() const;

    virtual QString tileFileExtension() const;

protected:
    virtual void fetchTile(quint32 x,
                           quint32 y,
                           quint8 z);

private:
    //Set used to ensure a tile with a certain cacheID isn't requested twice
    QSet<QString> _pendingRequests;

    //Hash used to keep track of what cacheID goes with what reply
    QHash<QNetworkReply *, QString> _pendingReplies;

signals:

public slots:

private slots:
    void handleNetworkRequestFinished();
};

#endif // MAPTILESOURCEAMAP_H
