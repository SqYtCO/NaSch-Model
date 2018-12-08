// © Copyright (c) 2018 SqYtCO

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "streetwidget.h"
#include "preferenceswidget.h"
#include "toolwidget.h"
#include "helpwidget.h"
#include "infowidget.h"
#include <QMainWindow>
#include <QGridLayout>
#include <QPropertyAnimation>
#include <QTranslator>
#include <QTimer>

// container and manager of all views
// manage signals, events and focus; animate view chanegs
// launch tranlsations
class MainWindow : public QMainWindow
{
	Q_OBJECT

	enum View
	{
		Street_View = 1,			// 0b00001
		Tool_View = 2,				// 0b00010
		Preferences_View = 4,		// 0b00100
		Help_View = 8,				// 0b01000
		Info_View = 16				// 0b10000
	};
	std::size_t current_view;						// contains currently active view; active views are stored as bitmask
	PreferencesWidget preferences_view;				// preferences; sliding in on the right side; shortcut: 'P'
	QPropertyAnimation preferences_animation;		// move-animation of preferences_view
	ToolWidget tool_view;							// tools; sliding in on the upper side; shortcut: 'T'
	QPropertyAnimation tool_animation;				// move-animation of tool_view
	HelpWidget help_view;							// help; sliding in on the left side; shortcut: 'H'
	QPropertyAnimation help_animation;				// move-animation of help_view
	InfoWidget info_view;
	QPropertyAnimation info_animation;

	const std::size_t ANIMATION_TIME = 1000;		// time for animations in ms
	void show_Street_View();							// hide preferences_view and help_view
	void show_preference_view();					// animate slide in of preferences_view; set Preferences_View bit
	void hide_preference_view();					// animate slide out of preferences_view; clear Preferences_View bit
	void show_tool_view();							// animate slide in of tool view; set Tool_View bit
	void hide_tool_view();							// animate slide out of tool view; clear Tool_View bit
	void show_help_view();							// animate slide in of help view; opengl is still able to receive events; set Help_View bit
	void hide_help_view();							// animate slide out of help view; clear Help_View bit
	void show_info_view();
	void hide_info_view();

	void init_gui();								// setup GUI; hide all views except Street_View

	QTranslator translator;							// translator for custom translation file (gol_XX.qm; XX=language)
	QTranslator qt_translator;						// translator for qt standard translation (qtbase_XX.qm; XX=language)
	void translate_application();					// load translations and translate all widgets

	QTimer resize_timer;							// call update_views_geometry() with delay; started in resizeEvent() (fix of fullscreen size bug)
	void update_views_geometry();					// update size and pos of views

public:
	// ctor: open start_file and init all views and animations; connect all signals translate to set language
	MainWindow(const char* start_file = nullptr, QWidget* parent = nullptr);
	virtual ~MainWindow() = default;

	// show maximized or in fullscreen in dependence of the set preferences; display also StartupDialog
	void show();

	// minimum width/height of window
	static constexpr std::size_t MIN_WIDTH = 1072;
	static constexpr std::size_t MIN_HEIGHT = 603;

protected:
	/* called on press of any key on keyboard
	 * shortcuts:
	 * G = hide preferences and help view
	 * P = show preferences view
	 * H = show help
	 * T = show tools
	 * Q = quit
	 */
	virtual void keyPressEvent(QKeyEvent* event) override;
	// update positions and sizes of views
	virtual void resizeEvent(QResizeEvent*) override;
	// show close dialogs (preferences and game save)
	virtual void closeEvent(QCloseEvent* event) override;
};

#endif // MAINWINDOW_H
