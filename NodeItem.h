#ifndef NODEITEM_H
#define NODEITEM_H

#include<QGraphicsEllipseItem>
#include<QGraphicsSceneMouseEvent>
#include<QBrush>
#include<QString>
#include <QColor>   //zhao
#include <functional> //zhao


class NodeItem : public QGraphicsEllipseItem
{
	public:
		NodeItem(const QString& name, const QPointF& pos, qreal radius = 20);
		
		QString getName() const {return m_name;}
		void setColor(const QColor& c);//controll color
		void setClickHandler(const std::function<void(const QString&)>& handler);//zhao
	protected:
		void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
	private:
		QString m_name;
		std::function<void(const QString&)> m_clickHandler; //zhao
};

#endif
