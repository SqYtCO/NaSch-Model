#-------------------------------------------------
#
# Project created by QtCreator 2018-06-04T17:08:02
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = nasch-model
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
        main.cpp \
        mainwindow.cpp \
	    car_system.cpp \
    	configuration.cpp \
	    streetwidget.cpp \
    	preferenceswidget.cpp

HEADERS += \
        mainwindow.h \
	    car.h \
    car_system.h \
    configuration.h \
    streetwidget.h \
    preferenceswidget.h

RESOURCES += \
    resources.qrc
