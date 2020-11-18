#pragma once

typedef struct Controller_t {
	//last time (in seconds from start)
	float integral_term;
	float integral_coeff;
	float proportional_term;
	float proportional_coeff;

	float set_point;
	float output;
} Controller_t;

void InitController(Controller_t* controller);
void RunController(Controller_t* controller, float current_value, float delta_t);