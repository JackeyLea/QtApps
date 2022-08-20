/////////////////////////
/// \author jackey lea
/// \date 2022.08.17
/// \note 所有在地图上显示的点、线、圆的基类
/////////////////////////

#ifndef MAPOBJECT_H
#define MAPOBJECT_H

#include <QObject>
#include <QPointF>
#include <QRectF>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneContextMenuEvent>
#include <QGraphicsItem>
#include <QString>
#include <QTimer>
#include <QKeyEvent>

//#include "mapobjectprivate.h"

class MapObject : public QObject
{
    Q_OBJECT
public:
    enum MapObjectFlag
    {
        ObjectIsMovable = 0x01,
        ObjectIsSelectable = 0x02,
        ObjectIsFocusable = 0x04
    };
    Q_DECLARE_FLAGS(MapObjectFlags,MapObjectFlag)

    friend class MapObjectPrivate;

    explicit MapObject(bool sizeIsZoomInvariant=false, MapObject *parent = 0);
    virtual ~MapObject();

    bool sizeIsZoomInvariant() const;

    virtual QRectF boundingRect() const=0;

    virtual bool contains(const QPointF& geoPos) const;

    virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget=0)=0;

    bool enabled() const;
    void setEnabled(bool);

    qreal opacity() const;
    void setOpacity(qreal);

    MapObject * parent() const;
    void setParent(MapObject *);

    const QPointF& pos() const;
    virtual void setPos(const QPointF&);

    qreal rotation() const;
    void setRotation(qreal);

    bool visible() const;
    void setVisible(bool);

    qreal longitude() const;
    void setLongitude(qreal);

    qreal latitude() const;
    void setLatitude(qreal);

    qreal zValue() const;
    void setZValue(qreal);

    bool isSelected() const;
    void setSelected(bool);

    QString toolTip() const;
    void setToolTip(const QString& toolTip);

    void setFlag(MapObjectFlag, bool enabled=true);
    void setFlags(MapObject::MapObjectFlags);
    MapObject::MapObjectFlags flags() const;

protected:
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent * event);
    virtual QVariant itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant& value);
    virtual void keyPressEvent(QKeyEvent * event);
    virtual void keyReleaseEvent(QKeyEvent * event);
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * event);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent * event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);
    virtual void wheelEvent(QGraphicsSceneWheelEvent * event);

signals:
    void enabledChanged();
    void opacityChanged();
    void parentChanged();
    void posChanged();
    void rotationChanged();
    void visibleChanged();
    void zValueChanged();
    void toolTipChanged(const QString& toolTip);

    void flagsChanged();

    void selectedChanged();

    void newObjectGenerated(MapObject *);

    void redrawRequested();

    void keyFocusRequested();

private slots:
    void setConstructed();

private:
    bool _sizeIsZoomInvariant;

    bool _enabled;
    qreal _opacity;
    MapObject * _parent;
    QPointF _pos;
    qreal _rotation;
    bool _visible;
    qreal _zValue;
    bool _selected;

    QString _toolTip;

    MapObject::MapObjectFlags _flags;

    bool _constructed;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(MapObject::MapObjectFlags)

#endif // MAPOBJECT_H
