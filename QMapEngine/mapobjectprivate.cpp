#include "mapobjectprivate.h"

#include <QtDebug>
#include <QKeyEvent>

#include "conversion.h"

class Conversion;

MapObjectPrivate::MapObjectPrivate(MapObject *mgObj, MapTileSourcePrivate *infoSource, QGraphicsItem *parent)
    : QGraphicsObject(parent)
    , _infoSource(infoSource)
{
    this->setMGObj(mgObj);
    this->setZValue(5.0);

    //this->setFlag(QGraphicsItem::ItemIsMovable,true);
    //this->setFlag(QGraphicsItem::ItemIsSelectable,true);
    this->setFlag(QGraphicsItem::ItemSendsScenePositionChanges,true);
}

MapObjectPrivate::~MapObjectPrivate()
{

}

QRectF MapObjectPrivate::boundingRect() const
{
    QRectF toRet(-1.0,-1.0,2.0,2.0);
    if (_mgObj.isNull())
    {
        qWarning() << "Warning:" << this << "could not get bounding rect as MapGraphicsObject is null";
        return toRet;
    }

    //If the object's size is zoom invariant (e.g., labels or markers) then assume the rect's units are pixels
    if (_mgObj->sizeIsZoomInvariant())
        return _mgObj->boundingRect();

    //Otherwise, assume they're meters and do some conversions!
    QRectF enuRect = _mgObj->boundingRect();

    //Convert from ENU to lat/lon
    QPointF latLonCenter = _mgObj->pos();
    Position latLonCenterPos(latLonCenter, 0.0);
    QPointF leftLatLon = Conversion::enu2lla(enuRect.left(),
                                              0.0,
                                              0.0,
                                              latLonCenterPos).lonLat();
    QPointF upLatLon = Conversion::enu2lla(0.0,
                                            enuRect.top(),
                                            0.0,
                                            latLonCenterPos).lonLat();

    qreal lonWidth = 2.0*(latLonCenter.x() - leftLatLon.x());
    qreal latHeight = 2.0*(upLatLon.y() - latLonCenter.y());

    //Once we've got the rect in lat/lon, we should convert it to scene pixels
    QRectF latLonRect(leftLatLon.x(),upLatLon.y(),lonWidth,latHeight);

    //We need our tile source to do the conversion
    QSharedPointer<MapTileSource> tileSource = _infoSource->tileSource();
    if (tileSource.isNull())
    {
        qWarning() << this << "can't do bounding box conversion, null tile source.";
        return toRet;
    }

    int zoomLevel = _infoSource->zoomLevel();
    QPointF topLeft = tileSource->ll2qgs(latLonRect.topLeft(),zoomLevel);
    QPointF bottomRight = tileSource->ll2qgs(latLonRect.bottomRight(),zoomLevel);

    toRet = QRectF(topLeft,bottomRight);
    toRet.moveCenter(QPointF(0,0));
    return toRet;
}

bool MapObjectPrivate::contains(const QPointF &point) const
{
    if (_mgObj.isNull())
        return false;

    //convert point to QGraphicsScene coordinates
    QPointF scenePoint = this->mapToScene(point);

    //Convert scenePoint to geo coordinates
    QSharedPointer<MapTileSource> tileSource = _infoSource->tileSource();
    if (tileSource.isNull())
    {
        qWarning() << this << "can't do bounding box conversion, null tile source.";
        return false;
    }
    QPointF geoPoint = tileSource->qgs2ll(scenePoint,_infoSource->zoomLevel());

    //Ask our MapGraphicsObject about containment
    return _mgObj->contains(geoPoint);
}

