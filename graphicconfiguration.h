#ifndef GRAPHICCONFIGURATION_H
#define GRAPHICCONFIGURATION_H

#include <cstddef>
#include <string>

// display language
enum Language
{
	System = 0,
	English,
	German
};

namespace Default_Values
{
	constexpr Language LANGUAGE = System;
	constexpr std::size_t CELL_SIZE = 100;
	constexpr bool DIAGRAM_MODE = false;
	constexpr bool SHOW_SPEED_COLOR = true;
	constexpr bool LONG_STREET_BREAK = true;
	constexpr bool SHOW_CARS = true;
	constexpr bool EXIT_WARNING = false;
	constexpr bool FULLSCREEN = false;
	constexpr bool SHOW_STARTUP_DIALOG = true;
	constexpr std::size_t DELAY = 15;
}

class GraphicConfiguration
{
	bool saved;

protected:
	static constexpr const char* CONFIG_FILE = ".gconfiguration";
	std::string config_path;

public:
	GraphicConfiguration();

	// config_path has to be empty or end on a separator; after setting config_path read config
	void set_config_path(const std::string& new_config_path) { config_path = new_config_path; read_config(); }

	bool write_config();
	bool read_config();
	void reset_config();

	const bool& is_saved() { return saved; }

// options
public:
	Language language;
	std::size_t cell_size;
	bool diagram_mode;
	bool show_speed_color;
	bool long_street_break;
	bool show_cars;
	bool exit_warning;
	bool fullscreen;
	bool show_startup_dialog;
	std::size_t delay;

// get options
public:
	inline const Language& get_language() const { return language; }
	inline std::size_t get_cell_size() const { return cell_size; }
	inline const bool& get_diagram_mode() const { return diagram_mode; }
	inline const bool& get_show_speed_color() const { return show_speed_color; }
	inline const bool& get_long_street_break() const { return long_street_break; }
	inline const bool& get_show_cars() const { return show_cars; }
	inline const bool& get_exit_warning() const { return exit_warning; }
	inline const bool& get_fullscreen() const { return fullscreen; }
	inline const bool& get_show_startup_dialog() const { return show_startup_dialog; }
	inline std::size_t get_delay() const { return delay; }

// set options
public:
	inline void set_language(const Language& new_language) { saved = false; language = new_language; }
	inline void set_cell_size(std::size_t new_cell_size) { saved = false; cell_size = new_cell_size; }
	inline void set_diagram_mode(const bool& new_diagram_mode) { saved = false; diagram_mode = new_diagram_mode; }
	inline void set_show_speed_color(const bool& new_show_speed_color) { saved = false; show_speed_color = new_show_speed_color; }
	inline void set_long_street_break(const bool& new_long_street_break) { saved = false; long_street_break = new_long_street_break; }
	inline void set_show_cars(const bool& new_show_cars) { saved = false; show_cars = new_show_cars; }
	inline void set_exit_warning(const bool& new_exit_warning) { saved = false; exit_warning = new_exit_warning; }
	inline void set_fullscreen(const bool& new_fullscreen) { saved = false; fullscreen = new_fullscreen; }
	inline void set_show_startup_dialog(const bool& new_show_startup_dialog) { saved = false; show_startup_dialog = new_show_startup_dialog; write_config(); }
	inline void set_delay(std::size_t new_delay) { saved = false; delay = new_delay; }
};

#endif // GRAPHICCONFIGURATION_H
