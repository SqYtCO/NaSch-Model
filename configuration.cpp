#include "configuration.h"
#include <fstream>
#include <experimental/filesystem>

Configuration::Configuration() : saved(true)
{
	if(!read_config())
		reset_config();
}

void Configuration::reset_config()
{
	street_length = Default_Values::STREET_LENGTH;
	street_lanes = Default_Values::STREET_LANES;
	slow_down_chance = Default_Values::SLOW_DOWN_CHANCE;
	car_density = Default_Values::CAR_DENSITY;
	max_speed = Default_Values::MAX_SPEED;
}

bool Configuration::read_config()
{
	std::ifstream in(config_path + CONFIG_FILE);

	// return on error
	if(!in)
		return false;

	// set default values if properties are missing in config_file
	reset_config();

	std::string property, value;
	// read in property name
	while(std::getline(in, property, '='))
	{
		// read in value of property
		std::getline(in, value);

		if(property == "street_length")
			street_length = std::stoul(value);
		else if(property == "street_lanes")
			street_lanes = std::stoul(value);
		else if(property == "slow_down_chance")
			slow_down_chance = std::stoul(value);
		else if(property == "car_density")
			car_density = std::stoul(value);
		else if(property == "max_speed")
			max_speed = std::stoul(value);
	}

	saved = true;

	// return on success
	return true;
}

bool Configuration::write_config()
{
	try
	{
		// check if path exists and create it if not
		if(!config_path.empty() && !std::experimental::filesystem::exists(config_path))
			std::experimental::filesystem::create_directories(config_path);
	}
	catch(...)	// if write permission is not granted
	{
		return false;
	}

	std::ofstream out(config_path + CONFIG_FILE);

	// return on error
	if(!out)
		return false;

	out << "street_length=" << street_length << '\n'
		<< "street_lanes=" << street_lanes << '\n'
		<< "slow_down_chance=" << slow_down_chance << '\n'
		<< "car_density=" << car_density << '\n'
		<< "max_speed=" << max_speed;

	// return on error
	if(!out)
		return false;

	saved = true;

	return true;
}
