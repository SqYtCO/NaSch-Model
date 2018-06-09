#ifndef CAR_SYSTEM_H
#define CAR_SYSTEM_H

#include "car.h"
#include "configuration.h"
#include <vector>
#include <algorithm>

class Car_System
{
	std::vector< std::vector<Car> > system[2];
	int current_system;
	std::size_t car_id;
	Configuration config;
	std::vector< std::vector<std::size_t> > slow_down;

public:
	Car_System();

	void generate();
	void new_random();

	// pos: 0 -> (x-1); lane: 0 -> (y-1)
	bool is_car_at(const std::size_t& pos, const std::size_t& lane = 0) { return system[current_system][lane][pos].speed > -1; }
	bool is_slow_down_at(const std::size_t& pos, const std::size_t& lane = 0) { return std::find(slow_down[lane].begin(), slow_down[lane].end(), pos) != slow_down[lane].end(); }
	bool is_barrier_at(const std::size_t& pos, const std::size_t& lane = 0) { return system[current_system][lane][pos].speed == -2; }
	const signed char& get_speed(const std::size_t& pos, const std::size_t& lane = 0) { return system[current_system][lane][pos].speed; }
	const std::size_t& get_id(const std::size_t& pos, const std::size_t& lane = 0) { return system[current_system][lane][pos].id; }
};

#endif // CAR_SYSTEM_H
