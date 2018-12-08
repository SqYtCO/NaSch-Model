#ifndef CAR_SYSTEM_H
#define CAR_SYSTEM_H

#include "configuration.h"
#include <vector>
#include <algorithm>

class Car_System
{
	std::vector<std::size_t> system;
	std::size_t car_id;
	std::size_t length;
	std::size_t lanes;
	std::size_t car_density;
	std::size_t max_speed;
	std::size_t slow_down_chance;

	static constexpr const std::size_t SLOW_DOWN_BIT = (1ul << 63);
	static constexpr const std::size_t EMPTY = (std::numeric_limits<std::size_t>::max() & ~SLOW_DOWN_BIT);
	static constexpr const std::size_t BARRIER = (EMPTY - 1);

	inline bool is_empty(std::size_t num) const { clear_slow_down_bit(num); return num == EMPTY; }
	inline std::size_t set_empty(std::size_t num) const { return num | EMPTY; }

	inline std::size_t set_slow_down_bit(std::size_t num) const { return num | SLOW_DOWN_BIT; }
	inline std::size_t clear_slow_down_bit(std::size_t num) const { return num & ~SLOW_DOWN_BIT; }

	void calculate();

public:
	Car_System(std::size_t length, std::size_t lanes, std::size_t car_density, std::size_t max_speed, std::size_t slow_down_chance);

	void generate();
	void create(std::size_t length, std::size_t lanes, std::size_t car_density, std::size_t max_speed, std::size_t slow_down_chance);

	void reset();

	void add_car(std::size_t speed, std::size_t pos, std::size_t lane = 0);
	void remove_car(std::size_t pos, std::size_t lane = 0);
	void set_car_speed(std::size_t speed, std::size_t pos, std::size_t lane = 0);

	void add_barrier(std::size_t pos, std::size_t lane = 0);
	void remove_barrier(std::size_t pos, std::size_t lane = 0);

	void add_slow_down(std::size_t pos, std::size_t lane = 0);
	void remove_slow_down(std::size_t pos, std::size_t lane = 0);

	double get_avg_speed();
	std::size_t get_car_amount();

	std::size_t get_speed(std::size_t pos, std::size_t lane = 0);
	long get_id(std::size_t pos, std::size_t lane = 0);
	bool is_car_at(std::size_t pos, std::size_t lane = 0);
	bool is_slow_down_at(std::size_t pos, std::size_t lane = 0);
	bool is_barrier_at(std::size_t pos, std::size_t lane = 0);

	std::size_t get_length() { return length; }
	std::size_t get_lanes() { return lanes; }
	std::size_t get_car_density() { return car_density; }
	std::size_t get_max_speed() { return max_speed; }
	std::size_t get_slow_down_chance() { return slow_down_chance; }
};

#endif // CAR_SYSTEM_H
