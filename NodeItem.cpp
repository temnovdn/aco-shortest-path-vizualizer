#include"NodeItem.h"
#include<QPen>
#include<QGraphicsTextItem>

NodeItem::NodeItem(const QString& name, const QPointF& pos, qreal radius) : m_name(name)
{
	setRect(pos.x() -radius, pos.y() -radius, radius *2, radius *2); //QGraphicsEllipseItem draw circle as a circle inside of a rectangle, need left upper corner point,height,width
	setBrush(QBrush(Qt::yellow));
	setPen(QPen(Qt::black, 2));

	//show Node name
	auto* text = new QGraphicsTextItem(name, this);
	text->setPos(pos.x()-6, pos.y()-10);
}

void NodeItem::setColor(const QColor& c)
{
    setBrush(QBrush(c));
}//zhao

void NodeItem::setClickHandler(const std::function<void(const QString&)>& handler)
{
    m_clickHandler = handler;
}//zhao

void NodeItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	if (m_clickHandler)
    m_clickHandler(m_name);//selectcontroller controller color
	
	QGraphicsEllipseItem::mousePressEvent(event);
}
