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
	std::vector< std::vector<std::size_t> > slow_down;
	std::vector< std::vector<std::size_t> > barriers;
	std::size_t length;
	std::size_t lanes;
	std::size_t car_density;
	std::size_t max_speed;
	std::size_t slow_down_chance;

	void calculate();

public:
	Car_System(const std::size_t& length, const std::size_t& lanes, const std::size_t& car_density, const std::size_t& max_speed, const std::size_t& slow_down_chance);

	void generate();
	void create(const std::size_t& length, const std::size_t& lanes, const std::size_t& car_density, const std::size_t& max_speed, const std::size_t& slow_down_chance);

	void reset();

	void add_car(const std::size_t& speed, const std::size_t& pos, const std::size_t& lane = 0);
	void remove_car(const std::size_t& pos, const std::size_t& lane = 0);
	void set_car_speed(const std::size_t& speed, const std::size_t& pos, const std::size_t& lane = 0);

	void add_barrier(const std::size_t& pos, const std::size_t& lane = 0);
	void remove_barrier(const std::size_t& pos, const std::size_t& lane = 0);

	void add_slow_down(const std::size_t& pos, const std::size_t& lane = 0);
	void remove_slow_down(const std::size_t& pos, const std::size_t& lane = 0);

	double get_avg_speed();
	std::size_t get_car_amount() { return cars.size(); }

	const Car& get_car(const std::size_t& pos, const std::size_t& lane = 0);
	long get_speed(const std::size_t& pos, const std::size_t& lane = 0);
	long get_id(const std::size_t& pos, const std::size_t& lane = 0);
	bool is_car_at(const std::size_t& pos, const std::size_t& lane = 0);
	bool is_slow_down_at(const std::size_t& pos, const std::size_t& lane = 0);
	bool is_barrier_at(const std::size_t& pos, const std::size_t& lane = 0);

	const std::size_t& get_length() { return length; }
	const std::size_t& get_lanes() { return lanes; }
	const std::size_t& get_car_density() { return car_density; }
	const std::size_t& get_max_speed() { return max_speed; }
	const std::size_t& get_slow_down_chance() { return slow_down_chance; }
};

#endif // CAR_SYSTEM_H
