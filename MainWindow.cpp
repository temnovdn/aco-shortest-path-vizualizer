#include"MainWindow.h"
#include<QRandomGenerator>
#include<QLabel>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
	scene = new GraphScene(this);
	view = new QGraphicsView(scene);
	view->setRenderHint(QPainter::Antialiasing);

	QPushButton* btn8 = new QPushButton("8 Nodes");
	QPushButton* btn16 = new QPushButton("16 Nodes");
	QPushButton* regenBtn = new QPushButton("Regenerate");

	minWeightBox = new QSpinBox;
	maxWeightBox = new QSpinBox;

	minWeightBox->setRange(1, 100);
	maxWeightBox->setRange(1, 100);
	minWeightBox->setValue(1);
	maxWeightBox->setValue(10);

	QWidget* panel = new QWidget;
	QHBoxLayout* top = new QHBoxLayout;

	top->addWidget(btn8);
	top->addWidget(btn16);
	top->addWidget(new QLabel("Weight range: "));
	top->addWidget(minWeightBox);
	top->addWidget(new QLabel(" - " ));
	top->addWidget(maxWeightBox);
	top->addWidget(regenBtn);

	QVBoxLayout* main = new QVBoxLayout;
	main->addLayout(top);
	main->addWidget(view);

	panel->setLayout(main);
	setCentralWidget(panel);

	connect(btn8, &QPushButton::clicked, this, &MainWindow::choose8);
	connect(btn16, &QPushButton::clicked, this, &MainWindow::choose16);
	connect(regenBtn, &QPushButton::clicked, this, &MainWindow::regenerate);

	regenerate();
}

void MainWindow::choose8()
{
	nodeCount = 8;
	regenerate();
}

void MainWindow::choose16()
{
	nodeCount = 16;
	regenerate();
}

void MainWindow::regenerate()
{
	Graph g = generateRandomGraph();
	scene->drawGraph(g);
}

Graph MainWindow::generateRandomGraph()
{
	Graph g;

	for(int i = 0; i < nodeCount; i++) g.addVertex(QString::number(i).toStdString());

	int minW = minWeightBox->value();
	int maxW = maxWeightBox->value();

	int maxEdges = nodeCount * (nodeCount - 1)/2;
	int edgeCount = QRandomGenerator::global()->bounded(maxEdges/3, maxEdges*2/3);

	for (int i = 0; i < edgeCount; i++)
	{
		int u = QRandomGenerator::global()->bounded(nodeCount);
		int v = QRandomGenerator::global()->bounded(nodeCount);

		if(u == v) continue;
		
		if(g.edgeExist(QString::number(u).toStdString(), QString::number(v).toStdString()) || (g.edgeExist(QString::number(v).toStdString(), QString::number(u).toStdString()))) 
		{
			continue;
		}

		int weight = QRandomGenerator::global()->bounded(minW, maxW + 1); //bounded is [a, b) , so we need maxW+1

		g.addEdge(QString::number(u).toStdString(), QString::number(v).toStdString(), weight);

	}
	
	return g;
}
