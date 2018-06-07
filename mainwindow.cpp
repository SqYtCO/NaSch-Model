#include "mainwindow.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent)
{
	this->setMinimumSize(MIN_WIDTH, MIN_HEIGHT);
	initGUI();

	street.setFocus();
}

void MainWindow::initGUI()
{
	main_layout.addWidget(&street, 0, 0, 4, 4);
	main_layout.addWidget(&preferences, 3, 3);

	main_widget.setLayout(&main_layout);
	this->setCentralWidget(&main_widget);

	preferences.raise();
}
