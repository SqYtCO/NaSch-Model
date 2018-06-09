#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <cstddef>

namespace Default_Values
{
	constexpr std::size_t STREET_LENGTH = 100;
	constexpr std::size_t STREET_LANES = 100;
	constexpr std::size_t CELL_SIZE = 100;
	constexpr std::size_t SLOW_DOWN_CHANCE = 0;
	constexpr std::size_t CAR_DENSITY = 10;
	constexpr std::size_t MAX_SPEED = 50;
	constexpr bool DIAGRAM_MODE = false;
	constexpr bool SHOW_SPEED_COLOR = true;
	constexpr bool LONG_STREET_BREAK = true;
	constexpr bool SHOW_CARS = false;
}

class Configuration
{
public:
	Configuration();

	bool write_config();
	bool read_config();
	void reset_config();

// options
private:
	std::size_t street_length;
	std::size_t street_lanes;
	std::size_t cell_size;
	std::size_t slow_down_chance;		// percent
	std::size_t car_density;			// percent
	std::size_t max_speed;
	bool diagram_mode;
	bool show_speed_color;
	bool long_street_break;
	bool show_cars;

// get options
public:
	const std::size_t& get_street_length() const { return street_length; }
	const std::size_t& get_street_lanes() const { return street_lanes; }
	const std::size_t& get_cell_size() const { return cell_size; }
	const std::size_t& get_slow_down_chance() const { return slow_down_chance; }
	const std::size_t& get_car_density() const { return car_density; }
	const std::size_t& get_max_speed() const { return max_speed; }
	const bool& get_diagram_mode() const { return diagram_mode; }
	const bool& get_show_speed_color() const { return show_speed_color; }
	const bool& get_long_street_break() const { return long_street_break; }
	const bool& get_show_cars() const { return show_cars; }
//	const bool& get_() const { return ; }

// set options
public:
	void set_street_length(const std::size_t& new_street_length) { street_length = new_street_length; }
	void set_street_lanes(const std::size_t& new_street_lanes) { street_lanes = new_street_lanes; }
	void set_cell_size(const std::size_t& new_cell_size) { cell_size = new_cell_size; }
	void set_slow_down_chance(const std::size_t& new_slow_down_chance) { slow_down_chance = new_slow_down_chance; }
	void set_car_density(const std::size_t& new_car_density) { car_density = new_car_density; }
	void set_max_speed(const std::size_t& new_max_speed) { max_speed = new_max_speed; }
	void set_diagram_mode(const bool& new_diagram_mode) { diagram_mode = new_diagram_mode; }
	void set_show_speed_color(const bool& new_show_speed_color) { show_speed_color = new_show_speed_color; }
	void set_long_street_break(const bool& new_long_street_break) { long_street_break = new_long_street_break; }
	void set_show_cars(const bool& new_show_cars) { show_cars = new_show_cars; }
//	void set_(const bool& new_) {  = new_; }
};

#endif // CONFIGURATION_H
