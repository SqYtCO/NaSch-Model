#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <cstddef>
#include <string>

namespace Default_Values
{
	constexpr std::size_t STREET_LENGTH = 1000;
	constexpr std::size_t STREET_LANES = 1;
	constexpr std::size_t SLOW_DOWN_CHANCE = 0;
	constexpr std::size_t CAR_DENSITY = 5;
	constexpr std::size_t MAX_SPEED = 20;
	constexpr bool LANE_RULES = false;
}

class Configuration
{
	bool saved;

protected:
	static constexpr const char* CONFIG_FILE = ".configuration";
	std::string config_path;

public:
	Configuration();

	// config_path has to be empty or end on a separator; after setting config_path read config
	void set_config_path(const std::string& new_config_path) { config_path = new_config_path; read_config(); }

	bool write_config();
	bool read_config();
	void reset_config();

	const bool& is_saved() { return saved; }

// options
private:
	std::size_t street_length;
	std::size_t street_lanes;
	std::size_t slow_down_chance;		// percent
	std::size_t car_density;			// percent
	std::size_t max_speed;
	bool lane_rules;

// get options
public:
	inline std::size_t get_street_length() const { return street_length; }
	inline std::size_t get_street_lanes() const { return street_lanes; }
	inline std::size_t get_slow_down_chance() const { return slow_down_chance; }
	inline std::size_t get_car_density() const { return car_density; }
	inline std::size_t get_max_speed() const { return max_speed; }
	inline const bool& get_lane_rules() const { return lane_rules; }
//	const bool& get_() const { return ; }

// set options
public:
	inline void set_street_length(std::size_t new_street_length) { saved = false; street_length = new_street_length; }
	inline void set_street_lanes(std::size_t new_street_lanes) { saved = false; street_lanes = new_street_lanes; }
	inline void set_slow_down_chance(std::size_t new_slow_down_chance) { saved = false; slow_down_chance = new_slow_down_chance; }
	inline void set_car_density(std::size_t new_car_density) { saved = false; car_density = new_car_density; }
	inline void set_max_speed(std::size_t new_max_speed) { saved = false; max_speed = new_max_speed; }
	inline void set_lane_rules(const bool& new_lane_rules) { saved = false; lane_rules = new_lane_rules; }
//	void set_(const bool& new_) {  = new_; }
};

#endif // CONFIGURATION_H
