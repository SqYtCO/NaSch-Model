// © Copyright (c) 2018 SqYtCO

#ifndef TOOLWIDGET_H
#define TOOLWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QToolButton>
#include <QSpinBox>
#include <QLabel>

// widget of tool view
// this widget is a toolbar which contains some useful tools
class ToolWidget : public QFrame
{
	Q_OBJECT

	// container of GUI components
	QHBoxLayout main_layout;

	// setup GUI and add components to main_layout
	void init_GUI();
	// init preferences, help, hide and quit button
	void init_control_buttons();
	// init tool buttons
	void init_tool_buttons();
	// init all other buttons
	void init_buttons();
	// init labels and spinbox
	void init_others();

	void play_or_stop();
	void swap_mouse_behavior();

public:
	// setup widget
	ToolWidget(QWidget* parent = nullptr);

	// compile time constants
	static constexpr std::size_t TOOL_HEIGHT = 35;
	static constexpr std::size_t MAXIMUM_GENERATIONS_PER_STEP_INPUT = 10000;

	// called when autogenerating is started/stopped to update play_stop button
	void update_play_stop_button();
	// update state displays
	void update_data();

	// set all texts; if a QTranslator is installed, the installed texts will be loaded
	void translate();

	// enable the focus of all children and the widget
	void enable_focus();
	// disable the focus of all children and the widget
	void disable_focus();

protected:
	void mousePressEvent(QMouseEvent*) { }

signals:
	// emitted on 'Esc'-key press and on hide button click; has to be connected to a slot which hides toolbar
	void hide_tool();
	// emitted on help button click; has to be connected to a slot which shows help
	void show_help();
	// emitted on preferences button click; has to be connected to a slot which shows preferences
	void show_preferences();
	// emitted on fullscreen button click; has to be connected to a slot which toggles fullscreen mode
	void fullscreen_changed();

// GUI components
private:
	QToolButton quit;
	QToolButton save;
	QToolButton open;
	QToolButton hide;
	QToolButton fullscreen;
	QToolButton preferences;
	QToolButton help;
	QToolButton new_game;
	QToolButton clear_all;
	QToolButton play_stop;
	QToolButton step;
	QToolButton car_tool;
	QToolButton speed_tool;
	QToolButton barrier_tool;
	QToolButton slow_down_tool;
	QToolButton reset_position;
	QLabel current_length;
	QLabel current_lanes;
	QLabel current_time;
	QLabel current_avg_speed;
	QLabel current_cars;
};

#endif // TOOLWIDGET_H
