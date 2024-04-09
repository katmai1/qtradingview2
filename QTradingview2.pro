QT       += core gui webenginewidgets widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    config.cpp \
    customwebengineview.cpp \
    dialogabout.cpp \
    dialogaddmarket.cpp \
    dialogoptions.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    config.h \
    customwebengineview.h \
    dialogabout.h \
    dialogaddmarket.h \
    dialogoptions.h \
    mainwindow.h \
    uimanager.h

FORMS += \
    dialogabout.ui \
    dialogaddmarket.ui \
    dialogoptions.ui \
    mainwindow.ui

TRANSLATIONS += \
    QTradingview2_es_ES.ts \
    QTradingview2_en_EN.ts \
    QTradingview2_it_IT.ts \
    QTradingview2_fr_FR.ts \
    QTradingview2_ca_ES.ts

CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    recursos.qrc

DISTFILES +=
