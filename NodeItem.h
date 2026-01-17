#ifndef NODEITEM_H
#define NODEITEM_H

#include<QGraphicsEllipseItem>
#include<QGraphicsSceneMouseEvent>
#include<QBrush>
#include<QString>

class NodeItem : public QGraphicsEllipseItem
{
	public:
		NodeItem(const QString& name, const QPointF& pos, qreal radius = 20);
		
		QString getName() const {return m_name;}
	protected:
		void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
	private:
		QString m_name;
};

#endif
