#include "car_system.h"
#include <random>
#include <chrono>

Car_System::Car_System() : car_id(0)
{
	slow_down.resize(config.get_street_lanes());
	barriers.resize(config.get_street_lanes());

	new_random();
}

void Car_System::generate()
{
	auto begin = std::chrono::high_resolution_clock::now();
	// random machine
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<std::size_t> dist_slow_down(0, 99);

	for(auto& a : cars)
	{
		// determine gap to next car
		std::size_t pos_temp = a.pos, timeout = 0;
		while(timeout++ < config.get_max_speed()
			  && !is_car_at(++pos_temp % config.get_street_length(), a.lane)
			  && !is_barrier_at(pos_temp % config.get_street_length(), a.lane))
			;

		std::size_t gap = 0;
		if(timeout)
			 gap = timeout - 1;

		if(a.speed < config.get_max_speed())
			++a.speed;
		if(gap < a.speed)
			a.speed = gap;

		// random slow down car is driving (speed != 0)
		if(dist_slow_down(mt) < config.get_slow_down_chance() && a.speed)
			--a.speed;

		std::size_t speed_copy = a.speed;
		for(std::size_t i = 1; i < speed_copy && a.speed; ++i)
		{
			if(is_slow_down_at(a.pos + i, a.lane))
				--a.speed;
		}

			if(config.get_lane_rules())
				;
	}

	// update pos
	for(auto& a : cars)
	{
		a.pos += a.speed;
		a.pos %= config.get_street_length();
	}

	auto end = std::chrono::high_resolution_clock::now();
	performance_us = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
}

void Car_System::new_random()
{
	// random machine
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<std::size_t> dist(0, 99);

	// generate new cars
	for(std::size_t lane = 0; lane < config.get_street_lanes(); ++lane)
	{
		for(std::size_t pos = 0; pos < config.get_street_length(); ++pos)
		{
			std::size_t random = dist(mt);
			if(random < config.get_car_density())
			{
				Car temp;
				temp.id = car_id++;
				temp.speed = (random % (config.get_max_speed() + 1));
				temp.pos = pos;
				temp.lane = lane;
				cars.emplace_back(std::move(temp));
			}
		}
	}
}
