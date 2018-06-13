#ifndef PREFERENCESWIDGET_H
#define PREFERENCESWIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QToolButton>
#include <QSpinBox>
#include <QCheckBox>
#include <QLabel>

class PreferencesWidget : public QFrame
{
	Q_OBJECT

	QGridLayout main_layout;

	bool new_game_required;

	void init_GUI();
	void init_control();
	void init_language();
	void init_game();
	void init_appearance();

public:
	PreferencesWidget(QWidget* parent = nullptr);

	void translate();
	void load_values();

	void apply();
	void discard();
	void reset();

	// enable the focus of all children and the widget
	void enable_focus();
	// disable the focus of all children and the widget
	void disable_focus();

protected:
	void mousePressEvent(QMouseEvent*) { }

signals:
	void language_changed();
	void hide_preferences();

// GUI components
private:
	// control buttons
	QToolButton hide_button;
	QToolButton apply_button;
	QToolButton discard_button;
	QToolButton reset_button;

	// language buttons
	QHBoxLayout language_layout;
	QToolButton english_button;
	QToolButton german_button;

	QGridLayout game_layout;
	QLabel length_text;
	QSpinBox length_input;
	QLabel lanes_text;
	QSpinBox lanes_input;
	QLabel slow_down_chance_text;
	QSpinBox slow_down_chance_input;
	QLabel slow_down_chance_unit;
	QLabel car_density_text;
	QSpinBox car_density_input;
	QLabel car_density_unit;
	QLabel max_speed_text;
	QSpinBox max_speed_input;
	QLabel delay_text;
	QSpinBox delay_input;

	QGridLayout appearance_layout;
	QCheckBox break_long_streets_input;
	QCheckBox show_speed_color_input;
	QCheckBox  show_cars_input;
	QCheckBox  exit_warning_input;
	QCheckBox  fullscreen_input;
};

#endif // PREFERENCESWIDGET_H
