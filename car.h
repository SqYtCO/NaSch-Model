#ifndef CAR_H
#define CAR_H

#include <cstddef>

struct Car
{
	std::size_t id = 0;
	std::size_t speed = 0;
	std::size_t pos;
	std::size_t lane;
};

#endif // CAR_H