void MapObjectPrivate::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if (_mgObj.isNull())
    {
        qWarning() << this << "could not paint as our MapGraphicsObject is null";
        return;
    }

    painter->save();
    painter->scale(1.0,-1.0);

    //Transform painter coordinates to the object's bounding box and then have the MapGraphicsObject do its thing
    if (!_mgObj->sizeIsZoomInvariant())
    {
        QRectF enuRect = _mgObj->boundingRect();
        qreal desiredWidthMeters = enuRect.width();
        qreal desiredHeightMeters = enuRect.height();
        QRectF pixelRect = this->boundingRect();
        qreal widthPixels = pixelRect.width();
        qreal heightPixels = pixelRect.height();

        qreal scaleX = widthPixels / desiredWidthMeters;
        qreal scaleY = heightPixels / desiredHeightMeters;

        painter->scale(scaleX,scaleY);
    }

    _mgObj->paint(painter,option,widget);

    painter->restore();

    if (this->isSelected())
        painter->drawRect(this->boundingRect());
}

void MapObjectPrivate::setSelected(bool selected)
{
    QGraphicsItem::setSelected(selected);

    //Tell the MapGraphicsObject that we represent about it
    _mgObj->setSelected(this->isSelected());
}

void MapObjectPrivate::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    if (_mgObj.isNull())
        return;

    //Convert to geo position for the MapGraphicsObject
    const QPointF qgsScenePos = event->scenePos();
    QSharedPointer<MapTileSource> tileSource = _infoSource->tileSource();
    QPointF geoScenePos = tileSource->qgs2ll(qgsScenePos,_infoSource->zoomLevel());
    event->setScenePos(geoScenePos);

    _mgObj->contextMenuEvent(event);

    //Convert back to QGS coordinates
    event->setScenePos(qgsScenePos);

    if (!event->isAccepted())
        QGraphicsObject::contextMenuEvent(event);
}

QVariant MapObjectPrivate::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (_mgObj.isNull())
        return value;

    //When the position changes as a result of user action we need to update the MapGraphicsObject's geo pos
    if (change == QGraphicsItem::ItemPositionChange)
    {
        QPointF scenePos = value.toPointF();
        QSharedPointer<MapTileSource> tileSource = _infoSource->tileSource();
        if (!tileSource.isNull())
        {
            QPointF geoPos = tileSource->qgs2ll(scenePos,_infoSource->zoomLevel());

            //Hackz
            this->setFlag(QGraphicsItem::ItemSendsScenePositionChanges,false);
            _mgObj->setPos(geoPos);
            this->setFlag(QGraphicsItem::ItemSendsScenePositionChanges,true);
        }

    }

    return _mgObj->itemChange(change,value);
}

void MapObjectPrivate::keyPressEvent(QKeyEvent *event)
{
    if (_mgObj.isNull())
        return;

    _mgObj->keyPressEvent(event);

    if (!event->isAccepted())
        QGraphicsObject::keyPressEvent(event);
}

void MapObjectPrivate::keyReleaseEvent(QKeyEvent *event)
{
    if (_mgObj.isNull())
        return;

    _mgObj->keyReleaseEvent(event);

    if (!event->isAccepted())
        QGraphicsObject::keyReleaseEvent(event);
}

void MapObjectPrivate::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if (_mgObj.isNull())
        return;

    this->convertSceneMouseEventCoordinates(event);
    _mgObj->mouseDoubleClickEvent(event);
    this->unconvertSceneMouseEventCoorindates(event);

    if (!event->isAccepted())
        QGraphicsObject::mouseDoubleClickEvent(event);
}

void MapObjectPrivate::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (_mgObj.isNull())
        return;

//    QGraphicsObject::mouseMoveEvent(event);
//    return;

    this->convertSceneMouseEventCoordinates(event);
    _mgObj->mouseMoveEvent(event);
    this->unconvertSceneMouseEventCoorindates(event);

    if (!event->isAccepted())
        QGraphicsObject::mouseMoveEvent(event);
}

