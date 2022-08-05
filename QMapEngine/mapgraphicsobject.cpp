#include "mapgraphicsobject.h"

MapGraphicsObject::MapGraphicsObject(bool sizeIsZoomInvariant, MapGraphicsObject *parent)
    : _sizeIsZoomInvariant(sizeIsZoomInvariant)
    , _constructed(false)
{
    //将输入参数作为默认属性设置
    _enabled = true;
    _opacity = 1.0;
    _pos = QPointF(0.0,0.0);
    _rotation = 0.0;
    _visible = true;
    _zValue = 0.0;
    _selected = false;

    setParent(parent);
}

MapGraphicsObject::~MapGraphicsObject()
{

}

/*
 * \brief 目标尺寸在地图缩放时是否保存不变
 * \return bool
 */
bool MapGraphicsObject::sizeIsZoomInvariant() const
{
    return _sizeIsZoomInvariant;
}

/*
 * \brief 输入为地理（经纬度）坐标数据，返回本目标是否包含此点
 *        默认情况下由boundingRect()决定
 */
bool MapGraphicsObject::contains(const QPointF &geoPos) const
{
    QRectF geoRect = boundingRect();
    return geoRect.contains(geoPos);
}

bool MapGraphicsObject::enabled() const
{
    return _enabled;
}

void MapGraphicsObject::setEnabled(bool nEnabled)
{
    _enabled = nEnabled;
    if(_constructed){
        enabledChanged();//enable状态改变了
    }else{

    }
}

qreal MapGraphicsObject::opacity() const
{
    return _opacity;
}
