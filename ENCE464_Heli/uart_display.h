/*
 * Written by (Shun Lyu && Ke Gao && Yat Chung LEUNG
 * Date:15/5/2018
 * Use to send string to serial port:PORTA0
 * Example:
        int
        main(void)
        {
            int8_t upPushes = 0, downPushes = 0;
            uint8_t butState;

            initClock ();
            initDisplay ();
            initButtons ();
            initialiseUSB_UART ();
            initSysTick ();

            OLEDStringDraw ("UART demo", 0, 0);
            displayButtonState ("UP  ", "RELS", upPushes, 2);
            displayButtonState ("DOWN", "RELS", downPushes, 3);

            while(1)
            {
                // check state of each button and display if a change is detected
                butState = checkButton (UP);
                switch (butState)
                {
                case PUSHED:
                    displayButtonState ("UP  ", "PUSH", ++upPushes, 2);
                    break;
                case RELEASED:
                    displayButtonState ("UP  ", "RELS", upPushes, 2);
                    break;
                // Do nothing if state is NO_CHANGE
                }
                butState = checkButton (DOWN);
                switch (butState)
                {
                case PUSHED:
                    displayButtonState ("DOWN", "PUSH", ++downPushes, 3);
                    break;
                case RELEASED:
                    displayButtonState ("DOWN", "RELS", downPushes, 3);
                    break;
                // Do nothing if state is NO_CHANGE
                }

                // Is it time to send a message?
                if (slowTick)
                {
                    slowTick = false;
                    // Form and send a status message to the console
                    sprintf (statusStr, "UP=%2d DN=%2d | ", upPushes, downPushes);
                    UARTSend (statusStr);
                }
            }
        }
*/

#ifndef UART_DISPLAY_H_
#define UART_DISPLAY_H_

#include "main.h"


//********************************************************
// Constants
//********************************************************
#define SYSTICK_RATE_HZ 100
#define SLOWTICK_RATE_HZ 4
#define MAX_STR_LEN 16
//---USB Serial comms: UART0, Rx:PA0 , Tx:PA1
#define BAUD_RATE 9600
#define UART_USB_BASE           UART0_BASE
#define UART_USB_PERIPH_UART    SYSCTL_PERIPH_UART0
#define UART_USB_PERIPH_GPIO    SYSCTL_PERIPH_GPIOA
#define UART_USB_GPIO_BASE      GPIO_PORTA_BASE
#define UART_USB_GPIO_PIN_RX    GPIO_PIN_0
#define UART_USB_GPIO_PIN_TX    GPIO_PIN_1
#define UART_USB_GPIO_PINS      UART_USB_GPIO_PIN_RX | UART_USB_GPIO_PIN_TX

void initialiseUSB_UART (void);
void UARTSend (char *pucBuffer);


#endif /* UART_DISPLAY_H_ */
