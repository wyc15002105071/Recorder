#ifndef MEDIAPATHUTILS_H
#define MEDIAPATHUTILS_H
#include "media/mediautils.h"
#include <QString>

class MediaPathUtils
{

public:
    static MediaPathUtils *get_instance() {
        static MediaPathUtils utils;
        return &utils;
    }

    QString getRootPath();
    QString getRootVideoPath();
    QString getRootImagePath();

protected:
    MediaPathUtils();
private:
    const char *getMountPath(const char *node_path);
};

#endif // MEDIAPATHUTILS_H
