#include "search.h"
#include <QFile>
#include <QDebug>

void Search::Manage::save(QString videoName)
{

    int idx = videoName.lastIndexOf(".");
    QString txtName = videoName.left(idx) + ".txt";
    qDebug() << txtName << "......................";

    QFile file(txtName);
    if(!file.open(QIODevice::ReadWrite))
        return;
    QTextStream out(&file);

    QString s1 = "Video File: " + videoName;
    out << s1 << "\r\n";
    out << "Detection Results: \r\n\r\n";

    out.setFieldWidth(15);
    out.setFieldAlignment(QTextStream::AlignCenter);
    out << "FrameID" << "Total" << "No hardhat" << "\r\n";

    int frameCount = df->getFrameCount();
    if(frameCount <= 0)
        return;
    for(int i = 0; i < frameCount; i++)
    {
        std::vector<bbox_t> results = df->getResult(i);
        int total = results.size();
        int nohat = 0;
        for(int k = 0; k < total; k++)
        {
            if(results[k].obj_id == 1)
                nohat++;
        }


        out << i  << total << nohat << "\r\n";

    }

    file.flush();
    file.close();
}
