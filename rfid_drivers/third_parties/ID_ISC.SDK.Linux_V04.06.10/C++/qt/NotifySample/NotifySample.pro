TEMPLATE = app
CONFIG += qt warn_on
unix:contains(QT_CONFIG, qdbus):CONFIG += qdbus
INCLUDEPATH +=  ../../../fecom-lib/include/ ../../../fetcp-lib/include/ ../../../feusb-lib/include/ ../../../fedm-classlib/src/
LIBS += -L/usr/lib -lFedmIscCore -lfeisc -lfecom -lfetcp -lfeusb

HEADERS     += src/NotifySampleDlg.h
RESOURCES   += res.qrc 
SOURCES     += src/main.cpp src/NotifySampleDlg.cpp
FORMS       += ui/NotifySampleDlg.ui

build_all:!build_pass {
    CONFIG -= build_all
    CONFIG += release
}

sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS *.pro images
sources.path = ISOHostSample
