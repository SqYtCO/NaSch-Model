#include "graphiccore.h"
#include "core.h"
#ifdef ENABLE_CALC_TIME_MEASUREMENT
#include <QDebug>
#include <chrono>
#endif

GraphicCore::GraphicCore() : street(new StreetWidget(&parent)), run_thread(nullptr)
{
#ifdef CREATE_CHARTS
	chart = new ChartWidget();
#endif
}

GraphicCore::~GraphicCore()
{
	if(run_thread)
	{
		run_thread->join();
		delete run_thread;
	}
}

GraphicCore* GraphicCore::get_instance()
{
	static GraphicCore gcore;

	return &gcore;
}

void GraphicCore::new_game()
{
	stop();
	Core::get_instance()->new_game();
	street->update_data();
	street->update();
}

void GraphicCore::reset()
{
	stop();
	Core::get_instance()->reset();
	street->update();
}

void GraphicCore::start()
{
	stop_thread = false;

	if(!run_thread)
		run_thread = new std::thread(&GraphicCore::run, this);
}

void GraphicCore::stop()
{
	stop_thread = true;

	if(run_thread)
	{
		run_thread->join();
		delete run_thread;
		run_thread = nullptr;
	}
}

void GraphicCore::step()
{
	stop();

#ifdef ENABLE_CALC_TIME_MEASUREMENT
	auto begin = std::chrono::high_resolution_clock::now();
#endif
	Core::get_instance()->step();
	street->update_data();
	street->update();

#ifdef CREATE_CHARTS
	chart->set_slow_down_data(Core::get_instance()->get_slow_down_chance(), Core::get_instance()->get_avg_speed(), Core::get_instance()->get_time());

	std::vector<std::size_t> speeds(Core::get_instance()->get_max_speed() + 1, 0);
	for(std::size_t lane = 0; lane < Core::get_instance()->get_lanes(); ++lane)
	{
		for(std::size_t pos = 0; pos < Core::get_instance()->get_length(); ++pos)
		{
			long speed = Core::get_instance()->get_speed(pos, lane);
			if(speed < 0 || speed > Core::get_instance()->get_max_speed())
				continue;

			++speeds[speed];
		}
	}
	chart->set_car_speed_data(speeds);
#endif

#ifdef ENABLE_CALC_TIME_MEASUREMENT
	auto end = std::chrono::high_resolution_clock::now();
	qInfo() << "calc: " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
#endif
}

void GraphicCore::run()
{
	while(!stop_thread)
	{
		Core::get_instance()->step();
		street->update_data();
		street->update();
		std::this_thread::sleep_for(std::chrono::milliseconds(config.get_delay()));
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
