/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* MODULENAME.: emp.c
*
* PROJECT....: EMP
*
* DESCRIPTION: See module specification file (.h-file).
*
* Change Log:
 ****************************************************************************
 Date    Id    Change
* YYMMDD
* --------------------
* 050128  KA    Module created.
*
*****************************************************************************/

/***************************** Include files *******************************/
#include "SPI.h"
#include "EMP_type.h"
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "defines.h"
#include "rtcs.h"
#include "uart.h"
/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/
extern volatile INT16S pos_var;
extern volatile INT16S pwm_var;

float control_1_pos;
float control_1_vel;
float control_1_cur;
float control_2_pos;
float control_2_vel;
float control_2_cur;

SemaphoreHandle_t POS_1_SEM;
SemaphoreHandle_t VEL_1_SEM;
SemaphoreHandle_t CUR_1_SEM;
SemaphoreHandle_t POS_2_SEM;
SemaphoreHandle_t VEL_2_SEM;
SemaphoreHandle_t CUR_2_SEM;

SemaphoreHandle_t POS_1_REF_SEM;
SemaphoreHandle_t VEL_1_REF_SEM;
SemaphoreHandle_t CUR_1_REF_SEM;
SemaphoreHandle_t POS_2_REF_SEM;
SemaphoreHandle_t VEL_2_REF_SEM;
SemaphoreHandle_t CUR_2_REF_SEM;

SemaphoreHandle_t QUEUE_SEM;
/*****************************   Functions   *******************************/
void SPI_init()
/*****************************************************************************
*   Input    :
*   Output   :
*   Function :Set up the SPI on the microcontroller.
*   Page     : 965
*   *   - Setups the microcontroller as Master.
*   - Uses Port A 5 as MOSI
*   - uses Port A 4 as MISO
*   - Uses Port A 3 as SS (slave select)
*   - Uses Port A 2 as SCK
*   - Uses SSIO0
*
*   Sends 8 bit data - with 250kHz
******************************************************************************/
{
    //To enable and initialize the SSI, the following steps are necessary:

    //Enable the SSI module using theRCGCSSIregister (see page 346)
    SYSCTL_RCGCSSI_R |= (1<<1);        //selecting SSI1 module

    //Enable the clock to the appropriate GPIO module via theRCGCGPIOregister
    //(see page 340).To find out which GPIO port to enable, refer to Table 23-5 on page 1351.
    SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOD; //Maybe the right register
    SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOF; //Maybe the right register

    //Set the GPIOAFSELbits for the appropriate pins (see page 671).
    //To determine which GPIOs to configure, see Table 23-4 on page 1344

    GPIO_PORTD_AFSEL_R |= (1<<0)|(1<<1)|(1<<2)|(1<<3);
    GPIO_PORTF_AFSEL_R |= (1<<0)|(1<<1)|(1<<2)|(1<<3);

    // Configure thePMCnfields in theGPIOPCTLregister to assign the SSI
    // signals to the appropriate pins. See page 688 and Table 23-5 on page 135

    GPIO_PORTD_PCTL_R |= (2<<0)|(2<<4)|(2<<8)|(2<12);
    GPIO_PORTF_PCTL_R |= (2<<0)|(2<<4)|(2<<8)|(2<12);

    //Program theGPIODENregister to enable the pin's digital function.
    // In addition, the drive strength,drain select and pull-up/pull-down functions must be configured.
    //Refer to “General-PurposeInput/Outputs (GPIOs)” on page 649 for more information.

    GPIO_PORTD_DEN_R |=(1<<1)|(1<<2); //enabling digital mode for PORTD 0,1,2,3

    GPIO_PORTD_PUR_R |=(1<<1)|(1<<2); //selecting pull ups for 0,1,2,3

    GPIO_PORTF_DEN_R |=(1<<0)|(1<<1)|(1<<2); //enabling digital mode for PORTD 0,1,2,3

    GPIO_PORTF_PUR_R |=(1<<0)|(1<<1)|(1<<2); //selecting pull ups for 0,1,2,3

    //Ensure that the SSE bit in the SSICR1 register is clear before making any configuration changes.

    SSI1_CR1_R = 0 ;

    // Select whether the SSI is a master or slave
    // For master operations, set the SSICR1 register to 0x0000.0000.

    SSI1_CR1_R = 0 ;

    //Configure the SSI clock source by writing to the SSICC register.

    SSI1_CC_R = 0; //using main system clock

    // Configure the clock prescale divisor by writing the SSICPSR register.

    SSI1_CPSR_R = 6; //selecting divisor 6 for SSI clock
                      // SSInClk = SysClk / (CPSDVSR * (1 + SCR) SCR = 0;

    //Write theSSICR0register with the following configuration
    SSI1_CR0_R |= 0xF|(1<<7); //freescale mode, 16 bit data, steady clock low

    // Enable the SSI by setting theSSEbit in theSSICR1register.
    SSI1_CR1_R |= (1<<1);//|(1<<4); //Set EOT (4),SSI_CR1_EOT, in order to use the end of transmission interrupt

    // Enable interrupt on end of transmission
    //SSI1_IM_R |= (1 << 3); // maybe this can be used, SSI_IM_TXIM,

    //Make sure all SS are high
    GPIO_PORTC_DATA_R |= (1<<7)|(1<<6)|(1<<5)|(1<<4);
}



