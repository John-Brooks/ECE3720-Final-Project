#include "pi_control.h"

void InitController(Controller_t* controller)
{
	controller->integral_term = 0.0f;
	controller->proportional_term = 0.0f;
	controller->set_point = 0.0f;
	controller->output = 0.0f;
}

void RunController(Controller_t* controller, float current_value, float delta_t)
{
	float error = controller->set_point - current_value;
	controller->proportional_term = error * controller->proportional_coeff;

	//note that we assume the error does not change significantly between calls, if 
	//we find that this function is called infrequently, or that the temperature 
	//changes faster, then this line should actually average the error since the last call.
	controller->integral_term += error * delta_t * controller->integral_coeff;

	controller->output = controller->proportional_term + controller->integral_term;
}
