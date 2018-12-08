#ifndef INFOWIDGET_H
#define INFOWIDGET_H

#include <QFrame>
#include <QGridLayout>
#include <QLabel>

class InfoWidget : public QFrame
{
	QGridLayout main_layout;

	void init_gui();

public:
	InfoWidget(QWidget* parent = nullptr);

	void update_data();

	// set all texts; if a QTranslator is installed, the installed texts will be loaded
	void translate();

	// enable the focus of all children and the widget
	void enable_focus();
	// disable the focus of all children and the widget
	void disable_focus();

protected:
	void mousePressEvent(QMouseEvent*) { }

// GUI components
private:
	QLabel avg_speed;
	QLabel length;
	QLabel lanes;
	QLabel cars;
	QLabel time;
};

#endif // INFOWIDGET_H
