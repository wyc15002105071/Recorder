#include "osdutils.h"
#include <QPainter>
#include <QFont>
#include <QPixmapCache>

OSDUtils::OSDUtils()
{

}

void OSDUtils::createOSD(QPixmap &pm, const QString &text)
{
    if(pm.size().width() == 0 || pm.size().height() == 0) {
        pm = QPixmap(OSD_DEFAULT_WIDTH,OSD_DEFAULT_HEIGHT);
    }

    pm.fill(Qt::transparent);
    QPainter painter(&pm);
    QRectF textRect(10, 0, pm.width(), pm.height());
//    QString ptext = "Styled Text";
    QColor color(QColor(OSD_COLOR));
    int fontSize = OSD_FONT_SIZE, spacing = 1;
    QFont font(OSD_FONT, fontSize, QFont::Thin);
    font.setBold(true);
    font.setLetterSpacing(QFont::AbsoluteSpacing, spacing);
    QPen pen(color); // 设置画笔颜色
    painter.setFont(font);
    painter.setPen(pen);

    painter.drawText(textRect, Qt::AlignCenter, text);
}

void OSDUtils::destroyOSD(QPixmap &pm)
{
    pm.detach();
    QPixmapCache::clear();
}
