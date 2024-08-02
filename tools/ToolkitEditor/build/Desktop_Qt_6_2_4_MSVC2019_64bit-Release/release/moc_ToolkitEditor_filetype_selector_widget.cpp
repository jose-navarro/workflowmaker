/****************************************************************************
** Meta object code from reading C++ file 'ToolkitEditor_filetype_selector_widget.hpp'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.2.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../src/ToolkitEditor_filetype_selector_widget.hpp"
#include <QtGui/qtextcursor.h>
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ToolkitEditor_filetype_selector_widget.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.2.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ToolkitEditor_filetype_selector_widget_t {
    const uint offsetsAndSize[16];
    char stringdata0[139];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_ToolkitEditor_filetype_selector_widget_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_ToolkitEditor_filetype_selector_widget_t qt_meta_stringdata_ToolkitEditor_filetype_selector_widget = {
    {
QT_MOC_LITERAL(0, 38), // "ToolkitEditor_filetype_select..."
QT_MOC_LITERAL(39, 24), // "description_text_changed"
QT_MOC_LITERAL(64, 0), // ""
QT_MOC_LITERAL(65, 8), // "the_text"
QT_MOC_LITERAL(74, 26), // "options_label_text_changed"
QT_MOC_LITERAL(101, 15), // "select_filetype"
QT_MOC_LITERAL(117, 12), // "row_selected"
QT_MOC_LITERAL(130, 8) // "rowIndex"

    },
    "ToolkitEditor_filetype_selector_widget\0"
    "description_text_changed\0\0the_text\0"
    "options_label_text_changed\0select_filetype\0"
    "row_selected\0rowIndex"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ToolkitEditor_filetype_selector_widget[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   38,    2, 0x09,    1 /* Protected */,
       4,    1,   41,    2, 0x09,    3 /* Protected */,
       5,    0,   44,    2, 0x09,    5 /* Protected */,
       6,    1,   45,    2, 0x09,    6 /* Protected */,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    7,

       0        // eod
};

void ToolkitEditor_filetype_selector_widget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ToolkitEditor_filetype_selector_widget *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->description_text_changed((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 1: _t->options_label_text_changed((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 2: _t->select_filetype(); break;
        case 3: _t->row_selected((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject ToolkitEditor_filetype_selector_widget::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_ToolkitEditor_filetype_selector_widget.offsetsAndSize,
    qt_meta_data_ToolkitEditor_filetype_selector_widget,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_ToolkitEditor_filetype_selector_widget_t
, QtPrivate::TypeAndForceComplete<ToolkitEditor_filetype_selector_widget, std::true_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<const QString &, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<const QString &, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>


>,
    nullptr
} };


const QMetaObject *ToolkitEditor_filetype_selector_widget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ToolkitEditor_filetype_selector_widget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ToolkitEditor_filetype_selector_widget.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int ToolkitEditor_filetype_selector_widget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 4;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
