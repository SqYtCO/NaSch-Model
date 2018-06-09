#ifndef CAR_SYSTEM_H
#define CAR_SYSTEM_H

#include "car.h"
#include "configuration.h"
#include <vector>
#include <algorithm>

class Car_System
{
	std::vector<Car> cars;
	std::size_t car_id;
	Configuration config;
	std::vector< std::vector<std::size_t> > slow_down;
	std::vector< std::vector<std::size_t> > barriers;

	std::size_t performance_us;

public:
	Car_System();

	void generate();
	void new_random();

	const Car& get_car(const std::size_t& pos, const std::size_t& lane = 0)
	{
		for(const auto& a : cars)
			if(a.pos == pos && a.lane == lane)
				return a;

		return cars[0];
	}

	// pos: 0 -> (x-1); lane: 0 -> (y-1)
	bool is_car_at(const std::size_t& pos, const std::size_t& lane = 0)
	{
		for(const auto& a : cars)
			if(a.pos == pos && a.lane == lane)
				return true;

		return false;
	}
	bool is_slow_down_at(const std::size_t& pos, const std::size_t& lane = 0) { return std::find(slow_down[lane].begin(), slow_down[lane].end(), pos) != slow_down[lane].end(); }
	bool is_barrier_at(const std::size_t& pos, const std::size_t& lane = 0) { return std::find(barriers[lane].begin(), barriers[lane].end(), pos) != barriers[lane].end(); }
	std::size_t get_speed(const std::size_t& pos, const std::size_t& lane = 0)
	{
		for(const auto& a : cars)
			if(a.pos == pos && a.lane == lane)
				return a.speed;

		return 0;
	}
	std::size_t get_id(const std::size_t& pos, const std::size_t& lane = 0)
	{
		for(const auto& a : cars)
			if(a.pos == pos && a.lane == lane)
				return a.id;

		return 0;
	}
};

#endif // CAR_SYSTEM_H
