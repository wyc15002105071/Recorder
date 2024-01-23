/****************************************************************************
** Meta object code from reading C++ file 'videoframereader.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "media/videoframereader.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'videoframereader.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_VideoSurface_t {
    QByteArrayData data[5];
    char stringdata0[48];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_VideoSurface_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_VideoSurface_t qt_meta_stringdata_VideoSurface = {
    {
QT_MOC_LITERAL(0, 0, 12), // "VideoSurface"
QT_MOC_LITERAL(1, 13, 14), // "frameAvailable"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 12), // "QVideoFrame&"
QT_MOC_LITERAL(4, 42, 5) // "frame"

    },
    "VideoSurface\0frameAvailable\0\0QVideoFrame&\0"
    "frame"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_VideoSurface[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   19,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

       0        // eod
};

void VideoSurface::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<VideoSurface *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->frameAvailable((*reinterpret_cast< QVideoFrame(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (VideoSurface::*)(QVideoFrame & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoSurface::frameAvailable)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject VideoSurface::staticMetaObject = { {
    QMetaObject::SuperData::link<QAbstractVideoSurface::staticMetaObject>(),
    qt_meta_stringdata_VideoSurface.data,
    qt_meta_data_VideoSurface,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *VideoSurface::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *VideoSurface::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_VideoSurface.stringdata0))
        return static_cast<void*>(this);
    return QAbstractVideoSurface::qt_metacast(_clname);
}

int VideoSurface::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractVideoSurface::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void VideoSurface::frameAvailable(QVideoFrame & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
struct qt_meta_stringdata_VideoFrameReader_t {
    QByteArrayData data[13];
    char stringdata0[146];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_VideoFrameReader_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_VideoFrameReader_t qt_meta_stringdata_VideoFrameReader = {
    {
QT_MOC_LITERAL(0, 0, 16), // "VideoFrameReader"
QT_MOC_LITERAL(1, 17, 15), // "imagesAvailable"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 13), // "QList<QImage>"
QT_MOC_LITERAL(4, 48, 10), // "image_list"
QT_MOC_LITERAL(5, 59, 14), // "imageAvailable"
QT_MOC_LITERAL(6, 74, 5), // "image"
QT_MOC_LITERAL(7, 80, 12), // "ProcessFrame"
QT_MOC_LITERAL(8, 93, 12), // "QVideoFrame&"
QT_MOC_LITERAL(9, 106, 5), // "frame"
QT_MOC_LITERAL(10, 112, 7), // "onError"
QT_MOC_LITERAL(11, 120, 19), // "QMediaPlayer::Error"
QT_MOC_LITERAL(12, 140, 5) // "error"

    },
    "VideoFrameReader\0imagesAvailable\0\0"
    "QList<QImage>\0image_list\0imageAvailable\0"
    "image\0ProcessFrame\0QVideoFrame&\0frame\0"
    "onError\0QMediaPlayer::Error\0error"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_VideoFrameReader[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x06 /* Public */,
       5,    1,   37,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    1,   40,    2, 0x0a /* Public */,
      10,    1,   43,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::QImage,    6,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void, 0x80000000 | 11,   12,

       0        // eod
};

void VideoFrameReader::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<VideoFrameReader *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->imagesAvailable((*reinterpret_cast< QList<QImage>(*)>(_a[1]))); break;
        case 1: _t->imageAvailable((*reinterpret_cast< QImage(*)>(_a[1]))); break;
        case 2: _t->ProcessFrame((*reinterpret_cast< QVideoFrame(*)>(_a[1]))); break;
        case 3: _t->onError((*reinterpret_cast< QMediaPlayer::Error(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QList<QImage> >(); break;
            }
            break;
        case 3:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QMediaPlayer::Error >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (VideoFrameReader::*)(QList<QImage> );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoFrameReader::imagesAvailable)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (VideoFrameReader::*)(QImage );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoFrameReader::imageAvailable)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject VideoFrameReader::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_VideoFrameReader.data,
    qt_meta_data_VideoFrameReader,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *VideoFrameReader::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *VideoFrameReader::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_VideoFrameReader.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int VideoFrameReader::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void VideoFrameReader::imagesAvailable(QList<QImage> _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void VideoFrameReader::imageAvailable(QImage _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
