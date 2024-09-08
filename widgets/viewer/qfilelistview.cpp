#include "qfilelistview.h"
#include "ui_qfilelistview.h"
#include "listwidgetitem.h"
#include "utils/fileutils.h"
#include <QScroller>
#include <QScrollBar>
#include <QDir>

#define MODULE_TAG "QFileListView"

QFileListView::QFileListView(QWidget *parent) :
    BaseWidget(parent),
    mIconWidth(0),
    mIconHeight(0),
    mSelectMode(false),
    mThumbnail(sp<ThumbnailUtils>(new ThumbnailUtils)),
    mFileType(File_TypeNone),
    ui(new Ui::QFileListView)
{
    ui->setupUi(this);

    mImageBrowser = sp<ImageBrowser>(new ImageBrowser);
	mVideoPlayer  = sp<VideoPlayer>(new VideoPlayer);

    mDirList  = sp<ListView>(new ListView(ListView::List_TypeDir, this));
    mFileList = sp<ListView>(new ListView(ListView::List_TypeFile, this));

    mDirList->list_ui()->verticalScrollBar()->setStyleSheet(mDirList->list_ui()->styleSheet());
    mDirList->list_ui()->setAttribute(Qt::WA_AcceptTouchEvents,true);

    mFileList->list_ui()->verticalScrollBar()->setStyleSheet(mFileList->list_ui()->styleSheet());
    mFileList->list_ui()->setAttribute(Qt::WA_AcceptTouchEvents,true);

    QScroller::grabGesture(mFileList->list_ui(),QScroller::TouchGesture);
    QScroller::grabGesture(mDirList->list_ui(),QScroller::TouchGesture);

    ui->stackedWidget->addWidget(mDirList.get());
    ui->stackedWidget->addWidget(mFileList.get());

    connect(mDirList->list_ui(), SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(onDirItemClicked(QListWidgetItem *)));
    connect(mFileList->list_ui(), SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(onFileItemClicked(QListWidgetItem *)));

    connect(mThumbnail.get(),SIGNAL(onGetOneImage(QImage,QString)),this,SLOT(onLoadThumbnail(QImage,QString)));
}

QFileListView::~QFileListView()
{
    delete ui;
}

void QFileListView::setIconSize(int icon_w, int icon_h)
{
    mIconWidth  = icon_w;
    mIconHeight = icon_h;
}

void QFileListView::updateDirView()
{
    RLOGD("--------update view----------");
    if(mDirList)
        mDirList->list_ui()->clear();
    if(mFileList)
        mFileList->list_ui()->clear();

    ui->stackedWidget->setCurrentIndex(0);

    updateDirList(mDirPath);
}

void QFileListView::updateFileView()
{
    if(mFileType == File_TypeNone)
        return;

    mFileList->list_ui()->clear();
//    if(mThumbnail.get()) {
//        mThumbnail->setThumbSize(mIconWidth - 20,mIconHeight - 20);
//        mThumbnail->setDataSource(mFilePathList);
//        mThumbnail->startTask();
//    }
}

int QFileListView::getCurrentPage()
{
    return ui->stackedWidget->currentIndex();
}

void QFileListView::setCurrentPage(int page)
{
    ui->stackedWidget->setCurrentIndex(page);
}

void QFileListView::setSelectionMode(bool mode)
{
	mSelectMode = mode;
	int page = getCurrentPage();
	QListWidget *list = getCurrentPageOfListWidget();

	if(!list)
		return;
	
	int count = list->count();
    if(count <= 0)
        return;
	
    for(int i = 0;i < count;i++) {
        QListWidgetItem *item = list->item(i);
        if(!item)
            continue;
        ListWidgetItem *item_widget = (ListWidgetItem *)list->itemWidget(item);
        item_widget->setSelectable(mSelectMode);
    }
}

QStringList QFileListView::getSelectedPathOfItems()
{
	QStringList path_list;

	int page = getCurrentPage();
	QListWidget *list = getCurrentPageOfListWidget();

	int count = list->count();
    if(count <= 0)
        return QStringList();
	
    for(int i = 0;i < count;i++) {
        QListWidgetItem *item = list->item(i);
        if(!item)
            continue;
        ListWidgetItem *item_widget = (ListWidgetItem *)list->itemWidget(item);
        if(item_widget->isSelected()) {
			path_list.push_back(item_widget->absolutePath());
		}
    }

	return path_list;
}

