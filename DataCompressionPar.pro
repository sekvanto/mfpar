QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

LIBS        += -lGL
LIBS        += -L/usr/lib64 -lmysqlcppconn
INCLUDEPATH += -I/usr/include/
INCLUDEPATH += -I/usr/include/jdbc/cppconn

RESOURCES += qdarkstyle/dark/darkstyle.qrc
RESOURCES += qdarkstyle/light/lightstyle.qrc

SOURCES += \
    benchmark.cpp \
    compress.cpp \
    decompress.cpp \
    edithistory.cpp \
    historydata.cpp \
    historyentry.cpp \
    main.cpp \
    mainwindow.cpp \
    par/algorithms/adaptive_huffman/adaptive_huffman.c \
    par/algorithms/huffman/heading.c \
    par/algorithms/huffman/huffman.c \
    par/archiver.c \
    par/common.c \
    par/data.c \
    par/parser.c \
    par/stats.c \
    par/utils/argparse.c \
    par/utils/linkedlist.c \
    par/utils/priority_queue.c \
    preferences.cpp

HEADERS += \
    benchmark.h \
    compress.h \
    decompress.h \
    defs.h \
    edithistory.h \
    historydata.h \
    historyentry.h \
    mainwindow.h \
    par/algorithms/adaptive_huffman/adaptive_huffman.h \
    par/algorithms/huffman/heading.h \
    par/algorithms/huffman/huffman.h \
    par/archiver.h \
    par/common.h \
    par/data.h \
    par/parser.h \
    par/stats.h \
    par/utils/argparse.h \
    par/utils/linkedlist.h \
    par/utils/priority_queue.h \
    preferences.h

FORMS += \
    benchmark.ui \
    compress.ui \
    decompress.ui \
    edithistory.ui \
    mainwindow.ui \
    preferences.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
