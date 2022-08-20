#ifndef MAPOBJECTPOLYGON_H
#define MAPOBJECTPOLYGON_H

#include "mapobject.h"
#include "mapobjectcircle.h"

#include "position.h"
#include "conversion.h"

#include <QPolygonF>
#include <QList>

class Conversion;
class Position;

class MapObjectPolygon : public MapObject
{
    Q_OBJECT
public:
    explicit MapObjectPolygon(QPolygonF geoPoly, QColor fillColor = QColor(200,200,200,200), QObject *parent = 0);
    virtual ~MapObjectPolygon();

    //pure-virtual from MapGraphicsObject
    QRectF boundingRect() const;

    //virtual from MapGraphicsObject
    bool contains(const QPointF &geoPos) const;

    //pure-virtual from MapGraphicsObject
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    //virtual from MapGraphicsObject
    virtual void setPos(const QPointF &);

    QPolygonF geoPoly() const;

    virtual void setGeoPoly(const QPolygonF& newPoly);

    void setFillColor(const QColor& color);

signals:
    void polygonChanged(const QPolygonF& poly);

public slots:

protected:
    //virtual from MapGraphicsObject
    virtual void mousePressEvent(QGraphicsSceneMouseEvent * event);
    virtual void keyReleaseEvent(QKeyEvent *event);

private slots:
    void handleEditCirclePosChanged();
    void handleAddVertexCircleSelected();
    void handleEditCircleDestroyed();

private:
    void fixAddVertexCirclePos();

    MapObjectCircle * constructEditCircle();
    void destroyEditCircle(MapObject * obj);

    MapObjectCircle * constructAddVertexCircle();
    void destroyAddVertexCircle(MapObject * obj);

    QPolygonF _geoPoly;
    QColor _fillColor;

    QList<MapObject *> _editCircles;
    QList<MapObject *> _addVertexCircles;
};

#endif // MAPOBJECTPOLYGON_H
