#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "streetwidget.h"
#include "preferenceswidget.h"
#include <QMainWindow>
#include <QGridLayout>

class MainWindow : public QMainWindow
{
	Q_OBJECT

	QWidget main_widget;
	QGridLayout main_layout;
	StreetWidget street;
	PreferencesWidget preferences;

	void initGUI();

public:
	MainWindow(QWidget* parent = nullptr);

	static constexpr std::size_t MIN_WIDTH = 800;
	static constexpr std::size_t MIN_HEIGHT = 450;
};

#endif // MAINWINDOW_H
