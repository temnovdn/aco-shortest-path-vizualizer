#ifndef GRAPHSCENE_H
#define GRAPHSCENE_H

#include<QGraphicsScene>
#include <QGraphicsScene>
#include <QMap>
#include <QString>
#include <QColor>
#include <functional>
#include <vector>//zhao
#include <string>//zhao

#include"graph.h"
#include"NodeItem.h"
#include"EdgeItem.h"

class GraphScene : public QGraphicsScene
{
	public:
		GraphScene(QObject* parent = nullptr);

		void drawGraph(Graph& graph);

	void setNodeClickHandler(const std::function<void(const QString&)>& handler);
    void resetStyles();
    void setNodeColor(const QString& nodeId, const QColor& color);//zhao

    void highlightPath(const std::vector<std::string>& path);//zhao

	private:
		QMap<QString, NodeItem*> nodeItems;
		std::function<void(const QString&)> m_nodeClickHandler;//zhao
};

#endif
