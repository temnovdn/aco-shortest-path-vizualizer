#include"MainWindow.h"
#include<QRandomGenerator>
#include<QLabel>
#include "AcoController.h" //zhao
#include "AcoRunner.h" //zhao
#include <vector>   


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
	
	aco = new AcoController(&selector, this);//zhao run reset buttons
    aco->addToLayout(top);//zhao
	resultLabel = new QLabel("Total path weight: -", this);//zhao
    top->addWidget(resultLabel);//zhao

	QVBoxLayout* main = new QVBoxLayout;
	main->addLayout(top);
	main->addWidget(view);

	panel->setLayout(main);
	setCentralWidget(panel);

	connect(btn8, &QPushButton::clicked, this, &MainWindow::choose8);
	connect(btn16, &QPushButton::clicked, this, &MainWindow::choose16);
	connect(regenBtn, &QPushButton::clicked, this, &MainWindow::regenerate);  
   
   selector.setScene(scene);
   scene->setNodeClickHandler([this](const QString& nodeId) 
   {
    this->onNodeClicked(nodeId);
   });//zhao set a return when click node weill return back mw
   
   connect(aco, &AcoController::runRequested,this, [this](const QString& src, const QString& dst)
   {
    AcoRunner runner;
    AcoResult r = runner.run(
    currentGraph,
    src.toStdString(),
    dst.toStdString()
    );
	if (r.path.empty())
	{  
        scene->setNodeColor(src, Qt::blue);//even no pat color it
        scene->setNodeColor(dst, Qt::red);
        resultLabel->setText("Total path weight: No path found (graph direction/connectivity issue).");
        return;
    }
    scene->highlightPath(r.path);
	scene->setNodeColor(src, Qt::blue);
    scene->setNodeColor(dst, Qt::red);//make sure no be cover
	resultLabel->setText(QString("Total path weight: %1").arg(r.totalWeight));
 });//zhao

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
	currentGraph = generateRandomGraph();   // save 
    scene->drawGraph(currentGraph); 
	selector.clear();
	if (aco) aco->sync();
	if (resultLabel) resultLabel->setText("Total path weight: -");
    //clean the show
}

void MainWindow::onNodeClicked(const QString& nodeId)
{
    selector.handleNodeClicked(nodeId);
	if (aco) aco->sync();
}//zhao

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
    


