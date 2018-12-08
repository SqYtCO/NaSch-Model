#include "streetwidget.h"
#include "graphiccore.h"
#include "core.h"
#include <QPaintEvent>
#include <QBitmap>
#include <cmath>						// std::ceil
#ifdef ENABLE_DRAW_TIME_MEASUREMENT
#include <QDebug>
#include <chrono>
#endif

StreetWidget::StreetWidget(QWidget* parent) : BASE_CLASS(parent), car_image(":/images/car-200.png"),
												car_colors{ Qt::blue, Qt::green, Qt::gray, Qt::yellow, Qt::darkMagenta, Qt::darkGreen, Qt::cyan, Qt::darkCyan, Qt::darkYellow,
														   Qt::darkGray, Qt::black },
												scale(1), move_x(0), move_y(0)
{
	this->setAutoFillBackground(true);
	this->setPalette(QPalette(Qt::black));
}

void StreetWidget::paintEvent(QPaintEvent*)
{
#ifdef ENABLE_DRAW_TIME_MEASUREMENT
	auto begin = std::chrono::high_resolution_clock::now();
#endif
	car_image = car_image.scaled(GraphicCore::get_instance()->get_config()->get_cell_size(), GraphicCore::get_instance()->get_config()->get_cell_size());

	QPainter painter(this);
	painter.setPen(Qt::black);

	painter.scale(scale, scale);

	for(std::size_t a = 0, y1 = 0; a < Core::get_instance()->get_lanes(); ++a, y1 += GraphicCore::get_instance()->get_config()->get_cell_size())
	{
		for(std::size_t b = 0, x = 0, y2 = 0; b < Core::get_instance()->get_length(); ++b, x += GraphicCore::get_instance()->get_config()->get_cell_size())
		{
			if(GraphicCore::get_instance()->get_config()->get_long_street_break())
			{
				if((x + GraphicCore::get_instance()->get_config()->get_cell_size()) * scale > static_cast<std::size_t>(width()))
				{
					y1 += GraphicCore::get_instance()->get_config()->get_cell_size();
					x = 0;
				}
			}

			if(b == 0)
			{
				painter.setBrush(QBrush(QColor(0xE0, 0xE0, 0xE0), Qt::SolidPattern));
				painter.drawRect(x + move_x, y1 + y2 + move_y, GraphicCore::get_instance()->get_config()->get_cell_size(), GraphicCore::get_instance()->get_config()->get_cell_size());
				painter.setBrush(QBrush(Qt::white, Qt::SolidPattern));
			}
			else
			{
				painter.setBrush(QBrush(Qt::white, Qt::SolidPattern));
				painter.drawRect(x + move_x, y1 + y2 + move_y, GraphicCore::get_instance()->get_config()->get_cell_size(), GraphicCore::get_instance()->get_config()->get_cell_size());
			}

			if(Core::get_instance()->is_slow_down_at(b, a))
			{
				painter.setBrush(QBrush(Qt::gray, Qt::SolidPattern));
				painter.drawRect(x + move_x, y1 + y2 + move_y, GraphicCore::get_instance()->get_config()->get_cell_size(), GraphicCore::get_instance()->get_config()->get_cell_size());
				painter.setBrush(QBrush(Qt::white, Qt::SolidPattern));
			}

			if(Core::get_instance()->is_car_at(b, a))
			{
				if(GraphicCore::get_instance()->get_config()->get_show_cars())
				{
					QPixmap temp(car_image.size());
					if(GraphicCore::get_instance()->get_config()->get_show_speed_color())
						temp.fill(QColor(0xFF * (static_cast<double>(Core::get_instance()->get_speed(b, a)) / Core::get_instance()->get_max_speed()),
										 (Core::get_instance()->get_speed(b, a) / Core::get_instance()->get_max_speed()) ? 0x8C : 0x00, 0x00));
					else
						temp.fill(car_colors[Core::get_instance()->get_id(b, a) % 11]);

					temp.setMask(car_image.createMaskFromColor(Qt::transparent));
					car_image = temp;
					painter.drawPixmap(x + move_x, y1 + y2 + move_y, car_image);
					painter.drawText(QRectF(x + GraphicCore::get_instance()->get_config()->get_cell_size() * 0.7  + move_x,
											y1 + y2 + GraphicCore::get_instance()->get_config()->get_cell_size() * 0.1 + move_y,
											GraphicCore::get_instance()->get_config()->get_cell_size() * 0.2, GraphicCore::get_instance()->get_config()->get_cell_size() * 0.3),
									QString::number(Core::get_instance()->get_speed(b, a)));
				}
				else
				{
					if(GraphicCore::get_instance()->get_config()->get_show_speed_color())
						painter.setBrush(QBrush(QColor(0xFF * (static_cast<double>(Core::get_instance()->get_speed(b, a)) / Core::get_instance()->get_max_speed()),
														(Core::get_instance()->get_speed(b, a) / Core::get_instance()->get_max_speed()) ? 0x8C : 0x00, 0x00), Qt::SolidPattern));
					else
						painter.setBrush(QBrush(car_colors[Core::get_instance()->get_id(b, a) % 11], Qt::SolidPattern));

					painter.drawRect(x + move_x, y1 + y2 + move_y, GraphicCore::get_instance()->get_config()->get_cell_size(), GraphicCore::get_instance()->get_config()->get_cell_size());
				}
			}
			else if(Core::get_instance()->is_barrier_at(b, a))
			{
				painter.setBrush(QBrush(Qt::black, Qt::SolidPattern));
				painter.drawRect(x + move_x, y1 + y2 + move_y, GraphicCore::get_instance()->get_config()->get_cell_size(), GraphicCore::get_instance()->get_config()->get_cell_size());

				painter.setBrush(QBrush(Qt::white, Qt::SolidPattern));
			}
		}
	}

#ifdef ENABLE_DRAW_TIME_MEASUREMENT
	auto end = std::chrono::high_resolution_clock::now();
	qInfo() << "draw: " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
#endif
}

