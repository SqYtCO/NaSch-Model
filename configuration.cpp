#include "configuration.h"

Configuration::Configuration()
{
	if(!read_config())
		reset_config();
}

bool Configuration::write_config()
{
	return false;
}

bool Configuration::read_config()
{
	return false;
}

void Configuration::reset_config()
{
	street_length = Default_Values::STREET_LENGTH;
	street_lanes = Default_Values::STREET_LANES;
	cell_size = Default_Values::CELL_SIZE;
	slow_down_chance = Default_Values::SLOW_DOWN_CHANCE;
	car_density = Default_Values::CAR_DENSITY;
	max_speed = Default_Values::MAX_SPEED;
	diagram_mode = Default_Values::DIAGRAM_MODE;
	show_speed_color = Default_Values::SHOW_SPEED_COLOR;
	long_street_break = Default_Values::LONG_STREET_BREAK;
	show_cars = Default_Values::SHOW_CARS;
}
