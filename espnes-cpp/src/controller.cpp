#include "../include/controller.hpp"

void Controller::write_controller_1(uint8_t value)
{
	// $4016
	if (value & 0x01)
	{
		controller_1_state_index = 0;
	}
}

uint8_t Controller::read_controller_1()
{
	uint8_t button_state = 0;
	last_bus_value = 0x40;

	if (controller_1_state_index < 8)
	{
		button_state = get_controller_1_state(controller_1_state_index);
		controller_1_state_index++;
	}
	else
	{
		// Return the upper bits of the last bus value
		button_state = (last_bus_value & 0xFE) | 1;
	}

	// Update the last bus value
	last_bus_value = button_state;

	// TODO implement more complex controller logic
	return 0x40;
}

uint8_t Controller::read_controller_2()
{
	uint8_t button_state = 0;

	if (controller_2_state_index < 8)
	{
		button_state = get_controller_2_state(controller_2_state_index);
		controller_2_state_index++;
	}
	else
	{
		// Return the upper bits of the last bus value
		button_state = (last_bus_value & 0xFE);
	}

	// Update the last bus value
	last_bus_value = button_state;

	// TODO implement more complex controller logic
	return 0x40;
}

uint8_t Controller::get_controller_1_state(uint8_t index)
{
	uint8_t state = 0;

	switch (index)
	{
	case 0:
		state = controller_1 & 0x01;
		break;
	case 1:
		state = (controller_1 & 0x02) >> 1;
		break;
	case 2:
		state = (controller_1 & 0x04) >> 2;
		break;
	case 3:
		state = (controller_1 & 0x08) >> 3;
		break;
	case 4:
		state = (controller_1 & 0x10) >> 4;
		break;
	case 5:
		state = (controller_1 & 0x20) >> 5;
		break;
	case 6:
		state = (controller_1 & 0x40) >> 6;
		break;
	case 7:
		state = (controller_1 & 0x80) >> 7;
		break;
	}

	return state;
}

uint8_t Controller::get_controller_2_state(uint8_t index)
{
	uint8_t state = 0;

	switch (index)
	{
	case 0:
		state = controller_2 & 0x01;
		break;
	case 1:
		state = (controller_2 & 0x02) >> 1;
		break;
	case 2:
		state = (controller_2 & 0x04) >> 2;
		break;
	case 3:
		state = (controller_2 & 0x08) >> 3;
		break;
	case 4:
		state = (controller_2 & 0x10) >> 4;
		break;
	case 5:
		state = (controller_2 & 0x20) >> 5;
		break;
	case 6:
		state = (controller_2 & 0x40) >> 6;
		break;
	case 7:
		state = (controller_2 & 0x80) >> 7;
		break;
	}

	return state;
}