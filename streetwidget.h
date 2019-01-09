#ifndef STREETWIDGET_H
#define STREETWIDGET_H

#include "caritem.h"
#include <QGraphicsView>
#include <QImage>

class StreetWidget : public QGraphicsView
{
	Q_OBJECT

	QGraphicsScene scene;

	std::size_t current_length;
	std::size_t current_lanes;

public:
	StreetWidget(QWidget* parent = nullptr);
	virtual ~StreetWidget() override;

	void reset_position();

	// enable the focus of all children and the widget
	void enable_focus();
	// disable the focus of all children and the widget
	void disable_focus();

	void update();
	void update_data() { emit data_changed(); }

signals:
	void data_changed();
	void start_update();

protected:
	virtual void keyPressEvent(QKeyEvent* event) override;
	virtual void wheelEvent(QWheelEvent* event) override;
};

#endif // STREETWIDGET_H
