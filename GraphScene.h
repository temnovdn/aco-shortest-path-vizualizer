#ifndef GRAPHSCENE_H
#define GRAPHSCENE_H

#include<QGraphicsScene>
#include"graph.h"
#include"NodeItem.h"
#include"EdgeItem.h"

class GraphScene : public QGraphicsScene
{
	public:
		GraphScene(QObject* parent = nullptr);

		void drawGraph(Graph& graph);

	private:
		QMap<QString, NodeItem*> nodeItems;
};

#endif
