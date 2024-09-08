#ifndef BASEVIEWER_H
#define BASEVIEWER_H

#include <QWidget>
#include <QList>
#include <QProgressBar>
#include <QTimer>
#include "widgets/basewidget.h"
#include "diskselectionwidget.h"
#include "utils/storageutils.h"
#include "utils/fileutils.h"
#include "utils/thumbnailutils.h"
#include "progressviewer.h"
#include "extstorageitem.h"
//#include "listeners/hotpluglistener.h"
#include "widgets/confirmdialog.h"
#include "widgets/viewer/qfilelistview.h"

class BaseViewer : public BaseWidget ,public Observer
{
    Q_OBJECT
public:
#define ICON_WIDTH  300
#define ICON_HEIGHT 230
#define X_OFFSET 	20
#define Y_OFFSET 	20
#define COLUMN_COUNT_ONE_LINE   4
#define ROW_COUNT_ONE_PAGE      3
    BaseViewer(QWidget *parent = nullptr);
    ~BaseViewer();
    virtual void resizeEvent(QResizeEvent *event);
    virtual void findAllFiles(const char *dir);
    virtual void update(NotifyData data) {if(isVisible()) onHotplugEvent();}

    void openDiskSelection();
    void loadThumbnail(QImage image,QString file_path);
    bool compareDisk(const char *path,QList<QString> fileList);
    bool isFileUtilsRun();
protected:
    QList<QString> mFilePathList;
    QList<QString> mFileNameList;
    typedef enum {
        FILE_TYPE_IMAGE,
        FILE_TYPE_VIDEO,
    }FileType;

    QList<QString> mSelectionlist;
    FileType mFileType;

    sp<DiskSelectionWidget> mDiskSelectionWidget;
    sp<ProgressViewer>      mProgressViewer;
    sp<FileUtils>           mFileUtils;
    sp<ThumbnailUtils>      mThumbnail;
    sp<QTimer>              mCapacityListenerTimer;
    //sp<HotplugListener>     mHotplugListener;

    sp<QLabel>       mlabel;
    QListWidget     *mListViewer;
    QScrollBar      *mScroll;
    QProgressBar    *mCapacityBar;
    QWidget         *mExtStorageWidget;
    StorageUtils    *mStorageUtils;
    QFileListView   *mFileListView;

    QVector<StorageUtils::ExternalStorageInfo> mExternalStorageInfo;
    QVector<sp<ExtStorageItem>> mitemVector;

    bool mSelectMode;
    int  mOperation;
    int  mLoadNum;
    int  mIconWidth;
    int  mIconHeight;

signals:
    void onHotplugEvent();

public slots:
    void onUpdateCapacity();
    void onUpdateExtStorageView();
};

#endif // BASEVIEWER_H
