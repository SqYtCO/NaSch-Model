// © Copyright (c) 2018 SqYtCO

#include "toolwidget.h"
#include "graphiccore.h"
#include "core.h"
#include <QApplication>		// get global keyboard modifier
#include <QKeyEvent>
#include <QFileDialog>

ToolWidget::ToolWidget(QWidget* parent) : QFrame(parent)
{
	// grey transparent background
	this->setAutoFillBackground(true);
	this->setPalette(QPalette(QColor(0x40, 0x40, 0x40, 0xE9)));

	// dark border around widget
	this->setFrameShadow(QFrame::Raised);
	this->setFrameShape(QFrame::Panel);
	this->setLineWidth(2);

	init_gui();

	this->setFocusProxy(parent);

	this->setFixedHeight(TOOL_HEIGHT);
}

void ToolWidget::init_gui()
{
	init_control_buttons();
	init_tool_buttons();
	init_buttons();
	init_others();

	// add widgets in right order to layout
	main_layout.addWidget(&quit);
	main_layout.addStretch();
	main_layout.addWidget(&save);
	main_layout.addWidget(&open);
	main_layout.addStretch();
	main_layout.addWidget(&new_game);
	main_layout.addWidget(&clear_all);
	main_layout.addStretch();
	main_layout.addWidget(&play_stop);
	main_layout.addWidget(&step);
	main_layout.addStretch();
	main_layout.addWidget(&car_tool);
	main_layout.addWidget(&speed_tool);
	main_layout.addWidget(&barrier_tool);
	main_layout.addWidget(&slow_down_tool);
	main_layout.addStretch();
	main_layout.addWidget(&reset_position);
	main_layout.addStretch();
	main_layout.addWidget(&current_avg_speed);
	main_layout.addStretch();
	main_layout.addWidget(&current_cars);
	main_layout.addStretch();
	main_layout.addWidget(&current_time);
	main_layout.addStretch();
	main_layout.addWidget(&current_length);
	main_layout.addStretch();
	main_layout.addWidget(&current_lanes);
	main_layout.addStretch();
	main_layout.addWidget(&fullscreen);
	main_layout.addWidget(&preferences);
	main_layout.addWidget(&help);
	main_layout.addWidget(&hide);

	translate();

	main_layout.setMargin(1);


	// set layout of toolwidget
	this->setLayout(&main_layout);
}

void ToolWidget::init_control_buttons()
{
	// init preferences button
	preferences.setIcon(QIcon(":/images/preferences-90.png"));
	QObject::connect(&preferences, &QToolButton::clicked, [this]() { emit show_preferences(); });

	// init hide button
	hide.setArrowType(Qt::UpArrow);
	QObject::connect(&hide, &QToolButton::clicked, [this]() { emit hide_tool(); });

	// init help button
	help.setIcon(QIcon(":/images/help-90.png"));
	QObject::connect(&help, &QToolButton::clicked, [this]() { emit show_help(); });

	// connect quit button
	QObject::connect(&quit, &QToolButton::clicked, []() { qApp->quit(); });
}

void ToolWidget::init_tool_buttons()
{
	car_tool.setIcon(QIcon(":/images/car-white-200.png"));
	auto set_down_tool_buttons = [this](Tool tool)
	{
		car_tool.setDown(tool == Car_Tool);
		speed_tool.setDown(tool == Speed_Tool);
		barrier_tool.setDown(tool == Barrier_Tool);
		slow_down_tool.setDown(tool == Slow_Down_Tool);
	};
	QObject::connect(&car_tool, &QToolButton::clicked, [set_down_tool_buttons]() { GraphicCore::get_run_config()->set_tool(Car_Tool); set_down_tool_buttons(Car_Tool); });
	speed_tool.setIcon(QIcon(":/images/arrow-up-200.png"));
	QObject::connect(&speed_tool, &QToolButton::clicked, [set_down_tool_buttons]() { GraphicCore::get_run_config()->set_tool(Speed_Tool); set_down_tool_buttons(Speed_Tool); });
	QObject::connect(&barrier_tool, &QToolButton::clicked, [set_down_tool_buttons]() { GraphicCore::get_run_config()->set_tool(Barrier_Tool); set_down_tool_buttons(Barrier_Tool); });
	QObject::connect(&slow_down_tool, &QToolButton::clicked, [set_down_tool_buttons]() { GraphicCore::get_run_config()->set_tool(Slow_Down_Tool); set_down_tool_buttons(Slow_Down_Tool); });
}

