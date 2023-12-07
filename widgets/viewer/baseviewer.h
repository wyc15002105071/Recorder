#ifndef ABSTRACTVIEWER_H
#define ABSTRACTVIEWER_H

#include <QWidget>
#include <QList>
#include "media/mediautils.h"
class AbStractViewer : public QWidget
{
public:
    AbStractViewer(QWidget *parent = nullptr);

    virtual void open(){}
    virtual void findAllFiles(const char *dir);

protected:
    QList<QString>mFileList;

    typedef enum {
        FILE_TYPE_IMAGE,
        FILE_TYPE_VIDEO,
    }FileType;

    FileType mFileType;
};

#endif // ABSTRACTVIEWER_H
