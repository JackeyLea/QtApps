////////////////////////
/// \author jackey lea
/// \date 20220721
/// \detail map for qt
////////////////////////

#ifndef QMAPENGINE_H
#define QMAPENGINE_H

#include <QWidget>
#include <QPointF>
#include <QRectF>
#include <QMenu>
#include <QMouseEvent>

class QMapEngine : public QWidget
{
    Q_OBJECT
public:
    enum MAPSTATE{
        Normal=0,
        Click,
        Drag,
        Resize,
    };

    typedef struct _QMapLayer{
        QString name;
    }QMapLayer;

    explicit QMapEngine(QWidget *parent = nullptr);
    ~QMapEngine();

    QString getMapPath();
    void setMapPath(QString path);

    QPointF getCenter();
    void setCenter(QPointF p);

    float getScaleRatio();
    void setScaleRatio(float ratio);

    MAPSTATE getState();
    void setState(MAPSTATE state);

    QRectF getGeoRect();
    void setGeoRect(QRectF rect);

    float getRotate();
    void setRotate(float rotate);

    QPointF transfGeo2View(QPointF geo);
    QPointF transfView2Geo(QPointF view);

    float getAltitude();

    float getDistance(QPointF p1,QPointF p2);
    float getAngle(QPointF p1,QPointF p2);

    float getArea(QList<QPointF> ps);

    void setGridVisible(bool b);

    void addLayer(QMapLayer *layer);
    void removeLayer(QString name);
    void removeLayer(QMapLayer *layer);

    QMapLayer findLayerByName(QString name);

    QList<QMapLayer> getAllLayers();

    QMenu *getMenu();
    void setMenu(QMenu* m);

    void setDrawBackground(bool b);

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *e);

signals:

private:
    QString _mapPath;

    QPointF _center;

    float _scaleratio;

    MAPSTATE _mapstate;

    QRectF _maprect;

    float _rotate;

    QList<QMapLayer> _maplayers;

    QMenu *_menu;

    bool _drawbg;
};

#endif // QMAPENGINE_H
