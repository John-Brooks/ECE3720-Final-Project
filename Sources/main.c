#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "pi_control.h"

void main(void) {
	Controller_t controller;
	InitController(&controller);
	


	EnableInterrupts;


	for(;;) {
		_FEED_COP(); /* feeds the dog */
	} /* loop forever */
/* please make sure that you never leave main */
}
