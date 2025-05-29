QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11


# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

INCLUDEPATH += /usr/local/include/gmssl
LIBS += -L/usr/local/lib -lgmssl -lcrypto




# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

TARGET = EncryptedTransfer
TEMPLATE = app

INCLUDEPATH += include

SOURCES += \
    main.cpp \
    src/encryptfilewindow.cpp \
    src/enrollwindow.cpp \
    src/filetransferwindow.cpp \
    src/historywindow.cpp \
    src/loginwindow.cpp \
    src/mainwindow.cpp \
    src/receivefilewindow.cpp \
    src/initdb.cpp \
    src/tcpclient.cpp \
    src/tcpserver.cpp

HEADERS += \
    include/encryptfilewindow.h \
    include/enrollwindow.h \
    include/filetransferwindow.h \
    include/historywindow.h \
    include/loginwindow.h \
    include/mainwindow.h \
    include/receivefilewindow.h \
    include/initdb.h \
    include/tcpclient.h \
    include/tcpserver.h \
    include/utils.h

FORMS += \
    ui/encryptfilewindow.ui \
    ui/enrollwindow.ui \
    ui/filetransferwindow.ui \
    ui/historywindow.ui \
    ui/loginwindow.ui \
    ui/mainwindow.ui \
    ui/receivefilewindow.ui


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
