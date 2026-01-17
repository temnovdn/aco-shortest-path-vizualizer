#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include<QMainWindow>
#include<QGraphicsView>
#include<QSpinBox>
#include<QPushButton>
#include<QHBoxLayout>
#include<QVBoxLayout>
#include"GraphScene.h"
#include"graph.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

	public:
		MainWindow(QWidget *parent = nullptr);
	private slots:
		void choose8();
		void choose16();
		void regenerate();
	private:
		GraphScene* scene;
		QGraphicsView* view;

		int nodeCount = 8;
		QSpinBox* minWeightBox;
		QSpinBox* maxWeightBox;

		Graph generateRandomGraph();
};

#endif
