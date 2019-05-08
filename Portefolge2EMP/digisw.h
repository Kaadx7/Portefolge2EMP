
#ifndef DIGISW_H_
#define DIGISW_H_

/***************************** Include files *******************************/
#include "defines.h"
#include "setup.h"


/*****************************    Defines    *******************************/
// Digiswitch States.
// ------------------
#define DSS_IDLE    0
#define DSS_A_OFF   1
#define DSS_A_ON    2



/*****************************   Constants   *******************************/

/*****************************   Functions   *******************************/


void digiSwitch_task(void *pvParameters);

/****************************** End Of Module *******************************/




#endif /* DIGISW_H_ */