void ToolWidget::init_buttons()
{
	// connect save button; text will be set in translate()
	QObject::connect(&save, &QToolButton::clicked, []() { GraphicCore::save(); });

	// connect open button; text will be set in translate()
	QObject::connect(&open, &QToolButton::clicked, []()
	{
		QString file = QFileDialog::getOpenFileName();
		if(!file.isEmpty())
			GraphicCore::load(file.toStdString());
	});

	// init new button
	QObject::connect(&new_game, &QToolButton::clicked, [this]() { GraphicCore::new_game(); update_data(); });

	// init clear button: on click all cells will be dead; on click + CTRL all cells will be alive
	QObject::connect(&clear_all, &QToolButton::clicked, [this]()
	{
		GraphicCore::reset();
		update_data();
	});

	// init play/stop button
	play_stop.setIcon(QIcon(":/images/play-90.png"));
	QObject::connect(&play_stop, &QToolButton::clicked, this, &ToolWidget::play_or_stop);

	// init reset_position; text will be set in translate()
	QObject::connect(&reset_position, &QToolButton::clicked, []() { GraphicCore::reset_position(); } );

	// init button for going one step forward
	step.setIcon(QIcon(":/images/play-to-90.png"));
	QObject::connect(&step, &QToolButton::clicked, []() { GraphicCore::step(); });

	fullscreen.setIcon(QIcon(":/images/fullscreen-90.png"));
	QObject::connect(&fullscreen, &QToolButton::clicked, [this]() { emit fullscreen_changed(); });
}

void ToolWidget::init_others()
{
	current_avg_speed.setAlignment(Qt::AlignCenter);
	current_lanes.setAlignment(Qt::AlignCenter);
	current_length.setAlignment(Qt::AlignCenter);
	current_time.setAlignment(Qt::AlignCenter);
}

void ToolWidget::translate()
{
	// control buttons
	new_game.setText(tr("New"));
	new_game.setToolTip(tr("New (N)"));
	clear_all.setText(tr("Clear"));
	clear_all.setToolTip(tr("Clear (A)/(CTRL+A)"));
	quit.setText(tr("Quit"));
	quit.setToolTip(tr("Quit (Q)"));
	save.setText(tr("Save"));
	save.setToolTip(tr("Save (S)"));
	open.setText(tr("Open"));
	open.setToolTip(tr("Open (O)"));
	hide.setToolTip(tr("Hide (Esc)"));
	preferences.setToolTip(tr("Preferences (P)"));
	help.setToolTip(tr("Help (H)"));

	// translate buttons
	update_play_stop_button();
	reset_position.setText(tr("Reset Position"));
	reset_position.setToolTip(tr("Reset Position (M)"));
	fullscreen.setToolTip(tr("Fullscreen (F)"));
	step.setToolTip(tr("Next Step (Spacebar)"));

	// translate other
	current_avg_speed.setToolTip(tr("Average Speed"));
	current_cars.setToolTip(tr("Amount Of Cars"));
	current_lanes.setToolTip(tr("Street Lanes"));
	current_length.setToolTip(tr("Street Length"));
	current_time.setToolTip(tr("Elapsed Time"));
	update_data();
}

void ToolWidget::update_play_stop_button()
{
	// if generating is running, set stop-icon
	if(GraphicCore::is_running())
	{
		play_stop.setIcon(QIcon(":/images/stop-90.png"));
		play_stop.setToolTip(tr("Stop (R)"));
	}
	// if generation is stopped, set play-icon
	else
	{
		play_stop.setIcon(QIcon(":/images/play-90.png"));
		play_stop.setToolTip(tr("Play (R)"));
	}
}

void ToolWidget::update_data()
{
	current_avg_speed.setText(tr("Avg. Speed: ") + QString::number(Core::get_avg_speed(), 'g', 4));
	current_cars.setText(tr("Cars: ") + QString::number(Core::get_car_amount()));
	current_lanes.setText(tr("Lanes: ") + QString::number(Core::get_lanes()));
	current_length.setText(tr("Length: ") + QString::number(Core::get_length()));
	current_time.setText(tr("Time: ") + QString::number(Core::get_time()));
}

void ToolWidget::play_or_stop()
{
	// if generating is running, stop it and set play-icon
	if(GraphicCore::is_running())
	{
		GraphicCore::stop();
		play_stop.setIcon(QIcon(":/images/play-90.png"));
		play_stop.setToolTip(tr("Play (R)"));
	}
	// if generation is stopped, start it and set stop-icon
	else
	{
		GraphicCore::start();
		play_stop.setIcon(QIcon(":/images/stop-90.png"));
		play_stop.setToolTip(tr("Stop (R)"));
	}
}

void ToolWidget::enable_focus()
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

void ToolWidget::disable_focus()
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
