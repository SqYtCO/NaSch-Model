#include "car_system.h"
#include "core.h"
#include <random>

Car_System::Car_System(const std::size_t& length, const std::size_t& lanes, const std::size_t& car_density,
					   const std::size_t& max_speed, const std::size_t& slow_down_chance) : car_id(0), length(length), lanes(lanes),
																							car_density(car_density), max_speed(max_speed),
																							slow_down_chance(slow_down_chance)
{
	slow_down.resize(lanes);
	barriers.resize(lanes);

	new_random(length, lanes, car_density, max_speed);
}

void Car_System::calculate()
{
	// random machine
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<std::size_t> dist_slow_down(0, 99);

	for(auto& a : cars)
	{
		// determine gap to next car
		std::size_t pos_temp = a.pos, timeout = 0;
		while(timeout++ < max_speed
			  && !is_car_at(++pos_temp % length, a.lane)
			  && !is_barrier_at(pos_temp % length, a.lane))
			;

		std::size_t gap = 0;
		if(timeout)
			 gap = timeout - 1;

		if(a.speed < max_speed)
			++a.speed;
		if(gap < a.speed)
			a.speed = gap;

		// random slow down car is driving (speed != 0)
		if(dist_slow_down(mt) < slow_down_chance && a.speed)
			--a.speed;

		std::size_t speed_copy = a.speed;
		for(std::size_t i = 1; i < speed_copy && a.speed; ++i)
		{
			if(is_slow_down_at(a.pos + i, a.lane))
				--a.speed;
		}

		//	if(Core::get_instance()->get_config()->get_lane_rules())
				;
	}
}

void Car_System::generate()
{
	calculate();

	// update pos
	for(auto& a : cars)
	{
		a.pos += a.speed;
		a.pos %= length;
	}
}

void Car_System::new_random(const std::size_t& length, const std::size_t& lanes, const std::size_t& car_density, const std::size_t& max_speed)
{
	this->length = length;
	this->lanes = lanes;
	this->car_density = car_density;
	this->max_speed = max_speed;

	// random machine
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<std::size_t> dist(0, 99);

	// clear old cars
	cars.clear();

	// generate new cars
	for(std::size_t lane = 0; lane < this->lanes; ++lane)
	{
		for(std::size_t pos = 0; pos < this->length; ++pos)
		{
			std::size_t random = dist(mt);
			if(random < this->car_density)
			{
				Car temp;
				temp.id = car_id++;
				temp.speed = random % (this->max_speed + 1);
				temp.pos = pos;
				temp.lane = lane;
				cars.emplace_back(std::move(temp));
			}
		}
	}
}

void Car_System::reset()
{
	cars.clear();
	slow_down.clear();
	barriers.clear();
}

void Car_System::add_car(const std::size_t &speed, const std::size_t &pos, const std::size_t &lane)
{
	Car temp;
	temp.id = car_id++;
	temp.speed = speed;
	temp.pos = pos;
	temp.lane = lane;
	cars.emplace_back(std::move(temp));
}

void Car_System::remove_car(const std::size_t &pos, const std::size_t &lane)
{
	for(auto a = cars.begin(); a != cars.end(); ++a)
		if(a->pos == pos && a->lane == lane)
		{
			cars.erase(a);
			return;
		}
}

void Car_System::set_car_speed(const std::size_t& speed, const std::size_t& pos, const std::size_t& lane)
{
	for(auto& a : cars)
		if(a.pos == pos && a.lane == lane)
			a.speed = speed;
}

void Car_System::add_barrier(const std::size_t& pos, const std::size_t& lane)
{
	barriers[lane].push_back(pos);
}

void Car_System::remove_barrier(const std::size_t& pos, const std::size_t& lane)
{
	for(auto a = barriers[lane].begin(); a != barriers[lane].end(); ++a)
		if(*a == pos)
		{
			barriers[lane].erase(a);
			return;
		}
}

void Car_System::add_slow_down(const std::size_t& pos, const std::size_t& lane)
{
	slow_down[lane].push_back(pos);
}

void Car_System::remove_slow_down(const std::size_t& pos, const std::size_t& lane)
{
	for(auto a = slow_down[lane].begin(); a != slow_down[lane].end(); ++a)
		if(*a == pos)
		{
			slow_down[lane].erase(a);
			return;
		}
}

double Car_System::get_avg_speed()
{
	std::size_t avg_speed = 0;

	for(const auto& a : cars)
		avg_speed += a.speed;

	return (static_cast<double>(avg_speed) / cars.size());
}

const Car& Car_System::get_car(const std::size_t& pos, const std::size_t& lane)
{
	for(const auto& a : cars)
		if(a.pos == pos && a.lane == lane)
			return a;

	return cars[0];
}

std::size_t Car_System::get_speed(const std::size_t& pos, const std::size_t& lane)
{
	for(const auto& a : cars)
		if(a.pos == pos && a.lane == lane)
			return a.speed;

	return 0;
}

std::size_t Car_System::get_id(const std::size_t& pos, const std::size_t& lane)
{
	for(const auto& a : cars)
		if(a.pos == pos && a.lane == lane)
			return a.id;

	return 0;
}

bool Car_System::is_car_at(const std::size_t& pos, const std::size_t& lane)
{
	for(const auto& a : cars)
		if(a.pos == pos && a.lane == lane)
			return true;

	return false;
}

bool Car_System::is_slow_down_at(const std::size_t& pos, const std::size_t& lane)
{
	return std::find(slow_down[lane].begin(), slow_down[lane].end(), pos) != slow_down[lane].end();
}

bool Car_System::is_barrier_at(const std::size_t& pos, const std::size_t& lane)
{
	return std::find(barriers[lane].begin(), barriers[lane].end(), pos) != barriers[lane].end();
}