extern void SPI_task(void * pvParameters)
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : -
******************************************************************************/
{
        uint8_t received_id = 0;
        int16_t received_data_queue = 0;
        float received_data_SPI = 0;
        int16_t dummyReceive;

        struct SPI_queue_element received_struct;
        for( ;; )
        {
            if( xQueueReceive( SPI_queue, &received_struct, portMAX_DELAY) == pdPASS )
            {
                //Get id and data from SPI_queue
                received_id = received_struct.id;
                received_data_queue = received_struct.data;

                // Send data to slave
                send_data( received_data_queue, received_id );

                // Receive data from slave and put into correct buffer
                switch( received_id )
                {
                case POS_1:
                    received_data_SPI = receive_data();

                    //Convert to degrees and put in correct buffer
                    control_1_pos = received_data_SPI / 3;

                    // Signal Semaphore
                    xSemaphoreGive( POS_1_SEM );
                    break;

                case VEL_1:
                    received_data_SPI = receive_data();

                    //Put in correct buffer
                    control_1_vel = received_data_SPI;

                    // Signal Semaphore
                    xSemaphoreGive( VEL_1_SEM );
                    break;

                case POS_2:
                    received_data_SPI = receive_data();

                    //Convert to degrees and put in correct buffer
                    control_2_pos = received_data_SPI / 3;

                    // Signal Semaphore
                    xSemaphoreGive( POS_2_SEM );
                    break;

                case VEL_2:
                    received_data_SPI = receive_data();

                    //Put in correct buffer
                    control_2_vel = received_data_SPI;

                    // Signal Semaphore
                    xSemaphoreGive( VEL_2_SEM );
                    break;

                case PROTOCOL_SLAVE:
                    received_data_SPI = receive_data();
                    //Semaphore
                        //Put in correct buffer
                    break;

                // Do dummy receive to empty SPI buffer
                default:
                    dummyReceive = receive_data();
                }
            }
        }
}

extern void update_values_task(void * pvParameters)
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : -
******************************************************************************/
{
    for( ;; )
    {
        struct SPI_queue_element SPI_struct_send;

        SPI_struct_send.id = PWM_1;

        SPI_struct_send.data = 0xFFFF;

        xQueueSend( SPI_queue, (void * ) &SPI_struct_send, 0);

        vTaskDelay(100);
    }


}

void data_transmit(INT16U data)
/*****************************************************************************
*   Input    :Byte that
*   Output   :
*   Function :Sends the data given and waits until the transmission is complete.
******************************************************************************/
{
    SSI1_DR_R = data;          //putting the byte to send from SSI

    for(int i=0; i<20; i++){}; //Burn few cycles to make sure SS is LOW
    while ( !(GPIO_PORTD_DATA_R & 0b0000010) ) //While SS HIGH make sure slave is also HIGH
    {       ;      }

}

