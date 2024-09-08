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
    QRectF textRect(0, 0, pm.width(), pm.height());
//    QString ptext = "Styled Text";
    QColor color(QColor(OSD_COLOR));
    int spacing = 1;
    int fontSize = 40;
    //int count  = text.count();

    //switch (count) {
    //case 8:fontSize = 35;break;
    //case 9:fontSize = 32;break;
    //case 10:fontSize = 28;break;
    //case 11:fontSize = 23;break;
    //case 12:
    //case 13:
    //case 14:fontSize = 18;break;
    //case 15:fontSize = 17;break;
    //case 16:fontSize = 16;break;
    //case 17:fontSize = 15;break;
    //case 18:fontSize = 14;break;
    //case 19:fontSize = 13;break;
    //case 20:fontSize = 12;break;
    //default:fontSize = 40;break;
    //}
    //if(count>20)fontSize = 10;
    QFont font(OSD_FONT, fontSize, QFont::Thin);
    font.setBold(true);
    font.setLetterSpacing(QFont::AbsoluteSpacing, spacing);
    QPen pen(color); // 设置画笔颜色
    painter.setFont(font);
    painter.setPen(pen);

    painter.drawText(textRect, Qt::AlignRight, text);
}

void OSDUtils::destroyOSD(QPixmap &pm)
{
    pm.detach();
    QPixmapCache::clear();
}
