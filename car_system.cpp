#include "car_system.h"
#include <random>

Car_System::Car_System() : current_system(0), car_id(0)
{
	// init systems and set in first system car_ids
	for(std::size_t i = 0; i < config.get_street_lanes(); ++i)
	{
		std::vector<Car> temp_lane;
		temp_lane.resize(config.get_street_length());
		for(auto& a : temp_lane)
			a.id = car_id++;

		system[0].emplace_back(std::move(temp_lane));
		system[1].emplace_back(std::vector<Car>(config.get_street_length()));
	}

	new_random();

/*	system[0][0][0].speed = 5;
	system[0][0][5].speed = 4;
	system[0][0][9].speed = 2;
	system[0][0][13].speed = 1;
	system[0][0][15].speed = 1;*/
}

void Car_System::generate()
{
	// random machine
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<std::size_t> dist_slow_down(0, 99);

	// clear inactive system
	for(auto& a : system[current_system ^ 0x01])
		for(auto& b : a)
			b.speed = -1;

	for(std::size_t lane = 0; lane < config.get_street_lanes(); ++lane)
	{
		for(std::size_t pos = 0; pos < config.get_street_length(); ++pos)
		{
			signed char speed = system[current_system][lane][pos].speed;

			// determine hole to next car
			std::size_t pos_temp = pos, timeout = 0;
			while(system[current_system][lane][++pos_temp % config.get_street_length()].speed == -1
				  && timeout++ < config.get_street_length())
				;

			std::size_t hole = pos_temp - pos - 1;

			if(is_car_at(pos, lane))
			{
				if(speed < config.get_max_speed())
					++speed;
				if(static_cast<int>(hole) < speed)
					speed = hole;

				// random slow down car is driving (speed != 0)
				if(dist_slow_down(mt) < config.get_slow_down_chance() && speed)
					--speed;

				// set car in next system
				system[current_system ^ 0x01][lane][(pos + speed) % config.get_street_length()].speed = speed;
				system[current_system ^ 0x01][lane][(pos + speed) % config.get_street_length()].id = system[current_system][lane][pos].id;
			}
		}
	}

	// update system by toggling current_system
	current_system ^= 0x01;
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
		for(std::size_t pos = 0; pos < config.get_street_lanes(); ++pos)
		{
			std::size_t random = dist(mt);
			if(random < config.get_car_density())
				system[current_system][lane][pos].speed = random % (config.get_max_speed() + 1);
			else
				system[current_system][lane][pos].speed = -1;
		}
	}
}
