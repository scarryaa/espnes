#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <cstdint>

class Controller
{
public:
	void write_controller_1(uint8_t value);
	uint8_t read_controller_1();
	uint8_t read_controller_2();

	private:
		uint8_t controller_1;
		uint8_t controller_2;
		uint8_t controller_1_state;
		uint8_t controller_2_state;
		uint8_t controller_1_state_index;
		uint8_t controller_2_state_index;
		uint8_t last_bus_value;

		uint8_t get_controller_1_state(uint8_t index);
		uint8_t get_controller_2_state(uint8_t index);
};

#endif