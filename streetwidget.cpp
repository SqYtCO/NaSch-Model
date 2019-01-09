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

StreetWidget::StreetWidget(QWidget* parent) : QGraphicsView(parent)
{
	QObject::connect(this, &StreetWidget::start_update, this, &StreetWidget::update);

	this->setAutoFillBackground(true);
	this->setPalette(QPalette(Qt::black));

	this->setScene(&scene);
	//setDragMode(QGraphicsView::ScrollHandDrag);
	//setDragMode(QGraphicsView::RubberBandDrag);
	this->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	this->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

	update();
}

StreetWidget::~StreetWidget()
{
	GraphicCore::stop();
}

void StreetWidget::keyPressEvent(QKeyEvent* event)
{
	if(event->key() == Qt::Key_Space)
		GraphicCore::step();
	else if(event->key() == Qt::Key_N)
		GraphicCore::new_game();
	else if(event->key() == Qt::Key_R)
	{
		if(GraphicCore::is_running())
			GraphicCore::stop();
		else
			GraphicCore::start();
	}
	else if(event->key() == Qt::Key_Up)
	{
		if(event->modifiers() & Qt::ControlModifier)
			this->verticalScrollBar()->setValue(this->verticalScrollBar()->value() - 100);
		else
			this->verticalScrollBar()->setValue(this->verticalScrollBar()->value() - 10);
	}
	else if(event->key() == Qt::Key_Down)
	{
		if(event->modifiers() & Qt::ControlModifier)
			this->verticalScrollBar()->setValue(this->verticalScrollBar()->value() + 100);
		else
			this->verticalScrollBar()->setValue(this->verticalScrollBar()->value() + 10);
	}
	else if(event->key() == Qt::Key_Left)
	{
		if(event->modifiers() & Qt::ControlModifier)
			this->horizontalScrollBar()->setValue(this->horizontalScrollBar()->value() - 100);
		else
			this->horizontalScrollBar()->setValue(this->horizontalScrollBar()->value() - 10);
	}
	else if(event->key() == Qt::Key_Right)
	{
		if(event->modifiers() & Qt::ControlModifier)
			this->horizontalScrollBar()->setValue(this->horizontalScrollBar()->value() + 100);
		else
			this->horizontalScrollBar()->setValue(this->horizontalScrollBar()->value() + 10);
	}
	else if(event->key() == Qt::Key_Plus)
		this->scale(1.1, 1.1);
	else if(event->key() == Qt::Key_Minus)
		this->scale(0.9, 0.9);


	update();

	event->ignore();
}

void StreetWidget::wheelEvent(QWheelEvent* event)
{
	double scale_factor = 1 + (event->angleDelta().y() / 360. * 0.3);
	this->scale(scale_factor, scale_factor);

	update();
}

void StreetWidget::reset_position()
{
	this->resetTransform();
	this->resetMatrix();
	this->horizontalScrollBar()->setValue(0);
	this->verticalScrollBar()->setValue(0);
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

void StreetWidget::update()
{
	std::lock_guard<typename std::remove_reference<decltype(GraphicCore::get_mutex())>::type> lock(GraphicCore::get_mutex());

	if(current_lanes != Core::get_lanes() || current_length != Core::get_length())
	{
		scene.clear();
		for(std::size_t lane = 0; lane < Core::get_lanes(); ++lane)
			for(std::size_t pos = 0; pos < Core::get_length(); ++pos)
				scene.addItem(new CarItem(pos, lane));
		current_lanes = Core::get_lanes();
		current_length = Core::get_length();
	}

	scene.update(sceneRect());
}
