#include "mapobjectcircle.h"

#include <QtDebug>
#include <QKeyEvent>

MapObjectCircle::MapObjectCircle(qreal radius, bool sizeIsZoomInvariant, QColor fillColor, MapObject *parent)
    : MapObject(sizeIsZoomInvariant,parent)
    , _fillColor(fillColor)
{
    _radius = qMax<qreal>(radius,0.01);

    this->setFlag(MapObject::ObjectIsSelectable);
    this->setFlag(MapObject::ObjectIsMovable);
    this->setFlag(MapObject::ObjectIsFocusable);
}

MapObjectCircle::~MapObjectCircle()
{

}

QRectF MapObjectCircle::boundingRect() const
{
    return QRectF(-1*_radius,
                  -1*_radius,
                  2*_radius,
                  2*_radius);
}

void MapObjectCircle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    painter->setRenderHint(QPainter::Antialiasing,true);
    painter->setBrush(_fillColor);
    painter->drawEllipse(QPointF(0,0),
                         _radius,
                         _radius);
}

qreal MapObjectCircle::radius() const
{
    return _radius;
}

void MapObjectCircle::setRadius(qreal radius)
{
    _radius = radius;
    this->redrawRequested();
}

void MapObjectCircle::keyReleaseEvent(QKeyEvent *event)
{
    if (event->matches(QKeySequence::Delete))
    {
        this->deleteLater();
        event->accept();
    }
    else
        event->ignore();
}
