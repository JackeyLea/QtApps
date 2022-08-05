///////////////////////////
//
// \author jackey lea
// \date 2022.08.05
// \note 在地图上显示的目标、飞机、图像、航线等等
///////////////////////////

#ifndef MAPGRAPHICSOBJECT_H
#define MAPGRAPHICSOBJECT_H

#include <QObject>
#include <QPointF>
#include <QRectF>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneContextMenuEvent>
#include <QGraphicsItem>
#include <QString>

class MapGraphicsObject : public QObject
{
    Q_OBJECT
public:
    enum MapGraphicsObjectFlag{
        ObjectIsMovable = 0x01,//目标可移动
        ObjectIsSelectable = 0x02,//目标可选
        ObjectIsFocusable = 0x04//目标可选中
    };
    Q_DECLARE_FLAGS(MapGraphicsObjectFlags,MapGraphicsObjectFlag)

    explicit MapGraphicsObject(bool sizeIsZoomInvariant=false,MapGraphicsObject *parent = 0);
    virtual ~MapGraphicsObject();

    /*
     * \brief 目标尺寸在地图缩放时是否保存不变
     * \return bool
     */
    bool sizeIsZoomInvariant() const;

    /*
     * \brief 如果sizeIsZoomInvariant为真，应该返回本目标的像素rect值；
     *        如果为假，应该返回本目标的mm rect值
     * \return QRectF
     */
    virtual QRectF boundingRect() const = 0;

    /*
     * \brief 输入为地理（经纬度）坐标数据，返回本目标是否包含此点
     *        默认情况下由boundingRect()决定
     */
    virtual bool contains(const QPointF& geoPos) const;

    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget=0)=0;

    bool enabled() const;
    void setEnabled(bool);

    qreal opacity() const;
    void setOpacity(qreal);

    MapGraphicsObject* parent() const;
    void setParent(MapGraphicsObject*);

    const QPointF& pos() const;
    virtual void setPos(const QPointF&);

    qreal rotation() const;
    void setRotation(qreal);

    bool visible() const;
    void setVisible(bool);

    qreal longitude() const;
    void setLongitude(qreal);

    qreal latitude() const;
    void setLatitude(qreal);

    qreal zValue() const;
    void setZValue(qreal);

    bool isSelected() const;
    void setSelected(bool);

    QString toolTip() const;
    void setToolTip(const QString& toolTip);

    MapGraphicsObject::MapGraphicsObjectFlags flags() const;
    void setFlag(MapGraphicsObjectFlag,bool enabled = true);
    void setFlags(MapGraphicsObject::MapGraphicsObjectFlags);

protected:
    virtual void contextMenuEvent();
    virtual QVariant itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant& value);
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void keyReleaseEvent(QKeyEvent* event);
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);
    virtual void wheelEvent(QGraphicsSceneWheelEvent* event);

signals:
    void enabledChanged();
    void opacityChanged();
    void parentChanged();
    void posChanged();
    void rotationChanged();
    void visibleChanged();
    void zValueChanged();
    void toolTipChanged(const QString& toolTip);

    void flagsChanged();

    //暂时是内部使用
    void selectedChanged();

    void newObjectGenerated(MapGraphicsObject*);

    //如果本目标需要重绘
    void redrawRequested();

    //如果本目标需要键盘焦点
    void keyFocusRequested();

private slots:
    void setConstructed();

private:
    MapGraphicsObject* _parent;

    QPointF _pos;

    qreal _rotation;//目标旋转角度
    qreal _zValue;//z方向的值
    qreal _opacity;//目标透明度，0时完全为白色

    bool _visible;
    bool _selected;
    bool _constructed;
    bool _sizeIsZoomInvariant;//尺寸在缩放时是否改变
    bool _enabled;//当前目标是否启用

    QString _toolTip;//鼠标放置在目标上显示提示内容

    MapGraphicsObject::MapGraphicsObjectFlags _flags;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(MapGraphicsObject::MapGraphicsObjectFlags)

#endif // MAPGRAPHICSOBJECT_H
