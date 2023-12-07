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
#define ICON_WIDTH  200
#define ICON_HEIGHT 200
#define X_OFFSET 20
#define Y_OFFSET 20
    QList<QString> mFileList;
    QList<QString> mFileNameList;
    typedef enum {
        FILE_TYPE_IMAGE,
        FILE_TYPE_VIDEO,
    }FileType;

    bool mSelectMode;
    FileType mFileType;
};

#endif // BASEVIEWER_H
