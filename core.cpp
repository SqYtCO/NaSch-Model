#include "core.h"

Configuration Core::config;
Car_System Core::system;
std::size_t Core::time;

bool Core::save(const std::string& file)
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
	system.create(config.get_street_length(), config.get_street_lanes(), config.get_car_density(), config.get_max_speed(), config.get_slow_down_chance());
	time = 0;
}

void Core::step()
{
	system.generate();
	++time;
}
