#include "streetwidget.h"
#include <QPaintEvent>
#include <QBitmap>

StreetWidget::StreetWidget(QWidget* parent) : QWidget(parent), car_image(":/images/car-200.png"),
												car_colors({ Qt::blue, Qt::green, Qt::gray, Qt::yellow, Qt::darkMagenta, Qt::darkGreen, Qt::cyan, Qt::darkCyan, Qt::darkYellow,
														   Qt::darkGray, Qt::black }),
												scale(1), move_x(0), move_y(0)
{
	this->setAutoFillBackground(true);
	this->setPalette(QPalette(Qt::black));

	car_image = car_image.scaled(config.get_cell_size(), config.get_cell_size());
}

void StreetWidget::paintEvent(QPaintEvent* event)
{
	Q_UNUSED(event)

	QPainter painter(this);
	painter.setPen(Qt::black);

	painter.scale(scale, scale);

	for(std::size_t a = 0, y1 = move_y; a < config.get_street_lanes(); ++a, y1 += config.get_cell_size())
	{
		for(std::size_t b = 0, x = move_x, y2 = 0; b < config.get_street_length(); ++b, x += config.get_cell_size())
		{
			if((x + config.get_cell_size()) * scale > static_cast<std::size_t>(width()))
			{
				y1 += config.get_cell_size();
				x = 0;
			}

			if(b == 0)
			{
				painter.setBrush(QBrush(Qt::lightGray, Qt::SolidPattern));
				painter.drawRect(x, y1 + y2, config.get_cell_size(), config.get_cell_size());

				painter.setBrush(QBrush(Qt::white, Qt::SolidPattern));
			}
			else
				painter.drawRect(x, y1 + y2, config.get_cell_size(), config.get_cell_size());

			if(cars.is_car_at(b, a))
			{
				QPixmap temp(car_image.size());
				temp.fill(car_colors[cars.get_id(b, a) % 11]);
				temp.setMask(car_image.createMaskFromColor(Qt::transparent));
				car_image = temp;
				painter.drawPixmap(x, y1 + y2, car_image);
				painter.drawText(QRectF(x + config.get_cell_size() * 0.7, y1 + y2 + config.get_cell_size() * 0.1, config.get_cell_size() * 0.2, config.get_cell_size() * 0.3), QString::number(cars.get_speed(b, a)));

			}
		}
	}
}

void StreetWidget::keyPressEvent(QKeyEvent* event)
{
	if(event->key() == Qt::Key_Space)
		cars.generate();
	else if(event->key() == Qt::Key_Delete)
		cars.new_random();

	update();
}

void StreetWidget::wheelEvent(QWheelEvent* event)
{
	scale *= 1 + (event->angleDelta().y() / 360. * 0.3);

	update();
}

void StreetWidget::mousePressEvent(QMouseEvent* event)
{

}