QStringList QFileListView::getAllPathOfItems()
{
	QStringList path_list;

	int page = getCurrentPage();
	QListWidget *list = getCurrentPageOfListWidget();

	int count = list->count();
    if(count <= 0)
        return QStringList();
	
    for(int i = 0;i < count;i++) {
        QListWidgetItem *item = list->item(i);
        if(!item)
            continue;
        ListWidgetItem *item_widget = (ListWidgetItem *)list->itemWidget(item);
        path_list.push_back(item_widget->absolutePath());
    }

	return path_list;
}

void QFileListView::delSelected()
{
	QList<QListWidgetItem*> del_list = getSelectedItems();
	QListWidget *list = getCurrentPageOfListWidget();
	
    while(del_list.count() > 0) {
        QListWidgetItem *item = del_list.front();
		
        int row = list->row(item);
        list->removeItemWidget(item);
        list->takeItem(row);
        del_list.pop_front();
    }

}

void QFileListView::delAll()
{
	QListWidget *list = getCurrentPageOfListWidget();
	list->clear();
}

QListWidget *QFileListView::getCurrentPageOfListWidget()
{
	QListWidget *list = nullptr;
	int page = getCurrentPage();
	if(page == 0) {
		list = mDirList->list_ui();
	} else if(page == 1) {
		list = mFileList->list_ui();
	}

	return list;
}

QList<QListWidgetItem*> QFileListView::getSelectedItems()
{
	QList<QListWidgetItem*> item_list;

	int page = getCurrentPage();
	QListWidget *list = getCurrentPageOfListWidget();

	int count = list->count();
    if(count <= 0)
        return QList<QListWidgetItem*>();
	
    for(int i = 0;i < count;i++) {
        QListWidgetItem *item = list->item(i);
        if(!item)
            continue;
        ListWidgetItem *item_widget = (ListWidgetItem *)list->itemWidget(item);
		if(item_widget->isSelected())
        	item_list.push_back(item);
    }

	return item_list;

}


void QFileListView::onLoadThumbnail(QImage image,QString file_path)
{
    QListWidgetItem *item = new QListWidgetItem;
    QImage file_icon;
    file_icon = image;
    if(image.isNull()) {
        QImage broken_image(":/resources/icons/broken.png");
        file_icon = broken_image.scaled(mIconWidth - 20,mIconHeight - 20,Qt::KeepAspectRatio);
    } else {
//        file_icon = file_icon.scaled(mIconWidth - X_OFFSET,mIconHeight - Y_OFFSET,Qt::KeepAspectRatio);
    }

    ListWidgetItem *custom_item = new ListWidgetItem;
    custom_item->setIcon(file_icon);
    custom_item->setFileName(FileUtils::pathToName(file_path));
    custom_item->setDirPath(FileUtils::pathToDirPath(file_path));
	custom_item->setAbsolutePath(file_path);
    custom_item->setAlignment(Qt::AlignCenter);
    custom_item->setSelectable(mSelectMode);
    item->setSizeHint(QSize(mIconWidth - 20,mIconHeight- 20));
    if(mFileList->list_ui()) {
        mFileList->list_ui()->addItem(item);
        mFileList->list_ui()->setItemWidget(item,custom_item);
    }

    file_icon.detach();
}

void QFileListView::onHasOpened()
{

}

void QFileListView::onHasClosed()
{

}

