/////////////////////////
/// \author jackey lea
/// \date 2022.08.17
/// \note 地图显示基类私有类补充
/////////////////////////

#ifndef MAPTILESOURCEPRIVATE_H
#define MAPTILESOURCEPRIVATE_H

#include <QSharedPointer>

#include "maptilesource.h"

/*!
 \brief This abstract class is inherited by MapGraphicsView as an implementation of
 the "dependency inversion" design pattern, or at least as well as I can remember it.

 Basically, PrivateQGraphicsObject and PrivateQGraphicsScene need information from
 MapGraphicsView, but MapGraphicsView is above those two classes.

 By making all references to MapGraphicsView in PrivateQGraphicsObject and PrivateQGraphicsScene
 go through this interface, we sort of eliminate an upwards-facing compile-time dependency.

*/
class MapTileSourcePrivate
{
public:
    virtual quint8 zoomLevel() const=0;

    virtual QSharedPointer<MapTileSource> tileSource() const=0;
};

#endif // MAPTILESOURCEPRIVATE_H
