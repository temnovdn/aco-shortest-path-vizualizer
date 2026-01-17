#ifndef EDGEITEM_H
#define EDGEITEM_H

#include<QGraphicsLineItem>
#include<QGraphicsTextItem>

class EdgeItem : public QGraphicsLineItem
{
	public: 
		EdgeItem(const QPointF& p1, const QPointF& p2, int weight);
	private:
		QGraphicsTextItem* weightText;
};

#endif
