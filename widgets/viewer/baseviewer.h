#ifndef BASEVIEWER_H
#define BASEVIEWER_H

#include <QWidget>
#include <QList>
#include "basewidget.h"

class BaseViewer : public BaseWidget
{
public:
    BaseViewer(QWidget *parent = nullptr);

    virtual void findAllFiles(const char *dir);

protected:
    QList<QString> mFileList;

    typedef enum {
        FILE_TYPE_IMAGE,
        FILE_TYPE_VIDEO,
    }FileType;

    FileType mFileType;
};

#endif // BASEVIEWER_H
