#include "core.h"

Core::Core() : system(config.get_street_length(), config.get_street_lanes(), config.get_car_density(), config.get_max_speed(), config.get_slow_down_chance()), time(0)
{

}

Core* Core::get_instance()
{
	static Core core;

	return &core;
}

bool Core::save(std::string file)
{
	if(file == "hi")
		;
	return false;
}

bool Core::load(const std::string& file)
{
	if(file == "hi")
		;
	return false;
}

void Core::new_game()
{
	system.new_random(config.get_street_length(), config.get_street_lanes(), config.get_car_density(), config.get_max_speed());
	time = 0;
}

void Core::step()
{
	system.generate();
	++time;
}
