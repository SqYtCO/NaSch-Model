#ifndef STREETWIDGET_H
#define STREETWIDGET_H

#include <QGraphicsView>
#include <QImage>
#include "car_system.h"
#include "configuration.h"

class StreetWidget : public QWidget
{
	Car_System cars;
	QPixmap car_image;
	QColor car_colors[11];
	Configuration config;

	// scale factor
	double scale;

	// movement
	int move_x;
	int move_y;

public:
	StreetWidget(QWidget* parent = nullptr);

protected:
	void paintEvent(QPaintEvent* event);
	void keyPressEvent(QKeyEvent* event);
	void wheelEvent(QWheelEvent* event);
	void mousePressEvent(QMouseEvent* event);
};

#endif // STREETWIDGET_H
