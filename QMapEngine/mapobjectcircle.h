#ifndef MAPOBJECTCIRCLE_H
#define MAPOBJECTCIRCLE_H

#include "mapobject.h"

class MapObjectCircle : public MapObject
{
    Q_OBJECT
public:
    explicit MapObjectCircle(qreal radius,bool sizeIsZoomInvariant=true, QColor fillColor = QColor(0,0,0,0),MapObject *parent = 0);
    virtual ~MapObjectCircle();

    //pure-virtual from MapGraphicsObject
    QRectF boundingRect() const;

    //pure-virtual from MapGraphicsObject
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    qreal radius() const;
    void setRadius(qreal radius);

signals:

public slots:

protected:
    //virtual from MapGraphicsObject
    virtual void keyReleaseEvent(QKeyEvent *event);

private:
    qreal _radius;
    QColor _fillColor;
};

#endif // MAPOBJECTCIRCLE_H
