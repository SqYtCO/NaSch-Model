#include "car_system.h"
#include <random>

constexpr const std::size_t Car_System::SLOW_DOWN_BIT;
constexpr const std::size_t Car_System::EMPTY;
constexpr const std::size_t Car_System::BARRIER;

Car_System::Car_System(std::size_t length, std::size_t lanes, std::size_t car_density, std::size_t max_speed, std::size_t slow_down_chance)
{
	multiple_lanes = true;
	create(length, lanes, car_density, max_speed, slow_down_chance);
}

void Car_System::calculate()
{
	// random machine
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<std::size_t> randomization(0, 99);

	// [[pos, lane], new_lane]
	std::vector< std::pair<std::pair<std::size_t, std::size_t>, std::size_t> > lane_changes;
	std::vector< std::vector<std::size_t> > reserved_pos(lanes);
	//std::vector< std::vector< std::pair<std::size_t, std::size_t> > > lane_changes(lanes);
	for(std::size_t lane = 0; lane < lanes; ++lane)
	{
		for(std::size_t pos = 0; pos < length; ++pos)
		{
			// skip empty cells
			if(!is_car_at(pos, lane))
				continue;

			// accelerate
			std::size_t speed = get_speed(pos, lane);
			if(speed < max_speed)
				++speed;

			// calculate distance to next car/barrier
			std::size_t gap = 0;
			while(gap++ < max_speed)
				if(is_car_at((pos + gap) % length, lane) || is_barrier_at((pos + gap) % length, lane))
					break;
			--gap;

			// if enabled, calc distances of other lanes
			if(multiple_lanes)
			{
				std::size_t gap_left = 0, gap_right = 0;
				if(lane > 0)
				{
					while(gap_left++ < max_speed)
						if(is_car_at((pos + gap_left) % length, lane - 1) || is_barrier_at((pos + gap_left) % length, lane - 1) || std::find(reserved_pos[lane - 1].begin(), reserved_pos[lane - 1].end(), pos + gap_left) != reserved_pos[lane - 1].end())
							break;
					--gap_left;

					std::size_t gap_back = 0;
					while(gap_back++ < max_speed)
					{
						std::size_t temp_pos = (gap_back <= pos) ? pos - gap_back : length - (gap_back - pos);
						if(is_car_at(temp_pos, lane - 1))
							break;
						else if(is_barrier_at(temp_pos, lane - 1))
						{
							gap_back = max_speed + 1;
							break;
						}
					}

					if(gap_back <= max_speed || is_car_at(pos, lane - 1) || is_barrier_at(pos, lane - 1) || std::find(reserved_pos[lane - 1].begin(), reserved_pos[lane - 1].end(), pos) != reserved_pos[lane - 1].end())
						gap_left = 0;
				}
				if(lane < lanes - 1)
				{
					while(gap_right++ < max_speed)
						if(is_car_at((pos + gap_right) % length, lane + 1) || is_barrier_at((pos + gap_right) % length, lane + 1) || std::find(reserved_pos[lane + 1].begin(), reserved_pos[lane + 1].end(), pos + gap_right) != reserved_pos[lane + 1].end())
							break;
					--gap_right;

					std::size_t gap_back = 0;
					while(gap_back++ < max_speed)
					{
						std::size_t temp_pos = (gap_back <= pos) ? pos - gap_back : length - (gap_back - pos);
						if(is_car_at(temp_pos, lane + 1))
							break;
						else if(is_barrier_at(temp_pos, lane + 1))
						{
							gap_back = max_speed + 1;
							break;
						}
					}

					if(gap_back <= max_speed || is_car_at(pos, lane + 1) || is_barrier_at(pos, lane + 1) || std::find(reserved_pos[lane + 1].begin(), reserved_pos[lane + 1].end(), pos) != reserved_pos[lane + 1].end())
						gap_right = 0;
				}

				if(gap < speed && /*gap_left > speed*/gap_left > gap && randomization(mt) < 100)
				{
					lane_changes.push_back({{pos, lane}, lane - 1});
					reserved_pos[lane - 1].push_back(pos);
					//lane_changes[lane - 1].push_back({pos, lane});
					if(gap_left < speed)
						speed = gap_left;
					gap = speed;
				}
				else if(gap < speed && /*gap_right > speed*/gap_right > gap && randomization(mt) < 100)
				{
					lane_changes.push_back({{pos, lane}, lane + 1});
					reserved_pos[lane + 1].push_back(pos);
					//lane_changes[lane + 1].push_back({pos, lane});
					if(gap_right < speed)
						speed = gap_right;
					gap = speed;
				}
			}


			if(gap < speed)
				speed = gap;
			// random slow down
			if(randomization(mt) < slow_down_chance && speed > 0)
				--speed;

			for(std::size_t i = 0; i <= speed; ++i)
				if(is_slow_down_at((pos + i) % length, lane) && speed > 1)
					--speed;

			set_car_speed(speed, pos, lane);
		}
	}

	for(const auto& a : lane_changes)
	{
		std::size_t speed = get_speed(a.first.first, a.first.second);
		remove_car(a.first.first, a.first.second);
		add_car(speed, a.first.first, a.second);
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

			std::size_t new_pos = (pos + get_speed(pos, lane)) % length;
			system[new_pos + lane * length] = get_speed(pos, lane) | ((is_slow_down_at(new_pos, lane)) ? SLOW_DOWN_BIT : 0x00);
			if(pos != new_pos)
				remove_car(pos, lane);
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
				system[pos + lane * length] = EMPTY;
		}
	}
}

