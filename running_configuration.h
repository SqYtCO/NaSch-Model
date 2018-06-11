#ifndef RUNNING_CONFIGURATION_H
#define RUNNING_CONFIGURATION_H

enum Tool
{
	Car_Tool,
	Speed_Tool,
	Barrier_Tool,
	Slow_Down_Tool
};

class Running_Configuration
{
public:
	Running_Configuration() : tool(Car_Tool) {	}

// options
private:
	Tool tool;

// set options
public:
	inline void set_tool(const Tool& new_tool) { tool = new_tool; }

// get options
public:
	inline const Tool& get_tool() const { return tool; }
};

#endif // RUNNING_CONFIGURATION_H