void QFileListView::onDirItemClicked(QListWidgetItem *item)
{
    ListWidgetItem *item_widget = (ListWidgetItem *)mDirList->list_ui()->itemWidget(item);

    if(mSelectMode) {
        if(!item_widget)
            return;
        bool is_selected = item_widget->isSelected();
        item_widget->setSelected(!is_selected);
    } else {
	    ListWidgetItem *item_widget = (ListWidgetItem *)mDirList->list_ui()->itemWidget(item);
	    QString file_name = item_widget->fileName();
	    mFileList->list_ui()->clear();
	    QString p = mDirPath + "/" + file_name;
	    QStringList file_list = findAllFiles(p);
	    if(mThumbnail.get()) {
	        mThumbnail->setThumbSize(mIconWidth - 20, mIconHeight - 20);
	        mThumbnail->setDataSource(file_list);
	        mThumbnail->startTask();
	    }

		ui->stackedWidget->setCurrentIndex(1);
    }

}

void QFileListView::onFileItemClicked(QListWidgetItem *item)
{
    QScroller *scroller = QScroller::scroller(mFileList->list_ui());

    if(scroller && scroller->state() != QScroller::Inactive)
        return;

    ListWidgetItem *item_widget = (ListWidgetItem *)mFileList->list_ui()->itemWidget(item);
    if(mSelectMode) {
        if(!item_widget)
            return;
        bool is_selected = item_widget->isSelected();
        item_widget->setSelected(!is_selected);
    } else {
        int current_index = mFileList->list_ui()->currentRow();

        QString dir_path = item_widget->dirPath();
        RLOGD("dir path is %s", dir_path.toLatin1().data());
		RLOGD("---current index is %d----", current_index);
		QStringList file_list = findAllFiles(dir_path);
		if(mFileType == File_TypeImage) {
	        if(mImageBrowser) {
	            mImageBrowser->showFullScreen();
	            mImageBrowser->openPlayer(file_list,current_index);
	        }
		} else if(mFileType == File_TypeVideo) {
			 if(mVideoPlayer) {
				RLOGD("---openPlayer file list %d current index %d----", file_list.count(), current_index);
	            mVideoPlayer->openPlayer(file_list,current_index);
			 	mVideoPlayer->showFullScreen();
	        }
		}
		

    }
}

void QFileListView::updateFileList(QString dir_path)
{

}

QStringList QFileListView::findAllFiles(QString dir)
{
    QStringList list;
    QDir file_dir(dir);
    RLOGD("-----------dir list %s", dir.toLatin1().data());

    if(!file_dir.exists())
        return list;
    QStringList nameFilter;
    if(mFileType == File_TypeImage)
        nameFilter<<"*.jpg"<<"*.png"<<"*.jfif";
    else if(mFileType == File_TypeVideo)
        nameFilter<<"*.mp4"<<"*.mkv"<<"*.avi";

    list.clear();
    for(auto file:file_dir.entryList(nameFilter))
    {
	if(file == "." || file == "..") continue;
        list.append(QString(dir)+"/"+file);
//        mFileNameList.append(file);
        RLOGD(" file %s ", file.toLatin1().data());
    }

    auto comp = [&](const QString &s1,const QString &s2)->bool {
        return (strcmp(s1.toLatin1().data(),s2.toLatin1().data()) >= 0)?1:0;
    };

    std::sort(list.begin(),list.end(),comp);
//    std::sort(mFileNameList.begin(),mFileNameList.end(),comp);
    return list;
}

void QFileListView::updateDirList(QString dir_path)
{
    if(dir_path.isNull() || dir_path.isEmpty())
        return;
    QDir directory(dir_path);

    QStringList directories = directory.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

    foreach (const QString &dir, directories) {
        QListWidgetItem *item = new QListWidgetItem;
        QImage file_icon(":/resources/icons/dir.png");

        file_icon = file_icon.scaled(mIconWidth - 20,mIconHeight - 20,Qt::KeepAspectRatio);

        ListWidgetItem *custom_item = new ListWidgetItem;
        custom_item->setIcon(file_icon);
		custom_item->setAbsolutePath(dir_path + "/" + dir);
        custom_item->setFileName(FileUtils::pathToName(dir));
        custom_item->setAlignment(Qt::AlignCenter);
        custom_item->setSelectable(mSelectMode);
        item->setSizeHint(QSize(mIconWidth - 20,mIconHeight- 20));
        if(mDirList) {
            mDirList->list_ui()->addItem(item);
            mDirList->list_ui()->setItemWidget(item,custom_item);
        }

        file_icon.detach();
    }
}