void MapObjectPrivate::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (_mgObj.isNull())
        return;

    this->convertSceneMouseEventCoordinates(event);
    _mgObj->mousePressEvent(event);
    this->unconvertSceneMouseEventCoorindates(event);

    if (!event->isAccepted())
        QGraphicsObject::mousePressEvent(event);
}

void MapObjectPrivate::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (_mgObj.isNull())
        return;
    this->convertSceneMouseEventCoordinates(event);
    _mgObj->mouseReleaseEvent(event);
    this->unconvertSceneMouseEventCoorindates(event);

    if (!event->isAccepted())
        QGraphicsObject::mouseReleaseEvent(event);
}

void MapObjectPrivate::wheelEvent(QGraphicsSceneWheelEvent *event)
{
    if (_mgObj.isNull())
        return;

    //Convert to geo position for the MapGraphicsObject
    const QPointF qgsScenePos = event->scenePos();
    QSharedPointer<MapTileSource> tileSource = _infoSource->tileSource();
    QPointF geoScenePos = tileSource->qgs2ll(qgsScenePos,_infoSource->zoomLevel());
    event->setScenePos(geoScenePos);

    _mgObj->wheelEvent(event);

    //Convert back to QGS coordinates
    event->setScenePos(qgsScenePos);

    if (!event->isAccepted())
        QGraphicsObject::wheelEvent(event);
}

void MapObjectPrivate::handleZoomLevelChanged()
{
    this->handlePosChanged();
}

void MapObjectPrivate::handleEnabledChanged()
{
    this->setEnabled(_mgObj->enabled());
}

void MapObjectPrivate::handleOpacityChanged()
{
    this->setOpacity(_mgObj->opacity());
}

void MapObjectPrivate::handleParentChanged()
{
    //Need more complicated logic here that involves PrivateQGraphicsScene
}

void MapObjectPrivate::handlePosChanged()
{
    //Get the position of the object in lat,lon,alt
    QPointF geoPos = _mgObj->pos();

    //TODO:If the object has a parent, do stupid stuff here to handle it

    //Convert LLA coordinates to QGraphicsScene coordinates
    QSharedPointer<MapTileSource> tileSource = _infoSource->tileSource();
    if (tileSource.isNull())
        return;

    int zoomLevel = _infoSource->zoomLevel();
    QPointF qgsPos = tileSource->ll2qgs(geoPos,zoomLevel);

    /*
      We disable the position change notifications to itemChange() before calling setPos so that
      itemChange() doesn't cause another change to geoPos. i.e., it isn't treated as if the object
      has been moved by the mouse. Don't think about it too much.
    */
    this->setFlag(QGraphicsObject::ItemSendsScenePositionChanges,false);
    this->setPos(qgsPos);
    this->setFlag(QGraphicsObject::ItemSendsScenePositionChanges,true);
}

void MapObjectPrivate::handleRotationChanged()
{
    this->setRotation(_mgObj->rotation());
}

void MapObjectPrivate::handleVisibleChanged()
{
    this->setVisible(_mgObj->visible());
}

void MapObjectPrivate::handleZValueChanged()
{
    this->setZValue(_mgObj->zValue());
}

void MapObjectPrivate::handleMGSelectedChanged()
{
    if (_mgObj->isSelected() == this->isSelected())
        return;

    this->setSelected(_mgObj->isSelected());
    _mgObj->_selected = this->isSelected();

    this->update();
}

void MapObjectPrivate::handleMGToolTipChanged(const QString &toolTip)
{
    this->setToolTip(toolTip);
}

void MapObjectPrivate::handleMGFlagsChanged()
{
    MapObject::MapObjectFlags flags = _mgObj->flags();

    bool movable = false;
    bool selectable = false;
    bool focusable = false;

    if (flags & MapObject::ObjectIsMovable)
        movable = true;

    if (flags & MapObject::ObjectIsSelectable)
        selectable = true;

    if (flags & MapObject::ObjectIsFocusable)
        focusable = true;

    this->setFlag(QGraphicsObject::ItemIsMovable,movable);
    this->setFlag(QGraphicsObject::ItemIsSelectable,selectable);
    this->setFlag(QGraphicsObject::ItemIsFocusable,focusable);
}

