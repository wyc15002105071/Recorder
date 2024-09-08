#ifndef LISTVIEW_H
#define LISTVIEW_H

#include <QWidget>
#include <QListWidget>

namespace Ui {
class ListView;
}

class ListView : public QWidget
{
    Q_OBJECT

public:
    typedef enum {
        List_TypeDir,
        List_TypeFile,
        List_TypeAll,
    } ListType;

    explicit ListView(ListType type, QWidget *parent = nullptr);
    ~ListView();

    QListWidget *list_ui();
private:
    Ui::ListView *ui;

    ListType mListType;

    QListWidget *mList;

signals:
    void itemClicked(QListWidgetItem* item);

public slots:
    void onItemClicked(QListWidgetItem* item);
};

#endif // LISTVIEW_H
