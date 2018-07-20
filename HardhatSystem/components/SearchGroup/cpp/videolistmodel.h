#ifndef VIDEOLISTMODEL_H
#define VIDEOLISTMODEL_H
#include <QAbstractListModel>
#include <QFile>

#define VIDEOLISTMODEL_INITIALIZA                                               \
{                                                                       \
    qmlRegisterType<VideoListModel>("com.nuaa.CModel", 1, 0, "VideoListModel"); \
}

class VideoListModelPrivate;
class VideoListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    VideoListModel(QObject *parent = 0);
    ~VideoListModel();

    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QHash<int, QByteArray> roleNames() const;

    Q_INVOKABLE QVariant getPath(int index) const;
    Q_INVOKABLE void reload();
    Q_INVOKABLE bool deleteVideo(QString path) const;

private:
    VideoListModelPrivate *m_dptr;

};

#endif // VIDEOLISTMODEL_H
