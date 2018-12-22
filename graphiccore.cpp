#include "graphiccore.h"
#include "core.h"
#ifdef ENABLE_CALC_TIME_MEASUREMENT
#include <QDebug>
#include <chrono>
#endif

GraphicConfiguration GraphicCore::config;
Running_Configuration GraphicCore::run_config;
StreetWidget* GraphicCore::street;
ChartWidget* GraphicCore::chart;
std::mutex GraphicCore::sync_mutex;
std::unique_ptr<std::thread> GraphicCore::run_thread;
std::atomic_bool GraphicCore::stop_thread;
bool GraphicCore::block_thread;

void GraphicCore::init()
{
	street = nullptr;
	chart = nullptr;
	run_thread.reset();
}

void GraphicCore::init_gui(StreetWidget* street, ChartWidget* chart)
{
	GraphicCore::street = street;
	GraphicCore::chart = chart;
}

void GraphicCore::new_game()
{
	stop();
	Core::new_game();
	street->update_data();
	street->update();
	add_chart_point();
}

void GraphicCore::reset()
{
	stop();
	Core::reset();
	street->update();
}

void GraphicCore::start()
{
	stop_thread = false;

	if(!run_thread)
		run_thread.reset(new std::thread([]()
		{
			while(!stop_thread)
			{
				sync_mutex.lock();
				Core::step();
				sync_mutex.unlock();
				street->update_data();
				street->update();
				std::this_thread::sleep_for(std::chrono::milliseconds(config.get_delay()));
			}
		}));
}

void GraphicCore::stop()
{
	stop_thread = true;

	if(run_thread)
	{
		run_thread->join();
		run_thread.reset();
	}
}

void GraphicCore::step()
{
	stop();

#ifdef ENABLE_CALC_TIME_MEASUREMENT
	auto begin = std::chrono::high_resolution_clock::now();
#endif
	sync_mutex.lock();
	Core::step();
	sync_mutex.unlock();
	street->update_data();
	street->update();

	add_chart_point();

#ifdef ENABLE_CALC_TIME_MEASUREMENT
	auto end = std::chrono::high_resolution_clock::now();
	qInfo() << "calc: " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
#endif
}

void GraphicCore::add_chart_point()
{
	if(chart)
	{
		chart->set_slow_down_data(Core::get_slow_down_chance(), Core::get_avg_speed(), Core::get_time());

		std::vector<std::size_t> speeds(Core::get_max_speed() + 1, 0);
		for(std::size_t lane = 0; lane < Core::get_lanes(); ++lane)
		{
			for(std::size_t pos = 0; pos < Core::get_length(); ++pos)
			{
				long speed = Core::get_speed(pos, lane);
				if(speed < 0 || speed > Core::get_max_speed())
					continue;

				++speeds[speed];
			}
		}
		chart->set_car_speed_data(speeds);
	}
}

bool GraphicCore::load(const QString& file)
{
	if(file == "hi")
		;
	return false;
}

bool GraphicCore::save(const QString& file)
{
	if(file == "hi")
		;
	return false;
}
