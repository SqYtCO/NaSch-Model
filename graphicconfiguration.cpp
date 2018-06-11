#include "graphicconfiguration.h"
#include <fstream>
#include <experimental/filesystem>

GraphicConfiguration::GraphicConfiguration() : saved(true)
{
	if(!read_config())
		reset_config();
}

void GraphicConfiguration::reset_config()
{
	language = Default_Values::LANGUAGE;
	cell_size = Default_Values::CELL_SIZE;
	diagram_mode = Default_Values::DIAGRAM_MODE;
	show_speed_color = Default_Values::SHOW_SPEED_COLOR;
	long_street_break = Default_Values::LONG_STREET_BREAK;
	show_cars = Default_Values::SHOW_CARS;
	exit_warning = Default_Values::EXIT_WARNING;
	fullscreen = Default_Values::FULLSCREEN;
	show_startup_dialog = Default_Values::SHOW_STARTUP_DIALOG;
	delay = Default_Values::DELAY;
}

bool GraphicConfiguration::read_config()
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

		if(property == "language")
			language = static_cast<Language>(std::stoul(value));
		else if(property == "cell_size")
			cell_size = std::stoul(value);
		else if(property == "diagram_mode")
			diagram_mode = std::stoul(value);
		else if(property == "show_speed_color")
			show_speed_color = std::stoul(value);
		else if(property == "long_street_break")
			long_street_break = std::stoul(value);
		else if(property == "show_cars")
			show_cars = std::stoul(value);
		else if(property == "exit_warning")
			exit_warning = std::stoul(value);
		else if(property == "fullscreen")
			fullscreen = std::stoul(value);
		else if(property == "show_startup_dialog")
			show_startup_dialog = std::stoul(value);
		else if(property == "delay")
			delay = std::stoul(value);
	}

	saved = true;

	// return on success
	return true;
}

bool GraphicConfiguration::write_config()
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

	out << "language=" << static_cast<int>(language) << '\n'
		<< "cell_size=" << cell_size << '\n'
		<< "diagram_mode=" << diagram_mode << '\n'
		<< "show_speed_color=" << show_speed_color << '\n'
		<< "long_street_break=" << long_street_break << '\n'
		<< "show_cars=" << show_cars << '\n'
		<< "exit_warning=" << exit_warning << '\n'
		<< "fullscreen=" << fullscreen << '\n'
		<< "show_startup_dialog=" << show_startup_dialog << '\n'
		<< "delay=" << delay;

	// return on error
	if(!out)
		return false;

	saved = true;

	return true;
}
