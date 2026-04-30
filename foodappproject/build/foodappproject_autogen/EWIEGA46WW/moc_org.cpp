/****************************************************************************
** Meta object code from reading C++ file 'org.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../org.h"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'org.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.4.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
namespace {
struct qt_meta_stringdata_OrgModule_t {
    uint offsetsAndSizes[18];
    char stringdata0[10];
    char stringdata1[16];
    char stringdata2[1];
    char stringdata3[18];
    char stringdata4[14];
    char stringdata5[17];
    char stringdata6[19];
    char stringdata7[15];
    char stringdata8[13];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_OrgModule_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_OrgModule_t qt_meta_stringdata_OrgModule = {
    {
        QT_MOC_LITERAL(0, 9),  // "OrgModule"
        QT_MOC_LITERAL(10, 15),  // "onRestaurantTab"
        QT_MOC_LITERAL(26, 0),  // ""
        QT_MOC_LITERAL(27, 17),  // "onOrganizationTab"
        QT_MOC_LITERAL(45, 13),  // "onDonationTab"
        QT_MOC_LITERAL(59, 16),  // "submitRestaurant"
        QT_MOC_LITERAL(76, 18),  // "submitOrganization"
        QT_MOC_LITERAL(95, 14),  // "submitDonation"
        QT_MOC_LITERAL(110, 12)   // "handleLogout"
    },
    "OrgModule",
    "onRestaurantTab",
    "",
    "onOrganizationTab",
    "onDonationTab",
    "submitRestaurant",
    "submitOrganization",
    "submitDonation",
    "handleLogout"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_OrgModule[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   56,    2, 0x08,    1 /* Private */,
       3,    0,   57,    2, 0x08,    2 /* Private */,
       4,    0,   58,    2, 0x08,    3 /* Private */,
       5,    0,   59,    2, 0x08,    4 /* Private */,
       6,    0,   60,    2, 0x08,    5 /* Private */,
       7,    0,   61,    2, 0x08,    6 /* Private */,
       8,    0,   62,    2, 0x08,    7 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject OrgModule::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_OrgModule.offsetsAndSizes,
    qt_meta_data_OrgModule,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_OrgModule_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<OrgModule, std::true_type>,
        // method 'onRestaurantTab'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onOrganizationTab'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onDonationTab'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'submitRestaurant'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'submitOrganization'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'submitDonation'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'handleLogout'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void OrgModule::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<OrgModule *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->onRestaurantTab(); break;
        case 1: _t->onOrganizationTab(); break;
        case 2: _t->onDonationTab(); break;
        case 3: _t->submitRestaurant(); break;
        case 4: _t->submitOrganization(); break;
        case 5: _t->submitDonation(); break;
        case 6: _t->handleLogout(); break;
        default: ;
        }
    }
    (void)_a;
}

const QMetaObject *OrgModule::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *OrgModule::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_OrgModule.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int OrgModule::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 7;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
