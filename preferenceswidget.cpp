#include "preferenceswidget.h"
#include <QKeyEvent>

PreferencesWidget::PreferencesWidget(QWidget* parent) : QWidget(parent)
{
	this->setAutoFillBackground(true);
	this->setPalette(QPalette(Qt::darkGray));

	initGUI();

	this->setFocusProxy(parent);
}

void PreferencesWidget::initGUI()
{
	init_size();
	init_other();

	main_layout.addLayout(&size_layout, 0, 0);
	main_layout.addLayout(&other_layout, 1, 0);

	// set texts
	translate();
	// set values
	load_values();

	this->setLayout(&main_layout);
}

void PreferencesWidget::init_size()
{
	length_input.setMinimum(1);
	length_input.setMaximum(10000);
	QObject::connect(&length_input, &QSpinBox::editingFinished, [this]() { config.set_street_length(length_input.value()); });
	lanes_input.setMinimum(1);
	lanes_input.setMaximum(10000);
	QObject::connect(&lanes_input, &QSpinBox::editingFinished, [this]() { config.set_street_lanes(lanes_input.value()); });
	size_layout.addWidget(&length_text, 0, 0);
	size_layout.addWidget(&length_input, 0, 1);
	size_layout.addWidget(&lanes_text, 1, 0);
	size_layout.addWidget(&lanes_input, 1, 1);
}

void PreferencesWidget::init_other()
{
	car_density_input.setMinimum(0);
	car_density_input.setMaximum(100);
	QObject::connect(&car_density_input, &QSpinBox::editingFinished, [this]() { config.set_car_density(car_density_input.value()); });

	slow_down_chance_input.setMinimum(0);
	slow_down_chance_input.setMaximum(100);
	QObject::connect(&slow_down_chance_input, &QSpinBox::editingFinished, [this]() { config.set_slow_down_chance(slow_down_chance_input.value()); });

	max_speed_input.setMinimum(1);
	max_speed_input.setMaximum(1000);
	QObject::connect(&max_speed_input, &QSpinBox::editingFinished, [this]() { config.set_max_speed(max_speed_input.value()); });

	other_layout.addWidget(&car_density_text, 0, 0);
	other_layout.addWidget(&car_density_input, 0, 1);
	other_layout.addWidget(&car_density_unit, 0, 2);
	other_layout.addWidget(&slow_down_chance_text, 1, 0);
	other_layout.addWidget(&slow_down_chance_input, 1, 1);
	other_layout.addWidget(&slow_down_chance_unit, 1, 2);
	other_layout.addWidget(&max_speed_text, 2, 0);
	other_layout.addWidget(&max_speed_input, 2, 1);
}

void PreferencesWidget::translate()
{
	length_text.setText(tr("Street Length"));
	lanes_text.setText(tr("Street Lanes"));

	car_density_text.setText(tr("Car Density"));
	car_density_unit.setText("%");

	slow_down_chance_text.setText(tr("Propability Of Deceleration"));
	slow_down_chance_unit.setText("%");

	max_speed_text.setText(tr("Maximum Speed"));
}

void PreferencesWidget::load_values()
{
	length_input.setValue(config.get_street_length());
	lanes_input.setValue(config.get_street_lanes());
	car_density_input.setValue(config.get_car_density());
	slow_down_chance_input.setValue(config.get_slow_down_chance());
	max_speed_input.setValue(config.get_max_speed());
}

void PreferencesWidget::enable_focus()
{
	// allow focus to all children
	for(QObject* a : this->children())
	{
		auto b = dynamic_cast<QWidget*>(a);
		if(b)
			b->setFocusPolicy(Qt::StrongFocus);
	}

	// enable own focus
	this->setFocusPolicy(Qt::StrongFocus);
}

void PreferencesWidget::disable_focus()
{
	// revoke focus of all children
	for(QObject* a : this->children())
	{
		auto b = dynamic_cast<QWidget*>(a);
		if(b)
			b->setFocusPolicy(Qt::NoFocus);
	}

	// disable own focus
	this->setFocusPolicy(Qt::NoFocus);
}
