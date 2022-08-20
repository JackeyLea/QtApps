#ifndef MAPVIEW_H
#define MAPVIEW_H

#include <QWidget>
#include <QPointer>
#include <QSharedPointer>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QList>
#include <QContextMenuEvent>
#include <QVector3D>
#include <QStringBuilder>
#include <QHash>

#include "mapscene.h"
#include "mapobject.h"
#include "maptilesource.h"
#include "maptileobject.h"
#include "maptilesourceprivate.h"

class MapView : public QWidget, public MapTileSourcePrivate
{
    Q_OBJECT
public:
    enum DragMode
    {
        NoDrag,
        ScrollHandDrag,
        RubberBandDrag
    };

    enum ZoomMode
    {
        CenterZoom,
        MouseZoom
    };

    explicit MapView(MapScene *scene=0, QWidget * parent = 0);
    virtual ~MapView();

    QPointF center() const;
    void centerOn(const QPointF& pos);
    void centerOn(qreal longitude, qreal latitude);
    void centerOn(const MapObject * item);

    QPointF mapToScene(const QPoint viewPos) const;

    MapView::DragMode dragMode() const;
    void setDragMode(MapView::DragMode);

    MapScene * scene() const;
    void setScene(MapScene *);

    //pure-virtual from PrivateQGraphicsInfoSource
    QSharedPointer<MapTileSource> tileSource() const;

    /**
     * @brief Sets the tile source that this view will pull from.
     * MapGraphicsView does NOT take ownership of the tile source.
     *
     * @param tSource
     */
    void setTileSource(QSharedPointer<MapTileSource> tSource);

    //pure-virtual from PrivateQGraphicsInfoSource
    quint8 zoomLevel() const;
    void setZoomLevel(quint8 nZoom, ZoomMode zMode = CenterZoom);

    void zoomIn(ZoomMode zMode = CenterZoom);
    void zoomOut(ZoomMode zMode = CenterZoom);

    void rotate(qreal rotation);

signals:
    void zoomLevelChanged(quint8 nZoom);

public slots:

protected slots:
    virtual void handleChildMouseDoubleClick(QMouseEvent * event);
    virtual void handleChildMouseMove(QMouseEvent * event);
    virtual void handleChildMousePress(QMouseEvent * event);
    virtual void handleChildMouseRelease(QMouseEvent * event);
    virtual void handleChildViewContextMenu(QContextMenuEvent * event);
    virtual void handleChildViewScrollWheel(QWheelEvent * event);

private slots:
    void renderTiles();

protected:
    void doTileLayout();
    void resetQGSSceneSize();

private:
    QPointer<MapScene> _scene;
    QPointer<QGraphicsView> _childView;
    QPointer<QGraphicsScene> _childScene;
    QSharedPointer<MapTileSource> _tileSource;

    QSet<MapTileObject *> _tileObjects;

    quint8 _zoomLevel;

    DragMode _dragMode;

};

inline uint qHash(const QPointF& key)
{
    const QString temp = QString::number(key.x()) % "," % QString::number(key.y());
    return qHash(temp);
}

#endif // MAPVIEW_H
