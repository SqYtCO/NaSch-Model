#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <cstddef>

namespace Default_Values
{
	constexpr std::size_t STREET_LENGTH = 1000;
	constexpr std::size_t STREET_LANES = 1;
	constexpr std::size_t CELL_SIZE = 50;
	constexpr std::size_t SLOW_DOWN_CHANCE = 0;
	constexpr std::size_t CAR_DENSITY = 20;
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

// get options
public:
	const std::size_t& get_street_length() const { return street_length; }
	const std::size_t& get_street_lanes() const { return street_lanes; }
	const std::size_t& get_cell_size() const { return cell_size; }
	const std::size_t& get_slow_down_chance() const { return slow_down_chance; }
	const std::size_t& get_car_density() const { return car_density; }
	const std::size_t& get_max_speed() const { return max_speed; }

// set options
public:
	void set_street_length(const std::size_t& new_street_length) { street_length = new_street_length; }
	void set_street_lanes(const std::size_t& new_street_lanes) { street_lanes = new_street_lanes; }
	void set_cell_size(const std::size_t& new_cell_size) { cell_size = new_cell_size; }
	void set_slow_down_chance(const std::size_t& new_slow_down_chance) { slow_down_chance = new_slow_down_chance; }
	void set_car_density(const std::size_t& new_car_density) { car_density = new_car_density; }
	void set_max_speed(const std::size_t& new_max_speed) { max_speed = new_max_speed; }
};

#endif // CONFIGURATION_H
