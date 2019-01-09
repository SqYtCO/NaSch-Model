#ifndef CORE_H
#define CORE_H

#include "car_system.h"
#include "configuration.h"
#include <string>

class Core
{
	static Configuration config;

	static Car_System system;
	static std::size_t time;

public:
	static bool save(const std::string& file = std::string());
	static bool load(const std::string& file);

	static void new_game();

	static void start();
	static void stop();
	static void step();

	static void reset() { system.reset(); }

	static void add_car(std::size_t speed, std::size_t pos, std::size_t lane = 0) { system.add_car(speed, pos, lane); }
	static void remove_car(std::size_t pos, std::size_t lane = 0) { system.remove_car(pos, lane); }
	static void set_car_speed(std::size_t speed, std::size_t pos, std::size_t lane = 0) { system.set_car_speed(speed, pos, lane); }

	static void add_barrier(std::size_t pos, std::size_t lane = 0) { system.add_barrier(pos, lane); }
	static void remove_barrier(std::size_t pos, std::size_t lane = 0) { system.remove_barrier(pos, lane); }

	static void add_slow_down(std::size_t pos, std::size_t lane = 0) { system.add_slow_down(pos, lane); }
	static void remove_slow_down(std::size_t pos, std::size_t lane = 0) { system.remove_slow_down(pos, lane); }

	static double get_avg_speed() { return system.get_avg_speed(); }
	static std::size_t get_car_amount() { return system.get_car_amount(); }
	static std::size_t get_time() { return time; }

	static bool is_car_at(std::size_t pos, std::size_t lane = 0) { return system.is_car_at(pos, lane); }
	static bool is_slow_down_at(std::size_t pos, std::size_t lane = 0) { return system.is_slow_down_at(pos, lane); }
	static bool is_barrier_at(std::size_t pos, std::size_t lane = 0) { return system.is_barrier_at(pos, lane); }
	static std::size_t get_speed(std::size_t pos, std::size_t lane = 0) { return system.get_speed(pos, lane); }

	static std::size_t get_length() { return system.get_length(); }
	static std::size_t get_lanes() { return system.get_lanes(); }
	static std::size_t get_car_density() { return system.get_car_density(); }
	static std::size_t get_max_speed() { return system.get_max_speed(); }
	static std::size_t get_slow_down_chance() { return system.get_slow_down_chance(); }

	static Configuration* get_config() { return &config; }
};

#endif // CORE_H
