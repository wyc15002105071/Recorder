#include "listwidgetitem.h"
#include "ui_listwidgetitem.h"
#include "common/log.h"
#include <QPixmap>

ListWidgetItem::ListWidgetItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ListWidgetItem),
    mSelectable(false)
{
    ui->setupUi(this);
}

void ListWidgetItem::resizeEvent(QResizeEvent *event)
{
    if(!mSelectionIcon) {
        mSelectionIcon = sp<SelectionIcon>(new SelectionIcon(this));
    }

    mSelectionIcon->move(ui->icon->width() - 20,ui->icon->y() + 20);
    mSelectionIcon->close();
}

ListWidgetItem::~ListWidgetItem()
{
    delete ui;
}

void ListWidgetItem::setFileName(QString file_name)
{
    ui->file_name->setText(file_name);
}

void ListWidgetItem::setIcon(QImage image)
{
    QImage icon_img(image);

    setIcon(QPixmap::fromImage(icon_img));
}

void ListWidgetItem::setIcon(QPixmap pixmap)
{
    QPixmap pix(pixmap);

    if(pix.isNull()) {
        pix = QPixmap(":/resources/icons/broken.png");
    }
    pix.scaled(ui->icon->size(),Qt::IgnoreAspectRatio);

    ui->icon->setPixmap(pix);

}

void ListWidgetItem::setAlignment(Qt::Alignment alignment)
{
    ui->icon->setAlignment(alignment);
}

void ListWidgetItem::setMovie(QMovie *movie)
{
    ui->icon->setMovie(movie);
}

void ListWidgetItem::setSelectable(bool selectable)
{
    mSelectable = selectable;
    if(mSelectionIcon) {
        if(mSelectable) {
            mSelectionIcon->show();
            mSelectionIcon->move(ui->icon->x() + ui->icon->width() - 30,ui->icon->y() + 5);
        } else {
            mSelectionIcon->close();
            mSelectionIcon->setSelected(false);
        }
    }
}

bool ListWidgetItem::isSelected()
{
    if(mSelectionIcon) {
        return mSelectionIcon->isSelected();
    }
    return false;
}

void ListWidgetItem::setSelected(bool selected)
{
    if(mSelectionIcon) {
        mSelectionIcon->setSelected(selected);
    }
}
