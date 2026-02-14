QT += core gui widgets xml

CONFIG += c++17

TARGET = OpenFF_GUI
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    presetmanager.cpp

HEADERS += \
    mainwindow.h \
    presetmanager.h
