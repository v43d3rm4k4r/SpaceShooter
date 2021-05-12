TEMPLATE = app
CONFIG += console1
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += -lwinmm

QMAKE_CXXFLAGS += -std=c++14 -pthread
LIBS += -pthread

SOURCES += \
        main.cpp

HEADERS += \
    GameEngine.h
