#include "pi_control.h"

void InitController(Controller_t* controller)
{
	//If the temperature has been 10 degrees hotter than desired 
	//for 10 seconds the accumulated error will be 100 "degree seconds"
	//in this situation I would expect maybe an additional 5% fan duty cycle?
	//Decreasing this coeff will make the controller less prone to overshoot.
	controller->integral_coeff = 0.05 / 100; //5% per 100 degree seconds

	//For every degree C scale fan speed by X percent.
	controller->proportional_term = 5.0;

	//maintain temperature of 40C
	controller->set_point = 40;

	controller->output = 0.0f;
}

void RunController(Controller_t* controller, float current_value, float delta_t)
{
	float error = current_value - controller->set_point;

	controller->proportional_term = error * controller->proportional_coeff;
	
	//note that we assume the error does not change significantly between calls, if 
	//we find that this function is called infrequently, or that the temperature 
	//changes faster, then this line should actually average the error since the last call.
	controller->integral_term += error * delta_t * controller->integral_coeff;

	controller->output = controller->proportional_term + controller->integral_term;

	if (controller->output < 0)
		controller->output = 0;
	else if (controller->output > 1)
		controller->output = 1;
}