void send_data(uint16_t data, uint8_t slave_no)
/*****************************************************************************
*   Input    :byte that is being sent by SPI
*             slave select
*   Output   :
*   Function :Sets up slave select for desired slave, and then sends data given.
******************************************************************************/
{
    switch( slave_no )
    {
    case 0:
        GPIO_PORTC_DATA_R |= (1<<7)|(1<<6)|(1<<5)|(1<<4);
        data_transmit(data);
        GPIO_PORTC_DATA_R |= (1<<7)|(1<<6)|(1<<5)|(1<<4);
        break;
    case 1:
        GPIO_PORTC_DATA_R &= (1<<7)|(1<<6)|(1<<5)|(0<<4);
        data_transmit(data);
        GPIO_PORTC_DATA_R |= (1<<7)|(1<<6)|(1<<5)|(1<<4);
        break;
    case 2:
        GPIO_PORTC_DATA_R &= (1<<7)|(1<<6)|(0<<5)|(1<<4);
        data_transmit(data);
        GPIO_PORTC_DATA_R |= (1<<7)|(1<<6)|(1<<5)|(1<<4);
        break;
    case 3:
        GPIO_PORTC_DATA_R &= (1<<7)|(1<<6)|(0<<5)|(0<<4);
        data_transmit(data);
        GPIO_PORTC_DATA_R |= (1<<7)|(1<<6)|(1<<5)|(1<<4);
        break;
    case 4:
        GPIO_PORTC_DATA_R &= (1<<7)|(0<<6)|(1<<5)|(1<<4);
        data_transmit(data);
        GPIO_PORTC_DATA_R |= (1<<7)|(1<<6)|(1<<5)|(1<<4);
        break;
    case 5:
        GPIO_PORTC_DATA_R &= (1<<7)|(0<<6)|(1<<5)|(0<<4);
        data_transmit(data);
        GPIO_PORTC_DATA_R |= (1<<7)|(1<<6)|(1<<5)|(1<<4);
        break;
    case 6:
        GPIO_PORTC_DATA_R &= (1<<7)|(0<<6)|(0<<5)|(1<<4);
        data_transmit(data);
        GPIO_PORTC_DATA_R |= (1<<7)|(1<<6)|(1<<5)|(1<<4);
        break;
    case 7:
        GPIO_PORTC_DATA_R &= (1<<7)|(0<<6)|(0<<5)|(0<<4);
        data_transmit(data);
        GPIO_PORTC_DATA_R |= (1<<7)|(1<<6)|(1<<5)|(1<<4);
        break;
    case 8:
        GPIO_PORTC_DATA_R &= (0<<7)|(1<<6)|(1<<5)|(1<<4);
        data_transmit(data);
        GPIO_PORTC_DATA_R |= (1<<7)|(1<<6)|(1<<5)|(1<<4);
        break;
    case 9:
        GPIO_PORTC_DATA_R &= (0<<7)|(1<<6)|(1<<5)|(0<<4);
        data_transmit(data);
        GPIO_PORTC_DATA_R |= (1<<7)|(1<<6)|(1<<5)|(1<<4);
        break;
    }
}

int16_t receive_data()
/*****************************************************************************
*   Input    :
*   Output   : The function return the received data.
*   Function : receiving data with SPI
******************************************************************************/
{
    INT16S data;

    // is this the right way to do it? It probably is fine if the code is preemptive.
        // is it possible to somehow wait for a signal or semaphore or interrupt instead?
        // using vTaskNotifyGiveFromISR() and an ISR, the above mentioned should be achievable
        // it seems that it is only possible to interrupt on FIFO half full. Instead it might be enough to make sure
        // that the last bit has been sent
        // it seems that it might be possible to use the end of transmission (EOT) combined with the SSI Transmit FIFO Interrupt
        // to achieve the desired behaviour
    while( !(SSI1_SR_R & (0b00000010)) ) //Check if receive FIFO empty
    {}
    data = SSI1_DR_R;

    //Til uart test
//            INT8U data_HIGH = data & 0xFF;
//            INT8U data_LOW = (data >> 8);
//            while( !uart0_tx_rdy() )
//            {}
//            uart0_putc(data_LOW);
//            while( !uart0_tx_rdy() )
//            {}
//            uart0_putc(data_HIGH);

    return data;
}

int16_t receive_data_ISR()
/*****************************************************************************
*   Input    :
*   Output   : The function return the received data.
*   Function : receiving data with SPI
******************************************************************************/
{
    INT16S data;

    // is this the right way to do it? It probably is fine if the code is preemptive.
        // is it possible to somehow wait for a signal or semaphore or interrupt instead?
        // using vTaskNotifyGiveFromISR() and an ISR, the above mentioned should be achievable
        // it seems that it is only possible to interrupt on FIFO half full. Instead it might be enough to make sure
        // that the last bit has been sent
        // it seems that it might be possible to use the end of transmission (EOT) combined with the SSI Transmit FIFO Interrupt
        // to achieve the desired behaviour

    if(xSemaphoreTake(SPI_EOT_SEM, portMAX_DELAY)==pdTRUE)
    {
        data = SSI1_DR_R;

    }



    //Til uart test
//            INT8U data_HIGH = data & 0xFF;
//            INT8U data_LOW = (data >> 8);
//            while( !uart0_tx_rdy() )
//            {}
//            uart0_putc(data_LOW);
//            while( !uart0_tx_rdy() )
//            {}
//            uart0_putc(data_HIGH);

    return data;
}

void ssi1_eot_ISR()
{// look into page 956

    BaseType_t  xHigherPriorityTaskWoken = pdFALSE;
    if (SSI1_RIS_R & SSI_RIS_TXRIS)
    {
        xSemaphoreGiveFromISR(SPI_EOT_SEM, &xHigherPriorityTaskWoken);
    }

    //maybe clear an interrupt flag
}

/****************************** End Of Module *******************************/
