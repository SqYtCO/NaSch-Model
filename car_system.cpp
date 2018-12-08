#include "car_system.h"
#include "core.h"
#include <random>

constexpr const std::size_t Car_System::SLOW_DOWN_BIT;
constexpr const std::size_t Car_System::EMPTY;
constexpr const std::size_t Car_System::BARRIER;

Car_System::Car_System(std::size_t length, std::size_t lanes, std::size_t car_density,
					   std::size_t max_speed, std::size_t slow_down_chance) : system(length * lanes), car_id(1), length(length), lanes(lanes),
																							car_density(car_density), max_speed(max_speed),
																							slow_down_chance(slow_down_chance)
{
	create(length, lanes, car_density, max_speed, slow_down_chance);
}

void Car_System::calculate()
{
	// random machine
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<std::size_t> dist_slow_down(0, 99);

	for(std::size_t lane = 0; lane < lanes; ++lane)
	{
		for(std::size_t pos = 0; pos < length; ++pos)
		{
			// skip empty cells
			if(!is_car_at(pos, lane))
				continue;

			// calculate distance to next car or barrier
			std::size_t gap = 0;
			while(gap++ < max_speed)
				if(is_car_at((pos + gap) % length, lane) || is_barrier_at((pos + gap) % length, lane))
					break;
			--gap;

			std::size_t speed = system[pos + lane * length];
			if(speed < max_speed)
				++speed;
			if(gap < speed)
				speed = gap;
			// randomized slow down
			if(dist_slow_down(mt) < slow_down_chance && speed > 0)
				--speed;

			for(std::size_t i = 1; i <= speed; ++i)
				if(is_slow_down_at(pos + i, lane))
					--speed | (is_slow_down_at(pos, lane) ? SLOW_DOWN_BIT : 0);

			system[pos + lane * length] = speed;

			//	if(Core::get_instance()->get_config()->get_lane_rules())
		}
	}
}

void Car_System::generate()
{
	calculate();

	// update pos
	for(std::size_t lane = 0; lane < lanes; ++lane)
	{
		for(std::size_t pos = 0; pos < length; ++pos)
		{
			// skip empty cells
			if(!is_car_at(pos, lane))
				continue;

			std::size_t new_pos = (pos + system[pos + lane * length]) % length;
			system[new_pos + lane * length] = system[pos + lane * length];
			if(pos != new_pos)
				system[pos + lane * length] = set_empty(system[pos + lane * length]);
			if(new_pos < pos)
				break;
			pos = new_pos;
		}
	}
}

void Car_System::create(std::size_t length, std::size_t lanes, std::size_t car_density, std::size_t max_speed, std::size_t slow_down_chance)
{
	this->length = length;
	this->lanes = lanes;
	this->car_density = car_density;
	this->max_speed = max_speed;
	this->slow_down_chance = slow_down_chance;

	system.resize(length * lanes);

	// random machine
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<std::size_t> dist(0, 99);

	// generate new cars
	for(std::size_t lane = 0; lane < this->lanes; ++lane)
	{
		for(std::size_t pos = 0; pos < this->length; ++pos)
		{
			std::size_t random = dist(mt);
			if(random < this->car_density)
				system[pos + lane * length] = random % (this->max_speed + 1);
			else
				system[pos + lane * length] = set_empty(system[pos + lane * length]);
		}
	}
}

void Car_System::reset()
{
	std::fill(system.begin(), system.end(), EMPTY);
}

void Car_System::add_car(std::size_t speed, std::size_t pos, std::size_t lane)
{
	system[pos + lane * length] = speed;
}

void Car_System::remove_car(std::size_t pos, std::size_t lane)
{
	system[pos + lane * length] = set_empty(system[pos + lane * length]);
}

void Car_System::set_car_speed(std::size_t speed, std::size_t pos, std::size_t lane)
{
	if(is_car_at(pos, lane))
	{
		system[pos + lane * length] = speed | system[pos + lane * length];
	}
}

void Car_System::add_barrier(std::size_t pos, std::size_t lane)
{
	system[pos + lane * length] = BARRIER;
}

void Car_System::remove_barrier(std::size_t pos, std::size_t lane)
{
	system[pos + lane * length] = set_empty(system[pos + lane * length]);
}

void Car_System::add_slow_down(std::size_t pos, std::size_t lane)
{
	system[pos + lane * length] = set_slow_down_bit(system[pos + lane * length]);
}

void Car_System::remove_slow_down(std::size_t pos, std::size_t lane)
{
	system[pos + lane * length] = clear_slow_down_bit(system[pos + lane * length]);
}

double Car_System::get_avg_speed()
{
	std::size_t avg_speed = 0;
	std::size_t cars = 0;
	for(std::size_t lane = 0; lane < lanes; ++lane)
	{
		for(std::size_t pos = 0; pos < length; ++pos)
		{
			// skip empty cells
			if(!is_car_at(pos, lane))
				continue;

			avg_speed += system[pos + lane * length];
			++cars;
		}
	}

	return (static_cast<double>(avg_speed) / cars);
}

std::size_t Car_System::get_car_amount()
{
	std::size_t cars = 0;
	for(std::size_t lane = 0; lane < lanes; ++lane)
		for(std::size_t pos = 0; pos < length; ++pos)
			if(is_car_at(pos, lane))
				++cars;
	return cars;
}

std::size_t Car_System::get_speed(std::size_t pos, std::size_t lane)
{
	return system[pos + lane * length];
}

long Car_System::get_id(std::size_t pos, std::size_t lane)
{
	return pos + lane * length;
}

bool Car_System::is_car_at(std::size_t pos, std::size_t lane)
{
	if(!is_empty(system[pos + lane * length]) && system[pos + lane * length] != BARRIER)
		return true;
	return false;
}

bool Car_System::is_slow_down_at(std::size_t pos, std::size_t lane)
{
	return system[pos + lane * length] & SLOW_DOWN_BIT;
}

bool Car_System::is_barrier_at(std::size_t pos, std::size_t lane)
{
	return system[pos + lane * length] == BARRIER;
}
