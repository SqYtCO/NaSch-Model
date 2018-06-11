#include "infowidget.h"
#include "core.h"

InfoWidget::InfoWidget(QWidget* parent) : QFrame(parent)
{
	// grey transparent background
	this->setAutoFillBackground(true);
	this->setPalette(QPalette(QColor(0x40, 0x40, 0x40, 0xE9)));

	// dark border around widget
	this->setFrameShadow(QFrame::Raised);
	this->setFrameShape(QFrame::Panel);
	this->setLineWidth(2);

	init_GUI();

	this->setFocusProxy(nullptr);
}

void InfoWidget::init_GUI()
{
	avg_speed.setAlignment(Qt::AlignCenter);
	cars.setAlignment(Qt::AlignCenter);
	lanes.setAlignment(Qt::AlignCenter);
	length.setAlignment(Qt::AlignCenter);
	time.setAlignment(Qt::AlignCenter);

	main_layout.addWidget(&avg_speed);
	main_layout.addWidget(&cars);
	main_layout.addWidget(&time);
	main_layout.addWidget(&length);
	main_layout.addWidget(&lanes);

	translate();
	update_data();

	this->setLayout(&main_layout);
}

void InfoWidget::translate()
{
	avg_speed.setToolTip(tr("Average Speed"));
	cars.setToolTip(tr("Amount Of Cars"));
	lanes.setToolTip(tr("Street Lanes"));
	length.setToolTip(tr("Street Length"));
	time.setToolTip(tr("Elapsed Time"));
	update_data();
}

void InfoWidget::update_data()
{
	avg_speed.setText(tr("Avg. Speed: ") + QString::number(Core::get_instance()->get_avg_speed(), 'g', 4));
	cars.setText(tr("Cars: ") + QString::number(Core::get_instance()->get_car_amount()));
	lanes.setText(tr("Lanes: ") + QString::number(Core::get_instance()->get_lanes()));
	length.setText(tr("Length: ") + QString::number(Core::get_instance()->get_length()));
	time.setText(tr("Time: ") + QString::number(Core::get_instance()->get_time()));
}

void InfoWidget::enable_focus()
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

void InfoWidget::disable_focus()
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
