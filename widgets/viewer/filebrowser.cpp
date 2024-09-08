#include "filebrowser.h"
#include "ui_filebrowser.h"

FileBrowser::FileBrowser(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FileBrowser)
{
    ui->setupUi(this);

    mDirListView = sp<ListView>(new ListView(ListView::List_TypeDir, this));
    mFileListView = sp<ListView>(new ListView(ListView::List_TypeFile, this));

    ui->stackedWidget->addWidget(mDirListView.get());
    ui->stackedWidget->addWidget(mFileListView.get());
    ui->stackedWidget->setCurrentIndex(0);
}

FileBrowser::~FileBrowser()
{
    delete ui;
}
