#include"GraphScene.h"
#include<QRandomGenerator>

GraphScene::GraphScene(QObject* parent) : QGraphicsScene(parent)
{
}
void GraphScene::setNodeClickHandler(const std::function<void(const QString&)>& handler)
{
    m_nodeClickHandler = handler;//The click logic is saved to the GraphScene member variable.
}//zhao

void GraphScene::resetStyles()
{
    for (auto it = nodeItems.begin(); it != nodeItems.end(); ++it)
    {
        if (it.value()) it.value()->setColor(Qt::yellow);// if empty pass or change to yellow
    }
}//zhao

void GraphScene::setNodeColor(const QString& nodeId, const QColor& color)
{
    if (!nodeItems.contains(nodeId)) 
	return;                              //check if ondeitems have node id,OR
    nodeItems[nodeId]->setColor(color);
}//zhao

void GraphScene::highlightPath(const std::vector<std::string>& path)
{
    resetStyles(); 
      for (const auto& id : path)
    {
        QString qid = QString::fromStdString(id);
        setNodeColor(qid, Qt::green); 
    }
}//zhao

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
	node->setClickHandler(m_nodeClickHandler);//return bcak zhao
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
