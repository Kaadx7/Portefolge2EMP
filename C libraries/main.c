#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "emp_type.h"
#include "gpio.h"
#include "systick.h"
#include "swtimers.h"
#include "tmodel.h"
#include "systick.h"
#include "rtcs.h"
#include "SPI.h"
#include "uart.h"
#include "test.h"



int main(void)
/*****************************************************************************
*   Input    : NONE
*   Output   : Returns 0
*   Function : Init hardware and then loop forever
******************************************************************************/
{
  INT8U event;
  INT8U counter_value;

  disable_global_int();
  init_systick();
  init_gpio();
  enable_global_int();

  uart0_init( 9600, 8, 1, 0 );
  SPI_init();
  GPIO_PORTC_DATA_R |= (1<<7)|(1<<6)|(1<<5)|(1<<4); //Make sure all SS are high

  while( !uart0_tx_rdy() ){}
  uart0_putc('k');

  open_queue(Q_SPI_POS);
  open_queue(Q_SPI_PWM);

  start_task(TASK_SPI, SPI_task);
  start_task(TASK_TEST, test_task);

  schedule();


  return( 0 );
}
