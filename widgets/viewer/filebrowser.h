#ifndef FILEBROWSER_H
#define FILEBROWSER_H

#include <QWidget>
#include "listview.h"
#include <QListWidgetItem>
#include "common/common.h"
#include "common/log.h"

namespace Ui {
class FileBrowser;
}

class FileBrowser : public QWidget
{
    Q_OBJECT

public:
    explicit FileBrowser(QWidget *parent = nullptr);
    ~FileBrowser();

private:
    Ui::FileBrowser *ui;

    sp<ListView> mDirListView;
    sp<ListView> mFileListView;
};

#endif // FILEBROWSER_H