void StreetWidget::keyPressEvent(QKeyEvent* event)
{
	if(event->key() == Qt::Key_Space)
		GraphicCore::get_instance()->step();
	else if(event->key() == Qt::Key_N)
		GraphicCore::get_instance()->new_game();
	else if(event->key() == Qt::Key_R)
	{
		if(GraphicCore::get_instance()->is_running())
			GraphicCore::get_instance()->stop();
		else
			GraphicCore::get_instance()->start();
	}
	else if(event->key() == Qt::Key_Up)
	{
		if(event->modifiers() & Qt::ControlModifier)
			move_y += 100;
		else
			move_y += 10;
	}
	else if(event->key() == Qt::Key_Down)
	{
		if(event->modifiers() & Qt::ControlModifier)
			move_y -= 100;
		else
			move_y -= 10;
	}
	else if(event->key() == Qt::Key_Left)
	{
		if(event->modifiers() & Qt::ControlModifier)
			move_x += 100;
		else
			move_x += 10;
	}
	else if(event->key() == Qt::Key_Right)
	{
		if(event->modifiers() & Qt::ControlModifier)
			move_x -= 100;
		else
			move_x -= 10;
	}
	else if(event->modifiers() & Qt::CTRL && event->key() == Qt::Key_Plus)
		scale *= 1.1;
	else if(event->modifiers() & Qt::CTRL && event->key() == Qt::Key_Minus)
		scale *= 0.9;


	update();

	event->ignore();
}

void StreetWidget::wheelEvent(QWheelEvent* event)
{
	scale *= 1 + (event->angleDelta().y() / 360. * 0.3);

	update();
}

void StreetWidget::mousePressEvent(QMouseEvent* event)
{
	double cell_size = GraphicCore::get_instance()->get_config()->get_cell_size() * scale;

	// avoid rounding errors
	if((static_cast<double>((event->x() - (move_x * scale))) / cell_size < 0) || (static_cast<double>((event->y() - (move_y * scale))) / cell_size < 0))
		return;

	std::size_t pos = (event->x() - (move_x * scale)) / cell_size;
	std::size_t lane = (event->y() - (move_y * scale)) / cell_size;
	if(GraphicCore::get_instance()->get_config()->get_long_street_break())
	{
		std::size_t breaks = Core::get_instance()->get_length() + 1;
		while(breaks * cell_size > width())
			--breaks;

		pos += lane * breaks;
		lane /= std::ceil(static_cast<double>(Core::get_instance()->get_length()) / breaks);
	}

	if(pos > Core::get_instance()->get_length() || lane > Core::get_instance()->get_lanes())
		return;

	if(GraphicCore::get_instance()->get_run_config()->get_tool() == Tool::Car_Tool)
	{
		if(Core::get_instance()->is_car_at(pos, lane))
			Core::get_instance()->remove_car(pos, lane);
		else
		//	Core::get_instance()->add_car(Core::get_instance()->get_max_speed(), pos, lane);
			Core::get_instance()->add_car(0, pos, lane);
	}
	else if(GraphicCore::get_instance()->get_run_config()->get_tool() == Tool::Slow_Down_Tool)
	{
		if(Core::get_instance()->is_slow_down_at(pos, lane))
			Core::get_instance()->remove_slow_down(pos, lane);
		else
			Core::get_instance()->add_slow_down(pos, lane);
	}
	else if(GraphicCore::get_instance()->get_run_config()->get_tool() == Tool::Barrier_Tool)
	{
		if(Core::get_instance()->is_barrier_at(pos, lane))
			Core::get_instance()->remove_barrier(pos, lane);
		else
			Core::get_instance()->add_barrier(pos, lane);
	}
	else if(GraphicCore::get_instance()->get_run_config()->get_tool() == Tool::Speed_Tool)
	{
		Core::get_instance()->set_car_speed((Core::get_instance()->get_speed(pos, lane) + 1) % (Core::get_instance()->get_max_speed() + 1), pos, lane);
	}

	update();
	update_data();
}

void StreetWidget::reset_position()
{
	move_x = 0;
	move_y = 0;
	scale = 1;
}

void StreetWidget::enable_focus()
{
	// enable own focus
	this->setFocusPolicy(Qt::StrongFocus);
}

void StreetWidget::disable_focus()
{
	// disable own focus
	this->setFocusPolicy(Qt::NoFocus);
}
