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
}
