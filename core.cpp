#include "core.h"

Core::Core()
{

}

Core* Core::get_instance()
{
	static Core core;

	return &core;
}
