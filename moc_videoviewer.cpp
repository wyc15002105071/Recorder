/****************************************************************************
** Meta object code from reading C++ file 'videoviewer.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "widgets/viewer/videoviewer.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'videoviewer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_VideoViewer_t {
    QByteArrayData data[18];
    char stringdata0[231];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_VideoViewer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_VideoViewer_t qt_meta_stringdata_VideoViewer = {
    {
QT_MOC_LITERAL(0, 0, 11), // "VideoViewer"
QT_MOC_LITERAL(1, 12, 11), // "onHasOpened"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 11), // "onHasClosed"
QT_MOC_LITERAL(4, 37, 13), // "onItemClicked"
QT_MOC_LITERAL(5, 51, 16), // "QListWidgetItem*"
QT_MOC_LITERAL(6, 68, 4), // "item"
QT_MOC_LITERAL(7, 73, 19), // "onSelectModeToggled"
QT_MOC_LITERAL(8, 93, 7), // "toggled"
QT_MOC_LITERAL(9, 101, 15), // "onLoadThumbnail"
QT_MOC_LITERAL(10, 117, 5), // "image"
QT_MOC_LITERAL(11, 123, 9), // "file_path"
QT_MOC_LITERAL(12, 133, 21), // "onCopySelectedClicked"
QT_MOC_LITERAL(13, 155, 16), // "onCopyAllClicked"
QT_MOC_LITERAL(14, 172, 18), // "onDelSelectClicked"
QT_MOC_LITERAL(15, 191, 15), // "onDelAllClicked"
QT_MOC_LITERAL(16, 207, 17), // "onDiskItemClicked"
QT_MOC_LITERAL(17, 225, 5) // "index"

    },
    "VideoViewer\0onHasOpened\0\0onHasClosed\0"
    "onItemClicked\0QListWidgetItem*\0item\0"
    "onSelectModeToggled\0toggled\0onLoadThumbnail\0"
    "image\0file_path\0onCopySelectedClicked\0"
    "onCopyAllClicked\0onDelSelectClicked\0"
    "onDelAllClicked\0onDiskItemClicked\0"
    "index"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_VideoViewer[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   64,    2, 0x0a /* Public */,
       3,    0,   65,    2, 0x0a /* Public */,
       4,    1,   66,    2, 0x0a /* Public */,
       7,    1,   69,    2, 0x0a /* Public */,
       9,    2,   72,    2, 0x0a /* Public */,
      12,    0,   77,    2, 0x0a /* Public */,
      13,    0,   78,    2, 0x0a /* Public */,
      14,    0,   79,    2, 0x0a /* Public */,
      15,    0,   80,    2, 0x0a /* Public */,
      16,    1,   81,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void, QMetaType::Bool,    8,
    QMetaType::Void, QMetaType::QImage, QMetaType::QString,   10,   11,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   17,

       0        // eod
};

void VideoViewer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<VideoViewer *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onHasOpened(); break;
        case 1: _t->onHasClosed(); break;
        case 2: _t->onItemClicked((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 3: _t->onSelectModeToggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->onLoadThumbnail((*reinterpret_cast< QImage(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 5: _t->onCopySelectedClicked(); break;
        case 6: _t->onCopyAllClicked(); break;
        case 7: _t->onDelSelectClicked(); break;
        case 8: _t->onDelAllClicked(); break;
        case 9: _t->onDiskItemClicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject VideoViewer::staticMetaObject = { {
    QMetaObject::SuperData::link<BaseViewer::staticMetaObject>(),
    qt_meta_stringdata_VideoViewer.data,
    qt_meta_data_VideoViewer,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *VideoViewer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *VideoViewer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_VideoViewer.stringdata0))
        return static_cast<void*>(this);
    return BaseViewer::qt_metacast(_clname);
}

int VideoViewer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = BaseViewer::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
