// © Copyright (c) 2018 SqYtCO

#include "mainwindow.h"
#include "graphiccore.h"
#include "core.h"
#include "startupdialog.h"
#include <QApplication>
#include <QMessageBox>
#include <QFileDialog>
#include <QKeyEvent>

#define TOOL_VIEW_TOP_HIDE_POINT QPoint(street_view.x(), street_view.y() - tool_view.height())
#define TOOL_VIEW_TOP_SHOW_POINT QPoint(street_view.x(), street_view.y())
#define TOOL_VIEW_BOTTOM_HIDE_POINT QPoint(street_view.x(), street_view.y() + street_view.height())
#define TOOL_VIEW_BOTTOM_SHOW_POINT QPoint(street_view.x(), street_view.y() + street_view.height() - tool_view.height())
#define TOOL_VIEW_HIDE_POINT TOOL_VIEW_TOP_HIDE_POINT
#define TOOL_VIEW_SHOW_POINT TOOL_VIEW_TOP_SHOW_POINT

MainWindow::MainWindow(const char* start_file, QWidget* parent) : QMainWindow(parent),
											preferences_view((GraphicCore::init_gui(&street_view, &chart_view), this)), preferences_animation(&preferences_view, "pos"),
											tool_view(this), tool_animation(&tool_view, "pos"),
											help_view(this), help_animation(&help_view, "pos"),
											info_view(this), info_animation(&info_view, "pos")
{
	// setup GUI and set current active view
	init_gui();
	current_view = Street_View;

	// set minimum size
	this->setMinimumSize(MIN_WIDTH, MIN_HEIGHT);

	this->setFocusProxy(&street_view);
	this->setFocus();

	// setup resize_timer; this timer fixes a resize bug when switching to fullscreen mode
	resize_timer.setInterval(10);
	resize_timer.setSingleShot(true);
	QObject::connect(&resize_timer, &QTimer::timeout, this, &MainWindow::update_views_geometry);

	QObject::connect(&preferences_view, &PreferencesWidget::language_changed, this, &MainWindow::translate_application);
	QObject::connect(&preferences_view, &PreferencesWidget::hide_preferences, this, &MainWindow::hide_preference_view);
	QObject::connect(&help_view, &HelpWidget::hide_help, this, &MainWindow::hide_help_view);
	QObject::connect(&tool_view, &ToolWidget::hide_tool, this, &MainWindow::hide_tool_view);
	QObject::connect(&tool_view, &ToolWidget::show_help, this, &MainWindow::show_help_view);
	QObject::connect(&tool_view, &ToolWidget::show_preferences, this, &MainWindow::show_preference_view);
	QObject::connect(&tool_view, &ToolWidget::fullscreen_changed, [this]()
	{
		if(isFullScreen())
			showMaximized();
		else
			showFullScreen();
	});
	QObject::connect(&street_view, &StreetWidget::data_changed, &tool_view, &ToolWidget::update_data);
	QObject::connect(&street_view, &StreetWidget::data_changed, &info_view, &InfoWidget::update_data);

	translate_application();

	if(start_file != nullptr)
		GraphicCore::load(start_file);
	else
		GraphicCore::new_game();
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
	// show/hide preferences view
	if(event->key() == Qt::Key_P)
	{
		if(current_view & Preferences_View)
			hide_preference_view();
		else
		{
			GraphicCore::stop();
			show_preference_view();
		}
	}
	// show/hide help view
	else if(event->key() == Qt::Key_H)
	{
		if(current_view & Help_View)
			hide_help_view();
		else
		{
			GraphicCore::stop();
			show_help_view();
		}
	}
	else if(event->key() == Qt::Key_I && !(current_view & Preferences_View))
	{
		if(current_view & Info_View)
			hide_info_view();
		else
			show_info_view();
	}
	// show tool view only if neither preferences nor help view are active
	else if(event->key() == Qt::Key_T && !(current_view & (Preferences_View | Help_View)))
	{
		if(current_view & Tool_View)
			hide_tool_view();
		else
			show_tool_view();
	}
	else if(event->key() == Qt::Key_Escape)
	{
		if(current_view & Help_View)
			hide_help_view();
		else if(current_view & Preferences_View)
			hide_preference_view();
		else if(current_view & Tool_View)
			hide_tool_view();
	}
	// open only if neither preferences nor help view are active
	else if(event->key() == Qt::Key_O && !(current_view & (Preferences_View | Help_View)))
	{
		QString file = QFileDialog::getOpenFileName();
		if(!file.isEmpty())
			GraphicCore::load(file.toStdString());
	}
	// save only if neither preferences nor help view are active
	else if(event->key() == Qt::Key_S && !(current_view & (Preferences_View | Help_View)))
	{
		if(event->modifiers() & Qt::ControlModifier)
		{
			QString file = QFileDialog::getSaveFileName();
			if(!file.isEmpty())
				GraphicCore::save(file.toStdString());
		}
		else
			GraphicCore::save();
	}
	else if(event->key() == Qt::Key_F)
	{
		if(isFullScreen())
			showMaximized();
		else
			showFullScreen();
	}
	else if(event->key() == Qt::Key_Q)
		close();
#ifdef CREATE_CHARTS
	else if(event->key() == Qt::Key_C)
	{
		if(chart_view.isVisible())
			chart_view.close();
		else
			chart_view.show();
	}
#endif
}

