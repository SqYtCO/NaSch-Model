#-------------------------------------------------
#
# Project created by QtCreator 2018-06-04T17:08:02
#
#-------------------------------------------------

QT += core gui widgets

# measure time of calculation while autogenerating in microseconds and output the result on the console
DEFINES += ENABLE_CALC_TIME_MEASUREMENT
# measure time of drawing cells in microseconds and output the result on the console
DEFINES += ENABLE_DRAW_TIME_MEASUREMENT

# enable some charts
DEFINES += CREATE_CHARTS

contains(DEFINES, CREATE_CHARTS): QT += charts

TARGET = nasch-model
TEMPLATE = app

CONFIG += c++14

DEFINES += QT_DEPRECATED_WARNINGS

gcc: LIBS += -lstdc++fs

SOURCES += \
        main.cpp \
        mainwindow.cpp \
	car_system.cpp \
    	configuration.cpp \
	streetwidget.cpp \
    	preferenceswidget.cpp \
	core.cpp \
	toolwidget.cpp \
	helpwidget.cpp \
	graphiccore.cpp \
	graphicconfiguration.cpp \
	startupdialog.cpp \
	infowidget.cpp \
	chartwidget.cpp

HEADERS += \
        mainwindow.h \
	car.h \
	car_system.h \
	configuration.h \
	streetwidget.h \
	preferenceswidget.h \
	running_configuration.h \
	core.h \
	toolwidget.h \
	helpwidget.h \
	graphiccore.h \
	graphicconfiguration.h \
	startupdialog.h \
	infowidget.h \
	chartwidget.h

RESOURCES += \
	resources.qrc

DISTFILES += \
	cut-off

TRANSLATIONS += \
	nasch_de.ts
