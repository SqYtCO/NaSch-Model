#ifndef RUNNING_CONFIGURATION_H
#define RUNNING_CONFIGURATION_H

enum Tool
{
	Create_Car,
	Increase_Speed,
	Decrease_Speed,
	Create_Slow_Down,
	Create_Barrier,
	Delete
};

class Running_Configuration
{
public:
	Running_Configuration() = default;

// options
private:
	Tool selected_tool;

// set options
public:


// get options
public:

};

#endif // RUNNING_CONFIGURATION_H
