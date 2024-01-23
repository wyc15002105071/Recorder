/****************************************************************************
** Meta object code from reading C++ file 'imageviewer.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "widgets/viewer/imageviewer.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'imageviewer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ImageViewer_t {
    QByteArrayData data[20];
    char stringdata0[258];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ImageViewer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ImageViewer_t qt_meta_stringdata_ImageViewer = {
    {
QT_MOC_LITERAL(0, 0, 11), // "ImageViewer"
QT_MOC_LITERAL(1, 12, 11), // "onHasOpened"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 11), // "onHasClosed"
QT_MOC_LITERAL(4, 37, 13), // "onItemClicked"
QT_MOC_LITERAL(5, 51, 16), // "QListWidgetItem*"
QT_MOC_LITERAL(6, 68, 4), // "item"
QT_MOC_LITERAL(7, 73, 19), // "onSelectModeToggled"
QT_MOC_LITERAL(8, 93, 7), // "toggled"
QT_MOC_LITERAL(9, 101, 21), // "onCopySelectedClicked"
QT_MOC_LITERAL(10, 123, 16), // "onCopyAllClicked"
QT_MOC_LITERAL(11, 140, 18), // "onDelSelectClicked"
QT_MOC_LITERAL(12, 159, 15), // "onDelAllClicked"
QT_MOC_LITERAL(13, 175, 17), // "onDiskItemClicked"
QT_MOC_LITERAL(14, 193, 5), // "index"
QT_MOC_LITERAL(15, 199, 20), // "onScrollValueChanged"
QT_MOC_LITERAL(16, 220, 5), // "value"
QT_MOC_LITERAL(17, 226, 15), // "onLoadThumbnail"
QT_MOC_LITERAL(18, 242, 5), // "image"
QT_MOC_LITERAL(19, 248, 9) // "file_path"

    },
    "ImageViewer\0onHasOpened\0\0onHasClosed\0"
    "onItemClicked\0QListWidgetItem*\0item\0"
    "onSelectModeToggled\0toggled\0"
    "onCopySelectedClicked\0onCopyAllClicked\0"
    "onDelSelectClicked\0onDelAllClicked\0"
    "onDiskItemClicked\0index\0onScrollValueChanged\0"
    "value\0onLoadThumbnail\0image\0file_path"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ImageViewer[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   69,    2, 0x0a /* Public */,
       3,    0,   70,    2, 0x0a /* Public */,
       4,    1,   71,    2, 0x0a /* Public */,
       7,    1,   74,    2, 0x0a /* Public */,
       9,    0,   77,    2, 0x0a /* Public */,
      10,    0,   78,    2, 0x0a /* Public */,
      11,    0,   79,    2, 0x0a /* Public */,
      12,    0,   80,    2, 0x0a /* Public */,
      13,    1,   81,    2, 0x0a /* Public */,
      15,    1,   84,    2, 0x0a /* Public */,
      17,    2,   87,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void, QMetaType::Bool,    8,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   14,
    QMetaType::Void, QMetaType::Int,   16,
    QMetaType::Void, QMetaType::QImage, QMetaType::QString,   18,   19,

       0        // eod
};

void ImageViewer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ImageViewer *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onHasOpened(); break;
        case 1: _t->onHasClosed(); break;
        case 2: _t->onItemClicked((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 3: _t->onSelectModeToggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->onCopySelectedClicked(); break;
        case 5: _t->onCopyAllClicked(); break;
        case 6: _t->onDelSelectClicked(); break;
        case 7: _t->onDelAllClicked(); break;
        case 8: _t->onDiskItemClicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->onScrollValueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->onLoadThumbnail((*reinterpret_cast< QImage(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ImageViewer::staticMetaObject = { {
    QMetaObject::SuperData::link<BaseViewer::staticMetaObject>(),
    qt_meta_stringdata_ImageViewer.data,
    qt_meta_data_ImageViewer,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ImageViewer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ImageViewer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ImageViewer.stringdata0))
        return static_cast<void*>(this);
    return BaseViewer::qt_metacast(_clname);
}

int ImageViewer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = BaseViewer::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 11;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
