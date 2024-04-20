QT       += core gui webenginewidgets widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    config.cpp \
    customwebengineview.cpp \
    src/dialogabout.cpp \
    src/dialogaddmarket.cpp \
    src/dialogoptions.cpp \
    main.cpp \
    mainwindow.cpp \
    src/marketslist.cpp

HEADERS += \
    config.h \
    customwebengineview.h \
    src/dialogabout.h \
    src/dialogaddmarket.h \
    src/dialogoptions.h \
    mainwindow.h \
    src/marketslist.h \
    src/systray.h \
    src/uimanager.h \
    version.h

FORMS += \
    src/dialogabout.ui \
    src/dialogaddmarket.ui \
    src/dialogoptions.ui \
    mainwindow.ui

TRANSLATIONS += \
    langs/QTradingview2_es_ES.ts \
    langs/QTradingview2_en_EN.ts \
    langs/QTradingview2_it_IT.ts \
    langs/QTradingview2_fr_FR.ts \
    langs/QTradingview2_ca_ES.ts

CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    recursos.qrc

DISTFILES +=
