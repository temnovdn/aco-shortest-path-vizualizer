#include"EdgeItem.h"
#include<QPen>

EdgeItem::EdgeItem(const QPointF& p1, const QPointF& p2, int weight)
{
	setLine(QLineF(p1,p2));
	setPen(QPen(Qt::black, 2));

	QPointF mid = (p1 + p2) / 2;
	weightText = new QGraphicsTextItem(QString::number(weight), this);
	weightText->setPos(mid.x(), mid.y());
}
