#include "mapobject.h"

MapObject::MapObject(bool sizeIsZoomInvariant, MapObject *parent)
    : _sizeIsZoomInvariant(sizeIsZoomInvariant)
    , _constructed(false)
{
    _enabled = true;
    _opacity = 1.0;
    this->setParent(parent);
    _pos = QPointF(0.0,0.0);
    _rotation = 0.0;
    _visible = true;
    _zValue = 0.0;
    _selected = false;

    QTimer::singleShot(1, this, SLOT(setConstructed()));
}

MapObject::~MapObject()
{

}

bool MapObject::sizeIsZoomInvariant() const
{
    return _sizeIsZoomInvariant;
}

bool MapObject::contains(const QPointF &geoPos) const
{
    QRectF geoRect = this->boundingRect();
    return geoRect.contains(geoPos);
}

bool MapObject::enabled() const
{
    return _enabled;
}

void MapObject::setEnabled(bool nEnabled)
{
    _enabled = nEnabled;
    if (_constructed)
        this->enabledChanged();
    else
        QTimer::singleShot(1, this, SIGNAL(enabledChanged()));
}

qreal MapObject::opacity() const
{
    return _opacity;
}

void MapObject::setOpacity(qreal nOpacity)
{
    _opacity = nOpacity;
    if (_constructed)
        this->opacityChanged();
    else
        QTimer::singleShot(1, this, SIGNAL(opacityChanged()));
}

MapObject *MapObject::parent() const
{
    return _parent;
}

void MapObject::setParent(MapObject *nParent)
{
    _parent = nParent;
    if (_constructed)
        this->parentChanged();
    else
        QTimer::singleShot(1, this, SIGNAL(parentChanged()));
}

const QPointF &MapObject::pos() const
{
    return _pos;
}

void MapObject::setPos(const QPointF &nPos)
{
    if (nPos == _pos)
        return;
    _pos = nPos;

    if (_constructed)
        this->posChanged();
    else
        QTimer::singleShot(1, this, SIGNAL(posChanged()));
}

qreal MapObject::rotation() const
{
    return _rotation;
}

void MapObject::setRotation(qreal nRotation)
{
    if (nRotation == _rotation)
        return;
    _rotation = nRotation;

    if (_constructed)
        this->rotationChanged();
    else
        QTimer::singleShot(1, this, SIGNAL(rotationChanged()));
}

bool MapObject::visible() const
{
    return _visible;
}

void MapObject::setVisible(bool nVisible)
{
    if (nVisible == _visible)
        return;
    _visible = nVisible;

    if (_constructed)
        this->visibleChanged();
    else
        QTimer::singleShot(1, this, SIGNAL(visibleChanged()));
}

qreal MapObject::longitude() const
{
    return _pos.x();
}

void MapObject::setLongitude(qreal nLongitude)
{
    this->setPos(QPointF(nLongitude,this->pos().y()));
}

qreal MapObject::latitude() const
{
    return _pos.y();
}

void MapObject::setLatitude(qreal nLatitude)
{
    this->setPos(QPointF(this->pos().x(),nLatitude));
}

qreal MapObject::zValue() const
{
    return _zValue;
}

void MapObject::setZValue(qreal nZValue)
{
    _zValue = nZValue;

    if (_constructed)
        this->zValueChanged();
    else
        QTimer::singleShot(1, this, SIGNAL(zValueChanged()));
}

bool MapObject::isSelected() const
{
    return _selected;
}

void MapObject::setSelected(bool sel)
{
    if (_selected == sel)
        return;
    _selected = sel;

    if (_constructed)
        this->selectedChanged();
    else
        QTimer::singleShot(1, this, SIGNAL(selectedChanged()));
}

QString MapObject::toolTip() const
{
    return _toolTip;
}

void MapObject::setToolTip(const QString &toolTip)
{
    _toolTip = toolTip;
    this->toolTipChanged(toolTip);
}

void MapObject::setFlag(MapObjectFlag flag, bool enabled)
{
    if (enabled)
        _flags = _flags | flag;
    else
        _flags = _flags & (~flag);

    this->flagsChanged();
}

void MapObject::setFlags(MapObjectFlags flags)
{
    _flags = flags;

    if (_constructed)
        this->flagsChanged();
    else
        QTimer::singleShot(1, this, SIGNAL(flagsChanged()));
}

MapObject::MapObjectFlags MapObject::flags() const
{
    return _flags;
}

void MapObject::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    event->ignore();
}

QVariant MapObject::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    Q_UNUSED(change)
    return value;
}

void MapObject::keyPressEvent(QKeyEvent *event)
{
    event->ignore();
}

void MapObject::keyReleaseEvent(QKeyEvent *event)
{
    event->ignore();
}

void MapObject::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    event->ignore();
}

void MapObject::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    event->ignore();
}

void MapObject::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (this->flags() & MapObject::ObjectIsMovable
            ||this->flags() & MapObject::ObjectIsSelectable)
    {
        event->accept();
        if (this->flags() & MapObject::ObjectIsFocusable)
            this->keyFocusRequested();
    }
    else
        event->ignore();
}

void MapObject::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    event->ignore();
}

void MapObject::wheelEvent(QGraphicsSceneWheelEvent *event)
{
    event->ignore();
}

void MapObject::setConstructed()
{
    _constructed = true;
}
