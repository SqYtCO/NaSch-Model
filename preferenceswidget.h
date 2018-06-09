#ifndef PREFERENCESWIDGET_H
#define PREFERENCESWIDGET_H

#include "configuration.h"
#include <QWidget>
#include <QGridLayout>
#include <QSpinBox>
#include <QLabel>

class PreferencesWidget : public QWidget
{
	Q_OBJECT

	QGridLayout main_layout;
	Configuration config;

	void initGUI();
	void init_size();
	void init_other();

public:
	PreferencesWidget(QWidget* parent = nullptr);

	void translate();
	void load_values();

	// enable the focus of all children and the widget
	void enable_focus();
	// disable the focus of all children and the widget
	void disable_focus();

public:
	QGridLayout size_layout;
	QLabel length_text;
	QSpinBox length_input;
	QLabel lanes_text;
	QSpinBox lanes_input;

	QGridLayout other_layout;
	QLabel slow_down_chance_text;
	QSpinBox slow_down_chance_input;
	QLabel slow_down_chance_unit;
	QLabel car_density_text;
	QSpinBox car_density_input;
	QLabel car_density_unit;
	QLabel max_speed_text;
	QSpinBox max_speed_input;
};

#endif // PREFERENCESWIDGET_H
