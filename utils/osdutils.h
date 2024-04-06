#ifndef OSDUTILS_H
#define OSDUTILS_H

#include <QPixmap>
#include <QString>

#define OSD_COLOR   200, 200, 200, 170
#define OSD_FONT    "SimSun"
#define OSD_FONT_SIZE 40
#define OSD_DEFAULT_WIDTH  400
#define OSD_DEFAULT_HEIGHT 200

class OSDUtils
{
public:
    OSDUtils();

    static void createOSD(QPixmap &pm,const QString &text);
    static void destroyOSD(QPixmap &pm);
};

#endif // OSDUTILS_H
