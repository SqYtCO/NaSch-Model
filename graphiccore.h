#ifndef GRAPHICCORE_H
#define GRAPHICCORE_H

#include "graphicconfiguration.h"
#include "running_configuration.h"
#include "streetwidget.h"
#include <QString>
#include <thread>
#include <atomic>

// measure time of calculation while autogenerating in microseconds and output the result on the console
#define ENABLE_CALC_TIME_MEASUREMENT

// measure time of drawing cells in microseconds and output the result on the console
#define ENABLE_DRAW_TIME_MEASUREMENT

class GraphicCore
{
	GraphicConfiguration config;
	Running_Configuration run_config;
	QWidget parent;
	StreetWidget* street;

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
};

#endif // GRAPHICCORE_H
