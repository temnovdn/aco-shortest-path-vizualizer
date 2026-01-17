#include"GraphScene.h"
#include<QRandomGenerator>

GraphScene::GraphScene(QObject* parent) : QGraphicsScene(parent)
{
}

void GraphScene::drawGraph(Graph& graph)
{
	clear();
	nodeItems.clear();

	auto vertices = graph.getVertices();

	for (const auto& v: vertices)
	{
		QPoint pos(
				QRandomGenerator::global()->bounded(100,600),
				QRandomGenerator::global()->bounded(100,400)
		);

	NodeItem* node = new NodeItem(QString::fromStdString(v), pos);
	addItem(node);

	nodeItems[v.c_str()] = node;
	}

	for (const auto& v1 : vertices)
	{
		for (const auto& v2 : vertices)
		{
			if(graph.edgeExist(v1,v2))
			{
				QPointF p1 = nodeItems[v1.c_str()]->rect().center();
				QPointF p2 = nodeItems[v2.c_str()]->rect().center();

				int w = graph.getWeight(v1, v2);
				addItem(new EdgeItem(p1, p2, w));
			}
		}
	}
}