void MapObjectPrivate::updateAllFromMG()
{
    this->handleEnabledChanged();
    this->handleOpacityChanged();
    this->handleParentChanged();
    this->handlePosChanged();
    this->handleRotationChanged();
    this->handleVisibleChanged();
    this->handleZValueChanged();
    this->handleMGSelectedChanged();
    this->handleMGFlagsChanged();
}

void MapObjectPrivate::handleRedrawRequested()
{
    this->update();
}

void MapObjectPrivate::handleKeyFocusRequested()
{
    this->setFocus();
}

void MapObjectPrivate::setMGObj(MapObject *mgObj)
{
    _mgObj = mgObj;

    if (_mgObj == 0)
        return;

    //Connect signals/slots for all of the changes in the MapGraphicsObject that we must be able to handle
    connect(_mgObj,
            SIGNAL(enabledChanged()),
            this,
            SLOT(handleEnabledChanged()));
    connect(_mgObj,
            SIGNAL(opacityChanged()),
            this,
            SLOT(handleOpacityChanged()));
    connect(_mgObj,
            SIGNAL(parentChanged()),
            this,
            SLOT(handleParentChanged()));
    connect(_mgObj,
            SIGNAL(posChanged()),
            this,
            SLOT(handlePosChanged()));
    connect(_mgObj,
            SIGNAL(rotationChanged()),
            this,
            SLOT(handleRotationChanged()));
    connect(_mgObj,
            SIGNAL(visibleChanged()),
            this,
            SLOT(handleVisibleChanged()));
    connect(_mgObj,
            SIGNAL(zValueChanged()),
            this,
            SLOT(handleZValueChanged()));
    connect(_mgObj,
            SIGNAL(selectedChanged()),
            this,
            SLOT(handleMGSelectedChanged()));
    connect(_mgObj,
            SIGNAL(toolTipChanged(QString)),
            this,
            SLOT(handleMGToolTipChanged(QString)));
    connect(_mgObj,
            SIGNAL(flagsChanged()),
            this,
            SLOT(handleMGFlagsChanged()));
    connect(_mgObj,
            SIGNAL(keyFocusRequested()),
            this,
            SLOT(handleKeyFocusRequested()));
    connect(_mgObj,
            SIGNAL(redrawRequested()),
            this,
            SLOT(handleRedrawRequested()));

    //Get all of the info about the MGObject
    this->updateAllFromMG();

    connect(mgObj,
            SIGNAL(destroyed()),
            this,
            SLOT(deleteLater()));
}

void MapObjectPrivate::convertSceneMouseEventCoordinates(QGraphicsSceneMouseEvent *event)
{
    const QPointF qgsScenePos = event->scenePos();
    QSharedPointer<MapTileSource> tileSource = _infoSource->tileSource();
    QPointF geoPos = tileSource->qgs2ll(qgsScenePos,_infoSource->zoomLevel());

    _unconvertedSceneMouseCoordinates.insert(event,qgsScenePos);

    event->setScenePos(geoPos);
}

void MapObjectPrivate::unconvertSceneMouseEventCoorindates(QGraphicsSceneMouseEvent *event)
{
    QPointF qgsScenePos;
    if (_unconvertedSceneMouseCoordinates.contains(event))
        qgsScenePos = _unconvertedSceneMouseCoordinates.take(event);
    else
    {
        qWarning() << this << "didn't have original scene mouse coordiantes stored for un-conversion";
        QSharedPointer<MapTileSource> tileSource = _infoSource->tileSource();
        qgsScenePos = tileSource->ll2qgs(event->scenePos(),_infoSource->zoomLevel());
    }
    event->setScenePos(qgsScenePos);
}
