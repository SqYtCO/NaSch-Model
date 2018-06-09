#ifndef CORE_H
#define CORE_H

#include "configuration.h"
#include "running_configuration.h"

class Core
{
	Configuration config;
	Running_Configuration run_config;

	// private constructors and assignment operator
	Core();
	Core(const Core&);
	Core& operator=(const Core&);

public:
	// singleton pattern - only one core per application; get this single instance by calling Core::get_instance()
	static Core* get_instance();

	Configuration* get_config() { return &config; }
	Running_Configuration* get_run_config() { return &run_config; }
};

#endif // CORE_H
