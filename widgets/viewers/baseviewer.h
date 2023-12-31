#ifndef BASEVIEWER_H
#define BASEVIEWER_H

#include <QWidget>
#include <QList>
#include "widgets/basewidget.h"
#include "progressviewer.h"
#include "diskselectionwidget.h"
#include "utils/storageutils.h"
#include "utils/fileutils.h"

class BaseViewer : public BaseWidget
{
    Q_OBJECT
public:
    BaseViewer(QWidget *parent = nullptr);
    void resizeEvent(QResizeEvent *event);
    virtual void findAllFiles();
    virtual void open();
    virtual void openDiskSelection();
protected:
#define ICON_WIDTH  200
#define ICON_HEIGHT 150
#define X_OFFSET 	20
#define Y_OFFSET 	20
    QList<QString> mFilePathList;
    QList<QString> mFileNameList;
    typedef enum {
        FILE_TYPE_IMAGE,
        FILE_TYPE_VIDEO,
    }FileType;

    bool mSelectMode;
    FileType mFileType;
    sp<ProgressViewer> mProgressViewer;
    QList<QString> mSelectionlist;
    QVector<StorageUtils::ExternalStorageInfo> mExternalStorageInfo;
    int mOperation;
    sp<DiskSelectionWidget> mDiskSelectionWidget;
    sp<StorageUtils> mStorageUtils;
    sp<FileUtils> mFileUtils;
};

#endif // BASEVIEWER_H
