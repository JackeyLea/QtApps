/////////////////////////
/// \author jackey lea
/// \date 2022.08.17
/// \note 所有在地图上显示的点、线、圆的基类的内部实现
/////////////////////////

#ifndef MAPOBJECTPRIVATE_H
#define MAPOBJECTPRIVATE_H

#include <QGraphicsObject>
#include <QPointer>
#include <QHash>

#include "mapobject.h"
#include "maptilesourceprivate.h"

class MapObjectPrivate : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit MapObjectPrivate(MapObject * mgObj,
                                    MapTileSourcePrivate * infoSource,
                                    QGraphicsItem *parent = 0);
    virtual ~MapObjectPrivate();

    //pure-virtual from QGraphicsItem
    virtual QRectF boundingRect() const;

    //virtual from QGraphicsitem
    bool contains(const QPointF &point) const;

    //pure-virtual from QGraphicsItem
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    //override from QGraphicsItem
    void setSelected(bool selected);

protected:
    //virtual from QGraphicsItem
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

    //virtual from QGraphicsItem
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);

    //virtual from QGraphicsItem
    virtual void keyPressEvent(QKeyEvent *event);

    //virtual from QGraphicsItem
    virtual void keyReleaseEvent(QKeyEvent *event);

    //virtual from QGraphicsItem
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

    //virtual from QGraphicsItem
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

    //virtual from QGraphicsItem
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);

    //virtual from QGraphicsItem
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    //virtual from QGraphicsItem
    virtual void wheelEvent(QGraphicsSceneWheelEvent *event);


signals:

public slots:
    void handleZoomLevelChanged();

private slots:
    void handleEnabledChanged();
    void handleOpacityChanged();
    void handleParentChanged();
    void handlePosChanged();
    void handleRotationChanged();
    void handleVisibleChanged();
    void handleZValueChanged();
    void handleMGSelectedChanged();
    void handleMGToolTipChanged(const QString& toolTip);
    void handleMGFlagsChanged();
    void updateAllFromMG();

    void handleRedrawRequested();
    void handleKeyFocusRequested();


private:
    void setMGObj(MapObject *);

    void convertSceneMouseEventCoordinates(QGraphicsSceneMouseEvent * event);
    void unconvertSceneMouseEventCoorindates(QGraphicsSceneMouseEvent * event);
    QHash<QGraphicsSceneMouseEvent *, QPointF> _unconvertedSceneMouseCoordinates;

    QPointer<MapObject> _mgObj;
    MapTileSourcePrivate * _infoSource;
};

#endif // MAPOBJECTPRIVATE_H
