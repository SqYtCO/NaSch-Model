#ifndef CORE_H
#define CORE_H

#include "car_system.h"
#include "configuration.h"
#include <string>

class Core
{
	Configuration config;

	Car_System system;
	std::size_t time;

public:
	// private constructors and assignment operator
	Core();
	Core(const Core&) = delete;
	Core& operator=(const Core&) = delete;

public:
	// singleton pattern - only one core per application; get this single instance by calling Core::get_instance()
	static Core* get_instance();
	~Core() = default;

	bool save(std::string file = std::string());
	bool load(const std::string& file);

	void new_game();

	void start();
	void stop();
	void step();

	void reset() { system.reset(); }

	void add_car(const std::size_t& speed, const std::size_t& pos, const std::size_t& lane = 0) { system.add_car(speed, pos, lane); }
	void remove_car(const std::size_t& pos, const std::size_t& lane = 0) { system.remove_car(pos, lane); }
	void set_car_speed(const std::size_t& speed, const std::size_t& pos, const std::size_t& lane = 0) { system.set_car_speed(speed, pos, lane); }

	void add_barrier(const std::size_t& pos, const std::size_t& lane = 0) { system.add_barrier(pos, lane); }
	void remove_barrier(const std::size_t& pos, const std::size_t& lane = 0) { system.remove_barrier(pos, lane); }

	void add_slow_down(const std::size_t& pos, const std::size_t& lane = 0) { system.add_slow_down(pos, lane); }
	void remove_slow_down(const std::size_t& pos, const std::size_t& lane = 0) { system.remove_slow_down(pos, lane); }

	double get_avg_speed() { return system.get_avg_speed(); }
	std::size_t get_car_amount() { return system.get_car_amount(); }
	const std::size_t& get_time() { return time; }

	const Car& get_car(const std::size_t& pos, const std::size_t& lane = 0) { return system.get_car(pos, lane); }
	bool is_car_at(const std::size_t& pos, const std::size_t& lane = 0) { return system.is_car_at(pos, lane); }
	bool is_slow_down_at(const std::size_t& pos, const std::size_t& lane = 0) { return system.is_slow_down_at(pos, lane); }
	bool is_barrier_at(const std::size_t& pos, const std::size_t& lane = 0) { return system.is_barrier_at(pos, lane); }
	std::size_t get_speed(const std::size_t& pos, const std::size_t& lane = 0) { return system.get_speed(pos, lane); }
	std::size_t get_id(const std::size_t& pos, const std::size_t& lane = 0) { return system.get_id(pos, lane); }

	const std::size_t& get_length() { return system.get_length(); }
	const std::size_t& get_lanes() { return system.get_lanes(); }
	const std::size_t& get_car_density() { return system.get_car_density(); }
	const std::size_t& get_max_speed() { return system.get_max_speed(); }
	const std::size_t& get_slow_down_chance() { return system.get_slow_down_chance(); }

	Configuration* get_config() { return &config; }
};

#endif // CORE_H
