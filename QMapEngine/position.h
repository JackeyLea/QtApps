/////////////////////////
/// \author jackey lea
/// \date 2022.08.17
/// \note 地图上点处理类
/////////////////////////

#ifndef POSITION_H
#define POSITION_H

#include <QPointF>
#include <QtDebug>
#include <QVector3D>
#include <QVector2D>

#include "conversion.h"

class Conversion;

class Position
{
public:
    Position();
    Position(qreal longitude, qreal latitude, qreal altitude = 0.0);
    Position(const QPointF& lonLat, qreal altitude = 0.0);
    Position(const Position& other);
    ~Position();

    bool operator ==(const Position& other) const;
    bool operator !=(const Position& other) const;
    Position& operator =(const Position& other);

    qreal longitude() const;
    qreal latitude() const;
    qreal altitude() const;
    QPointF lonLat() const;

    void setLongitude(const qreal& longitude);
    void setLatitude(const qreal& latitude);
    void setAltitude(const qreal& altitude);

    qreal flatDistanceEstimate(const Position& other) const;
    QVector2D flatOffsetMeters(const Position& dest) const;
    Position flatOffsetToPosition(const QPointF &offset) const;
    qreal flatManhattanEstimate(const Position& other) const;
    qreal angleTo(const Position& dest) const;

    static QVector3D Position2ENU(const Position& refPos, const Position& pos);
    static Position fromENU(const Position& refPos, const QVector3D& enu);

private:
    QPointF _lonLat;
    qreal _altitude;
};

//让qDebug()支持此类
QDebug operator<<(QDebug dbg, const Position& pos);

//Non-member methods for serializing and de-serializing
QDataStream& operator<<(QDataStream& stream, const Position& pos);
QDataStream& operator>>(QDataStream& stream, Position& pos);

//Non-member method for hashing

uint qHash(const Position& pos);

#endif // POSITION_H
