#ifndef CARITEM_H
#define CARITEM_H

#include <QGraphicsItem>
#include <QPixmap>
#include <memory>

class CarItem : public QGraphicsItem
{
	static std::unique_ptr<QPixmap> car_image;
	static QColor car_colors[11];
	QRectF rect;
	std::size_t pos;
	std::size_t lane;

public:
	CarItem(std::size_t pos, std::size_t lane, QGraphicsItem* parent = nullptr);

	virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) override;
	virtual QRectF boundingRect() const override { return rect; }

protected:
	virtual void mousePressEvent(QGraphicsSceneMouseEvent*) override;
};

#endif // CARITEM_H
