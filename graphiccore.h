#ifndef GRAPHICCORE_H
#define GRAPHICCORE_H

#include "graphicconfiguration.h"
#include "running_configuration.h"
#include "streetwidget.h"
#ifdef CREATE_CHARTS
#include "chartwidget.h"
#endif
#include <thread>
#include <atomic>
#include <memory>

class GraphicCore
{
	static GraphicConfiguration config;
	static Running_Configuration run_config;
	static StreetWidget* street;
	static ChartWidget* chart;

	static std::mutex sync_mutex;
	static std::unique_ptr<std::thread> run_thread;
	static std::atomic_bool stop_thread;
	static bool block_thread;

	static void add_chart_point();

public:
	static void init();
	// no ownership transfer
	static void init_gui(StreetWidget* street, ChartWidget* chart = nullptr);

	static void new_game();
	static void reset();
	static void start();
	static void stop();
	static void step();
	static std::mutex& get_mutex() { return sync_mutex; }

	static void reset_position() { street->reset_position(); street->update(); }
	static void update() { street->update_data(); street->update(); }

	static bool is_running() { return run_thread.get(); }

	static bool load(const std::string& file);
	static bool save(const std::string& file = std::string());

	static GraphicConfiguration* get_config() { return &config; }
	static Running_Configuration* get_run_config() { return &run_config; }
};

#endif // GRAPHICCORE_H