void MainWindow::resizeEvent(QResizeEvent*)
{
	resize_timer.start();
}

void MainWindow::closeEvent(QCloseEvent* event)
{
	GraphicCore::stop();
#ifdef CREATE_CHARTS
	chart_view.close();
#endif

	// if any preference is not saved, show question; this question is necessary if the user is in the preferences view and tries to close the window
	if(!(Core::get_config()->is_saved() && GraphicCore::get_config()->is_saved()))
	{
		QMessageBox::StandardButton preferences_save_answer = QMessageBox::question(this, tr("Save Preferences?"), tr("Save changed preferences?"),
																					QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

		if(preferences_save_answer == QMessageBox::Discard)
			preferences_view.discard();
		else if(preferences_save_answer == QMessageBox::Save)
			preferences_view.apply();
		else
		{
			event->ignore();
			return;
		}
	}

	if(GraphicCore::get_config()->get_exit_warning())
	{
		QMessageBox::StandardButton quit_answer = QMessageBox::question(this, tr("Save Game?"), tr("Save current game?"),
																		QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

		if(quit_answer == QMessageBox::Discard)
		{
			event->accept();
		}
		else if(quit_answer == QMessageBox::Save)
		{
			GraphicCore::save();
			event->accept();
		}
		else
		{
			event->ignore();
		}
	}
}

void MainWindow::show()
{
	// read fullscreen preference and start in set mode
	if(GraphicCore::get_config()->get_fullscreen())
		this->showFullScreen();
	else
		this->showMaximized();

	if(GraphicCore::get_config()->get_show_startup_dialog())
	{
		StartupDialog dialog;
		// connect dialog signal
		QObject::connect(&dialog, &StartupDialog::show_help, this, &MainWindow::show_help_view);
		dialog.exec();
	}

	resize_timer.start();
}

void MainWindow::init_gui()
{
	// set central widget
	setCentralWidget(&street_view);

	// set z axis order
	info_view.raise();
	tool_view.raise();
	help_view.raise();
	preferences_view.raise();
}

void MainWindow::translate_application()
{
	if(GraphicCore::get_config()->get_language() == Language::German ||
			(GraphicCore::get_config()->get_language() == Language::System && QLocale::system().language() == QLocale::German))
	{
		if(!translator.load(":/translations/gol_de"))
			qDebug("No DE translation found!");
		if(!qt_translator.load(":/translations/qtbase_de"))
			qDebug("No DE_Qt translation found!");

		qApp->installTranslator(&translator);
		qApp->installTranslator(&qt_translator);
	}
	else
	{
		// turn off translation; switch to native English
		qApp->removeTranslator(&translator);
		qApp->removeTranslator(&qt_translator);

	}

	preferences_view.translate();
	tool_view.translate();
	help_view.translate();
}

void MainWindow::update_views_geometry()
{
	// update size of views
	tool_view.resize(this->width(), ToolWidget::TOOL_HEIGHT);
	preferences_view.resize(this->size());
	help_view.resize(this->width() / 3, this->height());
	info_view.resize(this->width() / 4, this->height() / 5);

	// update pos of views
	if(current_view & Preferences_View)
	{
		// show
		show_preference_view();
		if(!(current_view & Help_View))
			help_view.move(-help_view.width(), street_view.y());
		tool_view.move(TOOL_VIEW_HIDE_POINT);
		info_view.move(width() - info_view.width(), height());
	}
	else if(current_view & Help_View)
	{
		show_help_view();
		if(!(current_view & Preferences_View))
			preferences_view.move(width(), street_view.y());
		tool_view.move(TOOL_VIEW_HIDE_POINT);
		info_view.move(width() - info_view.width(), height());
	}
	else if(current_view & Tool_View)
	{
		show_tool_view();
		preferences_view.move(width(), street_view.y());
		help_view.move(-help_view.width(), street_view.y());
	}
	else if(current_view & Info_View)
	{
		show_info_view();
		preferences_view.move(width(), street_view.y());
		help_view.move(-help_view.width(), street_view.y());
		tool_view.move(TOOL_VIEW_HIDE_POINT);
	}
	else
	{
		preferences_view.move(width(), street_view.y());
		help_view.move(-help_view.width(), street_view.y());
		tool_view.move(TOOL_VIEW_HIDE_POINT);
		info_view.move(width() - info_view.width(), height());
	}
}

void MainWindow::show_Street_View()
{
	hide_preference_view();
	hide_help_view();
}

void MainWindow::show_preference_view()
{
	// hide tool view but keep bit set
	if(current_view & Tool_View)
	{
		tool_animation.setStartValue(tool_view.pos());
		tool_animation.setEndValue(TOOL_VIEW_HIDE_POINT);
		tool_animation.setDuration(ANIMATION_TIME);
		tool_animation.stop();
		tool_animation.start();

		// disable focus in tool view
		tool_view.disable_focus();
		this->setFocus();
	}
	if(current_view & Info_View)
	{
		info_animation.setStartValue(info_view.pos());
		info_animation.setEndValue(QPoint(width() - info_view.width(), height()));
		info_animation.setDuration(ANIMATION_TIME);
		info_animation.stop();
		info_animation.start();

		info_view.disable_focus();
		this->setFocus();
	}

	if(current_view & Help_View)
	{
		preferences_animation.setStartValue(preferences_view.pos());
		preferences_animation.setEndValue(QPoint(help_view.width(), street_view.y()));
		preferences_animation.setDuration(ANIMATION_TIME);
		preferences_animation.stop();
		preferences_animation.start();
	}
	else
	{
		preferences_animation.setStartValue(preferences_view.pos());
		preferences_animation.setEndValue(QPoint(width() - preferences_view.width(), street_view.y()));
		preferences_animation.setDuration(ANIMATION_TIME);
		preferences_animation.stop();
		preferences_animation.start();
	}

	// enable focus in preferences view
	preferences_view.enable_focus();
	preferences_view.setFocus();
	current_view |= Preferences_View;
}

void MainWindow::hide_preference_view()
{
	// if any preference is not saved, show question
	if(!(Core::get_config()->is_saved() && GraphicCore::get_config()->is_saved()))
	{
		QMessageBox::StandardButton preferences_save_answer = QMessageBox::question(this, tr("Save Preferences?"), tr("Save changed preferences?"),
																					QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

		if(preferences_save_answer == QMessageBox::Discard)
			preferences_view.discard();
		else if(preferences_save_answer == QMessageBox::Save)
			preferences_view.apply();
		else
			return;
	}

	// restore tool view if it is active and help view is not active
	if((current_view & Tool_View) && !(current_view & Help_View))
		show_tool_view();
	if(current_view & Info_View)
		show_info_view();

	preferences_animation.setStartValue(preferences_view.pos());
	preferences_animation.setEndValue(QPoint(width(), street_view.y()));
	preferences_animation.setDuration(ANIMATION_TIME);
	preferences_animation.stop();
	preferences_animation.start();

	// disable focus in preferences view
	preferences_view.disable_focus();
	this->setFocus();
	// clear preferences view bit
	current_view &= ~Preferences_View;
}

void MainWindow::show_tool_view()
{
	tool_animation.setStartValue(tool_view.pos());
	tool_animation.setEndValue(TOOL_VIEW_SHOW_POINT);

	tool_animation.setDuration(ANIMATION_TIME);
	tool_animation.stop();
	tool_animation.start();

	// enable focus in tool view
	tool_view.enable_focus();
	// set tool view bit
	current_view |= Tool_View;
}

void MainWindow::hide_tool_view()
{
	tool_animation.setStartValue(tool_view.pos());
	tool_animation.setEndValue(TOOL_VIEW_HIDE_POINT);

	tool_animation.setDuration(ANIMATION_TIME);
	tool_animation.stop();
	tool_animation.start();

	// disable focus in tool view
	tool_view.disable_focus();
	this->setFocus();
	// clear tool view bit
	current_view &= ~Tool_View;
}

void MainWindow::show_help_view()
{
	// hide tool view but keep bit set
	if(current_view & Tool_View)
	{
		tool_animation.setStartValue(tool_view.pos());
		tool_animation.setEndValue(TOOL_VIEW_HIDE_POINT);
		tool_animation.setDuration(ANIMATION_TIME);
		tool_animation.stop();
		tool_animation.start();

		// disable focus in tool view
		tool_view.disable_focus();
		this->setFocus();
	}

	if(current_view & Preferences_View)
	{
		// reduce preferences view
		preferences_animation.setStartValue(preferences_view.pos());
		preferences_animation.setEndValue(QPoint(help_view.width(), street_view.y()));
		preferences_animation.setDuration(ANIMATION_TIME);
		preferences_animation.stop();
		preferences_animation.start();
	}

	help_animation.setStartValue(help_view.pos());
	help_animation.setEndValue(QPoint(street_view.x(), street_view.y()));
	help_animation.setDuration(ANIMATION_TIME);
	help_animation.stop();
	help_animation.start();

	// enable focus in help view
	help_view.enable_focus();
	// set help view bit
	current_view |= Help_View;
}

void MainWindow::hide_help_view()
{
	if(current_view & Preferences_View)
	{
		// extend preferences view
		preferences_animation.setStartValue(preferences_view.pos());
		preferences_animation.setEndValue(QPoint(width() - preferences_view.width(), street_view.y()));
		preferences_animation.setDuration(ANIMATION_TIME);
		preferences_animation.stop();
		preferences_animation.start();
	}
	// if preferences view is not active, but tool view
	else if(current_view & Tool_View)
		show_tool_view();

	help_animation.setStartValue(help_view.pos());
	help_animation.setEndValue(QPoint(-help_view.width(), street_view.y()));
	help_animation.setDuration(ANIMATION_TIME);
	help_animation.stop();
	help_animation.start();

	// disable focus in help view
	help_view.disable_focus();
	if(!(current_view & Preferences_View))
		this->setFocus();
	// clear help view bit
	current_view &= ~Help_View;
}

void MainWindow::show_info_view()
{
	info_animation.setStartValue(info_view.pos());
	info_animation.setEndValue(QPoint(width() - info_view.width(), height() - info_view.height()));
	info_animation.setDuration(ANIMATION_TIME);
	info_animation.stop();
	info_animation.start();

	current_view |= Info_View;
}

void MainWindow::hide_info_view()
{
	info_animation.setStartValue(info_view.pos());
	info_animation.setEndValue(QPoint(width() - info_view.width(), height()));
	info_animation.setDuration(ANIMATION_TIME);
	info_animation.stop();
	info_animation.start();

	info_view.disable_focus();
	this->setFocus();
	current_view &= ~Info_View;
}
