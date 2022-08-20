/////////////////////////
/// \author jackey lea
/// \date 2022.08.17
/// \note 地图显示基类私有类补充
/////////////////////////

#ifndef MAPTILESOURCEPRIVATE_H
#define MAPTILESOURCEPRIVATE_H

#include <QSharedPointer>

#include "maptilesource.h"

class MapTileSourcePrivate
{
public:
    virtual quint8 zoomLevel() const=0;

    virtual QSharedPointer<MapTileSource> tileSource() const=0;
};

#endif // MAPTILESOURCEPRIVATE_H