void Car_System::reset()
{
	std::fill(system.begin(), system.end(), EMPTY);
}

void Car_System::add_car(std::size_t speed, std::size_t pos, std::size_t lane)
{
	system[pos + lane * length] = speed | ((is_slow_down_at(pos, lane)) ? SLOW_DOWN_BIT : 0x00);
}

void Car_System::remove_car(std::size_t pos, std::size_t lane)
{
	system[pos + lane * length] = EMPTY | ((is_slow_down_at(pos, lane)) ? SLOW_DOWN_BIT : 0x00);
}

void Car_System::set_car_speed(std::size_t speed, std::size_t pos, std::size_t lane)
{
	if(is_car_at(pos, lane))
	{
		system[pos + lane * length] = speed | ((is_slow_down_at(pos, lane)) ? SLOW_DOWN_BIT : 0x00);
	}
}

void Car_System::add_barrier(std::size_t pos, std::size_t lane)
{
	system[pos + lane * length] = BARRIER | ((is_slow_down_at(pos, lane)) ? SLOW_DOWN_BIT : 0x00);
}

void Car_System::remove_barrier(std::size_t pos, std::size_t lane)
{
	system[pos + lane * length] = EMPTY | ((is_slow_down_at(pos, lane)) ? SLOW_DOWN_BIT : 0x00);
}

void Car_System::add_slow_down(std::size_t pos, std::size_t lane)
{
	system[pos + lane * length] = set_slow_down_bit(system[pos + lane * length]);
}

void Car_System::remove_slow_down(std::size_t pos, std::size_t lane)
{
	system[pos + lane * length] = clear_slow_down_bit(system[pos + lane * length]);
}

double Car_System::get_avg_speed(std::size_t lane)
{
	auto sum_lane = [this](std::size_t lane, std::size_t& avg_speed, std::size_t& cars)
	{
		for(std::size_t pos = 0; pos < length; ++pos)
		{
			// skip empty cells
			if(!is_car_at(pos, lane))
				continue;

			avg_speed += system[pos + lane * length];
			++cars;
		}
	};
	std::size_t avg_speed = 0;
	std::size_t cars = 0;
	if(lane == std::numeric_limits<std::size_t>::max())
		for(lane = 0; lane < lanes; ++lane)
			sum_lane(lane, avg_speed, cars);
	else
		sum_lane(lane, avg_speed, cars);

	return (cars != 0) ? (static_cast<double>(avg_speed) / cars) : 0;
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
	std::size_t speed = clear_slow_down_bit(system[pos + lane * length]);
	return (speed != EMPTY && speed != BARRIER) ? speed : 0;
}

bool Car_System::is_car_at(std::size_t pos, std::size_t lane)
{
	if(clear_slow_down_bit(system[pos + lane * length]) != EMPTY && clear_slow_down_bit(system[pos + lane * length]) != BARRIER)
		return true;
	return false;
}

bool Car_System::is_slow_down_at(std::size_t pos, std::size_t lane)
{
	return system[pos + lane * length] & SLOW_DOWN_BIT;
}

bool Car_System::is_barrier_at(std::size_t pos, std::size_t lane)
{
	return (clear_slow_down_bit(system[pos + lane * length]) == BARRIER);
}
