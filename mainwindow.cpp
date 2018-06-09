#include "mainwindow.h"
#include <QKeyEvent>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), preferences(this), preferences_animation(&preferences, "pos"), current_view(Street_View)
{
	initGUI();

	this->setFocusProxy(&street);
	this->setMinimumSize(MIN_WIDTH, MIN_HEIGHT);
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
	if(event->key() == Qt::Key_Escape)
	{
		if(current_view & Preferences_View)
			hide_preferences();
	}
	else if(event->key() == Qt::Key_P)
		show_preferences();
}

void MainWindow::resizeEvent(QResizeEvent*)
{
	preferences.resize(width() / 5, height() / 4);
	if(current_view & Preferences_View)
		show_preferences();
	else
		preferences.move(width() - preferences.width(), height());
}

void MainWindow::show()
{
	this->showMaximized();

	preferences.move(width() - preferences.width(), height());
}

void MainWindow::initGUI()
{
	preferences.setVisible(false);
	hide_preferences();

//	main_layout.addWidget(&street, 0, 0, 4, 4);
//	main_layout.addWidget(&preferences, 3, 3);

	main_widget.setLayout(&main_layout);
	this->setCentralWidget(&street);

	preferences.raise();
}

void MainWindow::show_preferences()
{
	preferences.show();

	preferences_animation.setStartValue(preferences.pos());
	preferences_animation.setEndValue(QPoint(width() - preferences.width(), height() - preferences.height()));
	preferences_animation.setDuration(ANIMATION_TIME);
	preferences_animation.stop();
	preferences_animation.start();

	// disable focus in preferences view
	preferences.enable_focus();
	// clear preferences view bit
	current_view |= Preferences_View;
}

void MainWindow::hide_preferences()
{
	preferences_animation.setStartValue(preferences.pos());
	preferences_animation.setEndValue(QPoint(width() - preferences.width(), height()));
	preferences_animation.setDuration(ANIMATION_TIME);
	preferences_animation.stop();
	preferences_animation.start();

	// disable focus in preferences view
	preferences.disable_focus();
	street.setFocus();
	// clear preferences view bit
	current_view &= ~Preferences_View;
}
