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

	slow_down.resize(config.get_street_lanes());

	new_random();
	system[current_system][1][4].speed = -2;
	system[current_system][2][4].speed = -2;
}
#include <qdebug.h>
#include <chrono>
void Car_System::generate()
{
	auto begin = std::chrono::high_resolution_clock::now();
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
			if(is_car_at(pos, lane))
			{
				std::size_t speed = system[current_system][lane][pos].speed;

				// determine gap to next car
				std::size_t pos_temp = pos, timeout = 0;
				while(timeout++ < config.get_max_speed()
					  && !is_car_at(++pos_temp % config.get_street_length(), lane)
					  && !is_barrier_at(pos_temp % config.get_street_length(), lane))
					;

				std::size_t gap = 0;
				if(timeout)
					 gap = timeout - 1;

				if(speed < config.get_max_speed())
					++speed;
				if(gap < speed)
					speed = gap;

				// random slow down car is driving (speed != 0)
				if(dist_slow_down(mt) < config.get_slow_down_chance() && speed)
					--speed;

				std::size_t speed_copy = speed;
				for(std::size_t a = 1; a < speed_copy && speed; ++a)
					if(is_slow_down_at(pos + a, lane))
						--speed;

				// set car in next system
				system[current_system ^ 0x01][lane][(pos + speed) % config.get_street_length()].speed = speed;
				system[current_system ^ 0x01][lane][(pos + speed) % config.get_street_length()].id = system[current_system][lane][pos].id;
			}
			else if(is_barrier_at(pos, lane))
				system[current_system ^ 0x01][lane][pos].speed = -2;
		}
	}

	for(std::size_t lane = 0; lane < config.get_street_lanes(); ++lane)
	{
		for(std::size_t pos = 0; pos < config.get_street_length(); ++pos)
		{
			if(is_car_at(pos, lane))
			{
				std::size_t speed = system[current_system][lane][pos].speed;
				std::size_t new_lane = lane;

				// determine gap to next car
				std::size_t pos_temp = pos, timeout = 0;
				while(timeout++ < config.get_max_speed()
					  && (system[current_system ^ 0x01][lane][++pos_temp % config.get_street_length()].speed == -1)
					  && (system[current_system ^ 0x01][lane][pos_temp % config.get_street_length()].speed != -3))
					;

				std::size_t gap = 0;
				if(timeout)
					 gap = timeout - 1;

				if(speed < config.get_max_speed())
					++speed;
				if(gap < speed)
				{
					std::size_t gap_left_lane = 0, gap_right_lane = 0;

					if(lane != 0 && config.get_street_lanes() > 1)
					{
						timeout = 0;
						while(timeout++ < config.get_max_speed()
							  && (system[current_system ^ 0x01][lane - 1][++pos_temp % config.get_street_length()].speed == -1)
							  && (system[current_system ^ 0x01][lane - 1][pos_temp % config.get_street_length()].speed != -3))
							;

						if(timeout > 1)
							gap_left_lane = timeout - 2;
					}
					if((lane + 1) < config.get_street_lanes())
					{
						timeout = 0;
						while(timeout++ < config.get_max_speed()
							  && (system[current_system ^ 0x01][lane + 1][++pos_temp % config.get_street_length()].speed == -1)
							  && (system[current_system ^ 0x01][lane + 1][pos_temp % config.get_street_length()].speed != -3))
							;

						if(timeout > 1)
							gap_right_lane = timeout - 2;
					}

					if(gap_left_lane > gap || gap_right_lane > gap)
					{
						if(gap_left_lane < gap_right_lane)
						{
							speed = gap_right_lane;
							++new_lane;
						}
						else
						{
							speed = gap_left_lane;
							--new_lane;
						}
					}
					else
						speed = gap;
				}

				// random slow down car is driving (speed != 0)
				if(dist_slow_down(mt) < config.get_slow_down_chance() && speed)
					--speed;

				std::size_t speed_copy = speed;
				for(std::size_t a = 1; a < speed_copy && speed; ++a)
					if(is_slow_down_at(pos + a, lane))
						--speed;

				if(new_lane != lane)
				{

					system[current_system ^ 0x01][new_lane][(pos + speed) % config.get_street_length()].speed = speed;
					system[current_system ^ 0x01][new_lane][(pos + speed) % config.get_street_length()].id = system[current_system][lane][pos].id;
				}

				// set car in next system
				system[current_system ^ 0x01][new_lane][(pos + speed) % config.get_street_length()].speed = speed;
				system[current_system ^ 0x01][new_lane][(pos + speed) % config.get_street_length()].id = system[current_system][lane][pos].id;
			}
			else if(is_barrier_at(pos, lane))
				system[current_system ^ 0x01][lane][pos].speed = -2;
		}
	}

	// update system by toggling current_system
	current_system ^= 0x01;

	auto end = std::chrono::high_resolution_clock::now();
	qDebug() << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
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
				system[current_system][lane][pos].speed = random % (config.get_max_speed() + 1);
			else
				system[current_system][lane][pos].speed = -1;
		}
	}
}
