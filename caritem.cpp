#include "caritem.h"
#include "core.h"
#include "graphiccore.h"

std::unique_ptr<QPixmap> CarItem::car_image;
QColor CarItem::car_colors[] = { Qt::blue, Qt::green, Qt::gray, Qt::yellow, Qt::darkMagenta, Qt::darkGreen, Qt::cyan, Qt::darkCyan, Qt::darkYellow,
								 Qt::darkGray, Qt::black };

CarItem::CarItem(std::size_t pos, std::size_t lane, QGraphicsItem* parent) : QGraphicsItem(parent), pos(pos), lane(lane)
{
	if(!car_image)
		car_image.reset(new QPixmap(":/images/car-200.png"));

	rect.setX(pos * GraphicCore::get_config()->get_cell_size());
	rect.setY(lane * GraphicCore::get_config()->get_cell_size());
	rect.setHeight(GraphicCore::get_config()->get_cell_size());
	rect.setWidth(GraphicCore::get_config()->get_cell_size());
}

void CarItem::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
	*car_image = car_image->scaled(GraphicCore::get_config()->get_cell_size(), GraphicCore::get_config()->get_cell_size());

	painter->setPen(Qt::black);

	if(pos == 0)
	{
		painter->setBrush(QBrush(QColor(0xE0, 0xE0, 0xE0), Qt::SolidPattern));
		painter->drawRect(rect);
		painter->setBrush(QBrush(Qt::white, Qt::SolidPattern));
	}
	else
	{
		painter->setBrush(QBrush(Qt::white, Qt::SolidPattern));
		painter->drawRect(rect);
	}

	if(Core::is_slow_down_at(pos, lane))
	{
		painter->setBrush(QBrush(Qt::gray, Qt::SolidPattern));
		painter->drawRect(rect);
		painter->setBrush(QBrush(Qt::white, Qt::SolidPattern));
	}

	if(Core::is_car_at(pos, lane))
	{
		if(GraphicCore::get_config()->get_show_cars())
		{
			QPixmap temp(car_image->size());
			if(GraphicCore::get_config()->get_show_speed_color())
				temp.fill(QColor(0xFF * (static_cast<double>(Core::get_speed(pos, lane)) / Core::get_max_speed()),
								 (Core::get_speed(pos, lane) / Core::get_max_speed()) ? 0x8C : 0x00, 0x00));
			else
				temp.fill(Qt::black);

			temp.setMask(car_image->createMaskFromColor(Qt::transparent));
//			car_image = temp;
			painter->drawPixmap(rect.x(), rect.y(), temp);
			painter->drawText(QRectF(rect.x() + GraphicCore::get_config()->get_cell_size() * 0.7,
									rect.y() + GraphicCore::get_config()->get_cell_size() * 0.1,
									GraphicCore::get_config()->get_cell_size() * 0.2, GraphicCore::get_config()->get_cell_size() * 0.3),
							QString::number(Core::get_speed(pos, lane)));
		}
		else
		{
			if(GraphicCore::get_config()->get_show_speed_color())
				painter->setBrush(QBrush(QColor(0xFF * (static_cast<double>(Core::get_speed(pos, lane)) / Core::get_max_speed()),
												(Core::get_speed(pos, lane) / Core::get_max_speed()) ? 0x8C : 0x00, 0x00), Qt::SolidPattern));
			else
				painter->setBrush(QBrush(Qt::black, Qt::SolidPattern));

			painter->drawRect(rect);
		}
	}
	else if(Core::is_barrier_at(pos, lane))
	{
		painter->setBrush(QBrush(Qt::black, Qt::SolidPattern));
		painter->drawRect(rect);

		painter->setBrush(QBrush(Qt::white, Qt::SolidPattern));
	}
}

void CarItem::mousePressEvent(QGraphicsSceneMouseEvent*)
{
	if(GraphicCore::get_run_config()->get_tool() == Tool::Car_Tool)
	{
		if(Core::is_car_at(pos, lane))
			Core::remove_car(pos, lane);
		else
			Core::add_car(Core::get_max_speed(), pos, lane);
	}
	else if(GraphicCore::get_run_config()->get_tool() == Tool::Slow_Down_Tool)
	{
		if(Core::is_slow_down_at(pos, lane))
			Core::remove_slow_down(pos, lane);
		else
			Core::add_slow_down(pos, lane);
	}
	else if(GraphicCore::get_run_config()->get_tool() == Tool::Barrier_Tool)
	{
		if(Core::is_barrier_at(pos, lane))
			Core::remove_barrier(pos, lane);
		else
			Core::add_barrier(pos, lane);
	}
	else if(GraphicCore::get_run_config()->get_tool() == Tool::Speed_Tool)
	{
		Core::set_car_speed((Core::get_speed(pos, lane) + 1) % (Core::get_max_speed() + 1), pos, lane);
	}

	scene()->update(rect);
}
