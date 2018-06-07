#ifndef CAR_SYSTEM_H
#define CAR_SYSTEM_H

#include "car.h"
#include "configuration.h"
#include <vector>

class Car_System
{
	std::vector< std::vector<Car> > system[2];
	int current_system;
	std::size_t car_id;
	Configuration config;

public:
	Car_System();

	void generate();
	void new_random();

	// pos: 0 -> (x-1); lane: 0 -> (y-1)
	bool is_car_at(const std::size_t& pos, const std::size_t& lane = 0) { return system[current_system][lane][pos].speed != -1; }
	const signed char& get_speed(const std::size_t& pos, const std::size_t& lane = 0) { return system[current_system][lane][pos].speed; }
	const std::size_t& get_id(const std::size_t& pos, const std::size_t& lane = 0) { return system[current_system][lane][pos].id; }
};

#endif // CAR_SYSTEM_H
