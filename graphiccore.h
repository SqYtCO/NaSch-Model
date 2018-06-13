#ifndef GRAPHICCORE_H
#define GRAPHICCORE_H

#include "graphicconfiguration.h"
#include "running_configuration.h"
#include "streetwidget.h"
#ifdef CREATE_CHARTS
#include "chartwidget.h"
#endif
#include <QString>
#include <thread>
#include <atomic>

class GraphicCore
{
	GraphicConfiguration config;
	Running_Configuration run_config;
	QWidget parent;
	StreetWidget* street;
#ifdef CREATE_CHARTS
	ChartWidget* chart;
#endif

	std::thread* run_thread;
	std::atomic_bool stop_thread;
	bool block_thread;
	void run();

	// private constructors and assignment operator
	GraphicCore();
	GraphicCore(const GraphicCore&);
	GraphicCore& operator=(const GraphicCore&);

public:
	static GraphicCore* get_instance();
	~GraphicCore();

	void new_game();
	void reset();
	void start();
	void stop();
	void step();

	void reset_position() { street->reset_position(); street->update(); }
	void update() { street->update_data(); street->update(); }

	bool is_running() { return run_thread; }

	bool load(const QString& file);
	bool save(const QString& file = QString());

	GraphicConfiguration* get_config() { return &config; }
	Running_Configuration* get_run_config() { return &run_config; }
	StreetWidget* get_street() { return street; }
#ifdef CREATE_CHARTS
	ChartWidget* get_chart() { return chart; }
#endif
};

#endif // GRAPHICCORE_H
