#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "streetwidget.h"
#include "preferenceswidget.h"
#include <QMainWindow>
#include <QGridLayout>
#include <QPropertyAnimation>
#include <QTimer>

enum View
{
	Street_View = 0x01,
	Preferences_View = 0x02
};

class MainWindow : public QMainWindow
{
	Q_OBJECT

	QWidget main_widget;
	QGridLayout main_layout;
	StreetWidget street;
	PreferencesWidget preferences;
	QPropertyAnimation preferences_animation;

	Configuration config;

	std::size_t current_view;

	void initGUI();

	const std::size_t ANIMATION_TIME = 1000;
	void show_preferences();
	void hide_preferences();

	QTimer resize_timer;
	void update_resize();

public:
	MainWindow(QWidget* parent = nullptr);

	void show();

	static constexpr std::size_t MIN_WIDTH = 800;
	static constexpr std::size_t MIN_HEIGHT = 450;

protected:
	void keyPressEvent(QKeyEvent* event);
	void resizeEvent(QResizeEvent*);
};

#endif // MAINWINDOW_H
