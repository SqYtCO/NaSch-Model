// © Copyright (c) 2018 SqYtCO

#include "helpwidget.h"
#include "graphiccore.h"
#include <QKeyEvent>
#include <QLocale>

HelpWidget::HelpWidget(QWidget* parent) : QFrame(parent)
{
	// grey transparent background
	this->setAutoFillBackground(true);
	this->setPalette(QPalette(QColor(0x40, 0x40, 0x40, 0xE9)));

	// dark border
	this->setFrameShadow(QFrame::Raised);
	this->setFrameShape(QFrame::Panel);
	this->setLineWidth(2);

	this->setFocusProxy(parent);

	init_gui();
}

void HelpWidget::init_gui()
{
	// init hide button
	hide.setArrowType(Qt::LeftArrow);
	QObject::connect(&hide, &QToolButton::clicked, [this]() { emit hide_help(); });

	// init show_index button
	show_index.setIcon(QIcon(":/images/white-index-90.png"));
	QObject::connect(&show_index, &QToolButton::clicked, [this]() { html_help.scrollToAnchor("index"); });

	// add components to layout
	main_layout.addWidget(&show_index, 0, 0);
	main_layout.addWidget(&hide, 0, 2);
	main_layout.addWidget(&html_help, 1, 0, 4, 3);

	// set texts
	translate();

	// set main layout
	this->setLayout(&main_layout);
}

void HelpWidget::translate()
{
	// tooltips
	hide.setToolTip(tr("Hide (Esc)"));
	show_index.setToolTip(tr("Show Index"));

	// load help text in the right language
	if(GraphicCore::get_config()->get_language() == Language::German ||
			(GraphicCore::get_config()->get_language() == Language::System && QLocale::system().language() == QLocale::German))
		html_help.setSource(QUrl("qrc:/info/de_help.html"));
	else
		html_help.setSource(QUrl("qrc:/info/en_help.html"));
}

void HelpWidget::enable_focus()
{
	// allow focus to all children
	for(QObject* a : this->children())
	{
		auto b = dynamic_cast<QWidget*>(a);
		if(b)
			b->setFocusPolicy(Qt::StrongFocus);
	}

	// enable own focus
	this->setFocusPolicy(Qt::StrongFocus);
}

void HelpWidget::disable_focus()
{
	// revoke focus of all children
	for(QObject* a : this->children())
	{
		auto b = dynamic_cast<QWidget*>(a);
		if(b)
			b->setFocusPolicy(Qt::NoFocus);
	}

	// disable own focus
	this->setFocusPolicy(Qt::NoFocus);
}
