/////////////////////////
/// \author jackey lea
/// \date 2022.08.17
/// \note 地图显示基类，比如说OSM、高德地图、百度地图、地形图、气象图等等，一层一层叠就行了
/////////////////////////

#ifndef MAPTILESOURCE_H
#define MAPTILESOURCE_H

#include <QObject>
#include <QPoint>
#include <QPointF>
#include <QImage>
#include <QCache>
#include <QMutex>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QHash>

class MapTileSource : public QObject
{
    Q_OBJECT
public:
    enum CacheMode
    {
        NoCaching,
        DiskAndMemCaching
    };

    explicit MapTileSource();
    virtual ~MapTileSource();

    void requestTile(quint32 x, quint32 y, quint8 z);

    QImage * getFinishedTile(quint32 x, quint32 y, quint8 z);

    MapTileSource::CacheMode cacheMode() const;
    void setCacheMode(MapTileSource::CacheMode);

    virtual QPointF ll2qgs(const QPointF& ll, quint8 zoomLevel) const=0;

    virtual QPointF qgs2ll(const QPointF& qgs, quint8 zoomLevel) const=0;

    virtual quint64 tilesOnZoomLevel(quint8 zoomLevel) const=0;

    virtual quint16 tileSize() const=0;

    virtual quint8 minZoomLevel(QPointF ll = QPointF())=0;

    virtual quint8 maxZoomLevel(QPointF ll = QPointF())=0;

    virtual QString name() const=0;

    virtual QString tileFileExtension() const=0;

protected:
    static QString createCacheID(quint32 x, quint32 y, quint8 z);
    static bool cacheID2xyz(const QString & string, quint32 * x, quint32 * y, quint32 * z);
    QImage * fromMemCache(const QString& cacheID);
    void toMemCache(const QString& cacheID, QImage * toCache, const QDateTime &expireTime = QDateTime());
    QImage * fromDiskCache(const QString& cacheID);
    void toDiskCache(const QString& cacheID, QImage * toCache, const QDateTime &expireTime = QDateTime());
    virtual void fetchTile(quint32 x, quint32 y, quint8 z)=0;
    void prepareNewlyReceivedTile(quint32 x, quint32 y, quint8 z, QImage * image, QDateTime expireTime = QDateTime());
    QDateTime getTileExpirationTime(const QString& cacheID);
    void setTileExpirationTime(const QString& cacheID, QDateTime expireTime);


signals:
    void tileRetrieved(quint32 x, quint32 y, quint8 z);
    void tileRequested(quint32 x, quint32 y, quint8 z);
    void allTilesInvalidated();

private slots:
    void startTileRequest(quint32 x, quint32 y, quint8 z);
    void clearTempCache();

private:
    void prepareRetrievedTile(quint32 x, quint32 y, quint8 z, QImage * image);
    QDir getDiskCacheDirectory(quint32 x, quint32 y, quint8 z) const;
    QString getDiskCacheFile(quint32 x, quint32 y, quint8 z) const;
    void loadCacheExpirationsFromDisk();

    void saveCacheExpirationsToDisk();

    bool _cacheExpirationsLoaded;

    QString _cacheExpirationsFile;

    MapTileSource::CacheMode _cacheMode;

    QCache<QString, QImage> _tempCache;
    QMutex _tempCacheLock;

    QCache<QString, QImage> _memoryCache;

    QHash<QString, QDateTime> _cacheExpirations;
};

#endif // MAPTILESOURCE_H
