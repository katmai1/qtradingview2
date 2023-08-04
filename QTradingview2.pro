QT       += core gui webenginewidgets widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    customwebengineview.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    customwebengineview.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

TRANSLATIONS += \
    QTradingview2_es_ES.ts \
    QTradingview2_en_EN.ts

CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    recursos.qrc

DISTFILES += \
    QTradingview2_en_EN.ts
