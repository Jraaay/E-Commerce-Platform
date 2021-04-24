QT       += core gui
QT       += sql
QT       += quickwidgets
QT       += winextras

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    addproduct.cpp \
    loginandregister.cpp \
    main.cpp \
    product.cpp \
    productitem.cpp \
    productlistui.cpp \
    promptbox.cpp \
    rechargepage.cpp \
    sqlite.cpp \
    user.cpp \
    usercenter.cpp \
    widget.cpp

HEADERS += \
    addproduct.h \
    loginandregister.h \
    product.h \
    productitem.h \
    productlistui.h \
    promptbox.h \
    rechargepage.h \
    sqlite.h \
    user.h \
    usercenter.h \
    widget.h

FORMS += \
    addproduct.ui \
    product.ui \
    productlistui.ui \
    promptbox.ui \
    rechargepage.ui \
    usercenter.ui \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc

DISTFILES += \
    en_us.qm \
    zh_cn.qm

TRANSLATIONS = zh_cn.ts \
               en_us.ts \
               en_us.qm \
               zh_cn.qm
