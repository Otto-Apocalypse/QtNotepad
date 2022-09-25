QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
greaterThan(QT_MAJOR_VERSION, 5): QT += core5compat

RC_ICONS = NotePad.ico

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    fontdlg.cpp \
    main.cpp \
    notepad.cpp \
    MyGlobalShortCut/MyWinEventFilter.cpp \
    MyGlobalShortCut/MyGlobalShortCut.cpp

HEADERS += \
    fontdlg.h \
    notepad.h \
    MyGlobalShortCut/MyWinEventFilter.h \
    MyGlobalShortCut/MyGlobalShortCut.h

FORMS += \
    fontdlg.ui \
    notepad.ui

TRANSLATIONS += \
    NotePad_zh_CN.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
