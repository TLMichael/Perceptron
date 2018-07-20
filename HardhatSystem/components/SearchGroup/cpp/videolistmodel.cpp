#include "videolistmodel.h"
#include <QVector>
#include <QDebug>
#include "utils.h"

typedef QVector<QString> VideoData;
class VideoListModelPrivate
{
public:
    VideoListModelPrivate()
    {
        int role = Qt::UserRole;
        m_roleNames.insert(role++, "path");
        m_roleNames.insert(role++, "fps");
        m_roleNames.insert(role++, "frameCount");
        m_roleNames.insert(role++, "saveTime");
    }
    ~VideoListModelPrivate()
    {
        clear();
    }

    void load()
    {
        QList<QString> pathL;
        QList<float> fpsL;
        QList<int> frameCountL;
        QList<QDateTime> saveTimeL;
        int videoCount = df->searchVideo(pathL, fpsL, frameCountL, saveTimeL);
        qDebug() << "videoCount: " << videoCount;

        VideoData *video;
        for(int i = 0; i < videoCount; i++)
        {
            video = new VideoData();

            QString path = pathL.first();
            QString fps = QString::number(fpsL.first(), 10, 2);
            QString frameCount = QString::number(frameCountL.first(), 10);
            QString saveTime = saveTimeL.first().toString("yyyy-MM-dd hh:mm:ss");

            pathL.pop_front();
            fpsL.pop_front();
            frameCountL.pop_front();
            saveTimeL.pop_front();

            video->append(path);
            video->append(fps);
            video->append(frameCount);
            video->append(saveTime);

            m_videos.append(video);
            video = 0;
        }

        QVector<VideoData*> temp;
        QVector<VideoData*>::reverse_iterator riter;
        for(riter = m_videos.rbegin(); riter != m_videos.rend(); riter++)
            temp.push_back(*riter);
        m_videos = temp;
    }

    void reset()
    {
        clear();
    }

    void clear()
    {
        int count = m_videos.size();
        if(count > 0)
        {
            for(int i = 0; i < count; i++)
            {
                delete m_videos.at(i);
            }
            m_videos.clear();
        }
    }

    QHash<int, QByteArray> m_roleNames;
    QVector<VideoData*> m_videos;
};




VideoListModel::VideoListModel(QObject *parent)
    : QAbstractListModel(parent)
    , m_dptr(new VideoListModelPrivate)
{
    m_dptr->load();
}

VideoListModel::~VideoListModel()
{
    delete m_dptr;
}

int VideoListModel::rowCount(const QModelIndex &parent) const
{
    return m_dptr->m_videos.size();
}

QVariant VideoListModel::data(const QModelIndex &index, int role) const
{
    VideoData *d = m_dptr->m_videos[index.row()];
    return d->at(role - Qt::UserRole);
}

QVariant VideoListModel::getPath(int index) const
{
    VideoData *d = m_dptr->m_videos[index];
    return d->at(0);
}

void VideoListModel::reload()
{
    beginResetModel();
    m_dptr->reset();
    m_dptr->load();
    endResetModel();
}

bool VideoListModel::deleteVideo(QString path) const
{
    QFile file(path);
    if(file.exists())
        file.remove();
    return df->deleteVideo(path);
}

QHash<int, QByteArray> VideoListModel::roleNames() const
{
    return m_dptr->m_roleNames;
}

