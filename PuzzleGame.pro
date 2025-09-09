QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    bmp/bmp.cpp \
    bmp/bmpfileheader.cpp \
    bmp/bmpinfoheader.cpp \
    defaultpuzzlemenuwindow.cpp \
    generatepuzzlemenuwindow.cpp \
    main.cpp \
    mainwindow.cpp \
    mainwindowpuzzle.cpp \
    puzzle/database.cpp \
    puzzle/databasemanager.cpp \
    puzzle/leaderboard.cpp \
    puzzle/pieceslist.cpp \
    puzzle/puzzlewidget.cpp

HEADERS += \
    bmp/bmp.h \
    bmp/bmpfileheader.h \
    bmp/bmpinfoheader.h \
    defaultpuzzlemenuwindow.h \
    generatepuzzlemenuwindow.h \
    mainwindow.h \
    mainwindowpuzzle.h \
    puzzle/database.h \
    puzzle/databasemanager.h \
    puzzle/leaderboard.h \
    puzzle/pieceslist.h \
    puzzle/puzzlewidget.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# CORRECTED RESOURCE PATH
RESOURCES += images/images.qrc
