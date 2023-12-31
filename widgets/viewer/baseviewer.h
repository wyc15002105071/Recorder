#ifndef BASEVIEWER_H
#define BASEVIEWER_H

#include <QWidget>
#include <QList>
#include "widgets/basewidget.h"
#include "diskselectionwidget.h"
#include "utils/storageutils.h"
#include "utils/fileutils.h"
#include "utils/thumbnailutils.h"
#include "progressviewer.h"

class BaseViewer : public BaseWidget
{
    Q_OBJECT
public:
#define ICON_WIDTH  250
#define ICON_HEIGHT 200
#define X_OFFSET 	20
#define Y_OFFSET 	20
#define COLUMN_COUNT_ONE_LINE   4
#define ROW_COUNT_ONE_PAGE      3
    BaseViewer(QWidget *parent = nullptr);
    ~BaseViewer();
    virtual void resizeEvent(QResizeEvent *event);
    virtual void findAllFiles(const char *dir);
    void openDiskSelection();

    void loadThumbnail(QPixmap pixmap,QString file_path);
protected:
    QList<QString> mFilePathList;
    QList<QString> mFileNameList;
    typedef enum {
        FILE_TYPE_IMAGE,
        FILE_TYPE_VIDEO,
    }FileType;

    FileType mFileType;

    sp<DiskSelectionWidget> mDiskSelectionWidget;
    sp<ProgressViewer>  mProgressViewer;
    sp<FileUtils>       mFileUtils;
    sp<ThumbnailUtils>  mThumbnail;

    QListWidget  *mListViewer;
    QScrollBar   *mScroll;
    StorageUtils *mStorageUtils;

    QVector<StorageUtils::ExternalStorageInfo> mExternalStorageInfo;
    QList<QString> mSelectionlist;

    bool mSelectMode;
    int  mOperation;
    int  mLoadNum;
    int  mIconWidth;
    int  mIconHeight;
};

#endif // BASEVIEWER_H
