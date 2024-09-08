#ifndef QFILELISTVIEW_H
#define QFILELISTVIEW_H

#include <QWidget>
#include "listview.h"
#include <QListWidgetItem>
#include "common/common.h"
#include "common/log.h"
#include "widgets/basewidget.h"
#include "utils/thumbnailutils.h"
#include "widgets/viewer/imagebrowser.h"
#include "widgets/viewer/videoplayer.h"

namespace Ui {
class QFileListView;
}

class QFileListView : public BaseWidget
{
    Q_OBJECT

public:
    typedef enum FileType {
        File_TypeNone,
        File_TypeImage,
        File_TypeVideo,
        FIle_TypeALL,
    } FileType_t;
    explicit QFileListView(QWidget *parent = nullptr);
    ~QFileListView();

    void setPath(QString dir_path) { mDirPath = dir_path; }

    void setIconSize(int icon_w, int icon_h);

    void updateDirView();

    void updateFileView();

    int  getCurrentPage();

    void setCurrentPage(int page);

    void setFileType(FileType_t type) { mFileType = type;}

	void setSelectionMode(bool mode);

	void delSelected();

	void delAll();
	
	QStringList getSelectedPathOfItems();
	QStringList getAllPathOfItems();
private:
    Ui::QFileListView *ui;

    sp<ListView> mDirList;
    sp<ListView> mFileList;
    QString mDirPath;

    int mIconWidth;
    int mIconHeight;
    bool mSelectMode;

    sp<ThumbnailUtils> mThumbnail;
	sp<VideoPlayer>	   mVideoPlayer;
    sp<ImageBrowser>   mImageBrowser;
    FileType_t mFileType;
public slots:
    virtual void onHasOpened();
    virtual void onHasClosed();

    void onDirItemClicked(QListWidgetItem *item);
    void onFileItemClicked(QListWidgetItem *item);
    void onLoadThumbnail(QImage image,QString file_path);
private:
    void updateDirList(QString dir_path);
    void updateFileList(QString dir_path);

	QListWidget *getCurrentPageOfListWidget();
	QList<QListWidgetItem*> getSelectedItems();
    QStringList findAllFiles(QString dir);
	
};

#endif // QFILELISTVIEW_H
