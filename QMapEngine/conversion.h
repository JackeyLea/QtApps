/////////////////////////
/// \author jackey lea
/// \date 2022.08.17
/// \note 地图点在各种坐标系下转换
/////////////////////////

#ifndef CONVERSION_H
#define CONVERSION_H

#include <QTransform>
#include <QVector3D>
#include <QPointF>

#include "position.h"

class Position;

class Conversion
{
public:
    static QVector3D lla2xyz(qreal wlat, qreal wlon, qreal walt);
    static QVector3D lla2xyz(const Position &lla);
    static Position xyz2lla(const QVector3D &);
    static Position xyz2lla(qreal x, qreal y, qreal z);

    static QVector3D xyz2enu(const QVector3D & xyz, qreal reflat, qreal reflon, qreal refalt);
    static QVector3D xyz2enu(const QVector3D & xyz, const Position & refLLA);
    static QVector3D xyz2enu(qreal x, qreal y, qreal z, qreal reflat, qreal reflon, qreal refalt);
    static QVector3D xyz2enu(qreal x, qreal y, qreal z, const Position & refLLA);
    static QVector3D enu2xyz(const QVector3D & enu, qreal reflat, qreal reflon, qreal refalt);
    static QVector3D enu2xyz(const QVector3D & enu, const Position & refLLA);
    static QVector3D enu2xyz(qreal east, qreal north, qreal up, qreal reflat, qreal reflon, qreal refalt);
    static QVector3D enu2xyz(qreal east, qreal north, qreal up, const Position &refLLA);

    static Position enu2lla(const QVector3D &enu, qreal reflat, qreal reflon, qreal refalt);
    static Position enu2lla(const QVector3D &enu, const Position &refLLA);
    static Position enu2lla(qreal east, qreal north, qreal up, qreal reflat, qreal reflon, qreal refalt);
    static Position enu2lla(qreal east, qreal north, qreal up, const Position & refLLA);
    static QVector3D lla2enu(qreal lat, qreal lon, qreal alt, qreal reflat, qreal reflon, qreal refalt);
    static QVector3D lla2enu(qreal lat, qreal lon, qreal alt, const Position & refLLA);
    static QVector3D lla2enu(const Position & lla, qreal reflat, qreal reflon, qreal refalt);
    static QVector3D lla2enu(const Position & lla, const Position & refLLA);

    static qreal degreesLatPerMeter(const qreal latitude);
    static qreal degreesLonPerMeter(const qreal latitude);

    static QTransform rot(qreal angle, quint32 axis);

    static void test();
};

#endif // CONVERSION_H
