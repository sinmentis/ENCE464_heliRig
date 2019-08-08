/*
 * Written by (Shun Lyu && Ke Gao && Yat Chung LEUNG
 * Date:15/5/2018
 * Main Function
 */
#include "Read_heli_height.h"

void ADCIntHandler(void)
{
    uint32_t ulValue;
    ADCSequenceDataGet(ADC0_BASE, 3, &ulValue); //read from adc
    writeCircBuf (&g_inBuffer, ulValue);
    ADCIntClear(ADC0_BASE, 3);
}

void initADC (void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);
    ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_CH9 | ADC_CTL_IE |
                             ADC_CTL_END);
    ADCSequenceEnable(ADC0_BASE, 3);
    ADCIntRegister (ADC0_BASE, 3, ADCIntHandler);
    ADCIntEnable(ADC0_BASE, 3);
}

void YawIntHandler(void)
{
    phase_A = GPIOPinRead(GPIO_PORTB_BASE, GPIO_INT_PIN_0);
    phase_B = GPIOPinRead(GPIO_PORTB_BASE, GPIO_INT_PIN_1);
    if((last_state_A == 0 && last_state_B == 0) && (phase_A == 1 && phase_B == 0)){yaw_count--;}
    else if((last_state_A == 0 && last_state_B == 0) && (phase_A == 0 && phase_B == 1)){yaw_count++;}

    else if((last_state_A == 0 && last_state_B == 1) && (phase_A == 0 && phase_B == 0)){yaw_count--;}
    else if((last_state_A == 0 && last_state_B == 1) && (phase_A == 1 && phase_B == 1)){yaw_count++;}

    else if((last_state_A == 1 && last_state_B == 0) && (phase_A == 0 && phase_B == 0)){yaw_count++;}
    else if((last_state_A == 1 && last_state_B == 0) && (phase_A == 1 && phase_B == 1)){yaw_count--;}

    else if((last_state_A == 1 && last_state_B == 1) && (phase_A == 1 && phase_B == 0)){yaw_count++;}
    else if((last_state_A == 1 && last_state_B == 1) && (phase_A == 0 && phase_B == 1)){yaw_count--;}
    last_state_A = phase_A;
    last_state_B = phase_B;
    GPIOIntClear(GPIO_PORTB_BASE,GPIO_INT_PIN_0 | GPIO_INT_PIN_1);
}

void SWIntHandler(void)
{
    if(sys_init_flag == 0)                                                            // if the system was not initialized
    {
        while(GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_7) == GPIO_PIN_7){initSwitch();}  // the SW_1 was blocked and the system stops processing
    }
    if(GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_7) == GPIO_PIN_7){SW_on_flag = 1;}       // toggle the state of the switch
    else {SW_on_flag = 0;}
    GPIOIntClear(GPIO_PORTA_BASE, GPIO_INT_PIN_7);
}

/*
 * Only called once when calibration
 */
void PhaseCIntHandler(void)
{
    phase_C = 1; // Read yaw ref
    yaw_count = 0; // set the reference point to 0
    GPIOIntClear(GPIO_PORTC_BASE, GPIO_INT_PIN_4);
}


void initGPIO(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);

    GPIOPinTypeGPIOInput(GPIO_PORTA_BASE, GPIO_PIN_7);
    GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    GPIOPinTypeGPIOInput(GPIO_PORTC_BASE, GPIO_PIN_4);

    GPIOIntRegister(GPIO_PORTB_BASE, YawIntHandler);
    GPIOIntRegister(GPIO_PORTA_BASE, SWIntHandler);
    GPIOIntRegister(GPIO_PORTC_BASE, PhaseCIntHandler);

    GPIOIntTypeSet(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1, GPIO_BOTH_EDGES);
    GPIOIntTypeSet(GPIO_PORTA_BASE, GPIO_PIN_7, GPIO_BOTH_EDGES);
    GPIOIntTypeSet(GPIO_PORTC_BASE, GPIO_PIN_4, GPIO_BOTH_EDGES);

    GPIOIntEnable(GPIO_PORTB_BASE, GPIO_INT_PIN_0 | GPIO_INT_PIN_1);
    GPIOIntEnable(GPIO_PORTA_BASE, GPIO_INT_PIN_7);
    GPIOIntEnable(GPIO_PORTC_BASE, GPIO_INT_PIN_4);
}

/*
 * Input destination, Keep calling PID until 1000 sample yaw are all with in range(-5,5)
 */
void go_yaw(int32_t destination){
    int32_t yaw_sum = 0;
    while (yaw_sum <= 1000) {                     // if the helicopter was not in the destination for 1000 times
        YawControl(yaw_count, destination);       // turn the helicopter to the destnation
        SysCtlDelay (SysCtlClockGet() / 1600);
        if (abs(yaw_count-destination) <= 5)      // if the helicoptor was in range
        {
            yaw_sum += 1;                         // count once
        } else {
            yaw_sum = 0;                          // move out of the range, reset the counter
        }
    }
}

/*
 * Before calibration settle down, keep printing "calibrating..."
 * Flying one direction until find phase_c == 1
 * reset the reference yaw, and set up destination into 0
 * Return 1 if calibrating is down, otherwise return 0
 */

int calibration()
{
    OLEDStringDraw ("                ", 0, 0);
    OLEDStringDraw ("Calibrating...  ", 0, 1);
    OLEDStringDraw ("                ", 0, 2);
    OLEDStringDraw ("                ", 0, 3);
    UARTSend("calibrating...\n\r");
    setPWM(20);                                      // Flying one direction slowly to find the reference point
    setPWM2(4);
    if(phase_C == 1) {                               // if the reference point was reached
        GPIOIntDisable(GPIO_PORTC_BASE, GPIO_PIN_4); // block the intrrupt on that pin
        go_yaw(dest_yaw);                            // turn the face forward
        return 1;                                    // calibration finished
    }
    return 0;                                        // calibration not finished
}

/*
 *read data from buffer and calcualte the mean for ADC
 */
int32_t get_mean_ADC()
{
    int32_t sum;
    uint16_t i;

    sum = 0;
    for (i = 0; i < BUF_SIZE; i++)
    {
        sum = sum + readCircBuf (&g_inBuffer);
    }
    return (2 * sum + BUF_SIZE) / 2 / BUF_SIZE;
}

/*
 * landing program.
 * Calculate difference between current and destination position.
 * lower the height step by step until the error smaller than 25
 * then adjust the yaw.
 * After landed, disable 2 PWM and enable GPIO port:A7
 */
int land(int land_flag)
{
    if(!land_flag)  // if not on the ground
    {
        GPIOIntDisable(GPIO_PORTA_BASE, GPIO_INT_PIN_7);       //disable the SW_1
        dest_height = base_line - Heli_Height_StepSize;        //set the destnation right above the base line
        if(abs(dest_height - mean_ADC)>25 && calibration_flag) // if not arrive the destnation and already taken off
        {
            UARTSend("landing...\n\r");                        // say "I'm landing"
            dest_yaw = 0;                                      // set the destnation of yaw to 0
            HPIDcontrol(mean_ADC, dest_height);                // landing
            go_yaw(0);                                         // turning forward
            OLEDStringDraw ("landed....      ", 0, 1);
            OLEDStringDraw ("                ", 0, 0);
            OLEDStringDraw ("                ", 0, 2);
            OLEDStringDraw ("                ", 0, 3);
            return 0;                                          // landing not finished
        }
        else                                                   // if arrived the destnation
        {
            SysCtlDelay (SysCtlClockGet());                    // hold for 1 sec
            setPWM(0);                                         // turn of motors
            setPWM2(0);
            UARTSend("landed!!\n\r");
            GPIOIntEnable(GPIO_PORTA_BASE, GPIO_INT_PIN_7);    // enable SW_1
            return 1;                                          // landed
        }
    }
    else
    { return 1;}                                               // keep in the landed state

}

/*
 * Print out height & yaw reading and
 *  PWM output duty of main motor and
 *   tail motor
 */
void userInterfaceUpdate()
{
    char string1[17];

    height_percen = (int32_t) (base_line-mean_ADC)*100/(base_line-1230); // 3300/8=412.5
    yaw_degree = yaw_count * 360 / 448;
    displayUpdate ("Per: ", height_percen, "%", 0);
    displayUpdate ("Deg:",yaw_degree,"", 1);
    displayUpdate ("Duty main:", PWMPulseWidthGet(PWM_MAIN_BASE, PWM_MAIN_OUTNUM)/PWMGenPeriodGet(PWM_MAIN_BASE, PWM_MAIN_OUTNUM)*100,"%", 2);
    displayUpdate ("Duty tail:", PWMPulseWidthGet(PWM_SEC_BASE, PWM_SEC_OUTNUM)/PWMGenPeriodGet(PWM_SEC_BASE, PWM_SEC_OUTNUM)*100,"%", 3);
    sprintf(string1, "Height:%d\n\r", height_percen);
    UARTSend(string1);
    sprintf(string1, "Yaw : %d\n\r", yaw_degree);
    UARTSend(string1);
    sprintf(string1, "Main: %d\n\r",  PWMPulseWidthGet(PWM_MAIN_BASE, PWM_MAIN_OUTNUM)/PWMGenPeriodGet(PWM_MAIN_BASE, PWM_MAIN_OUTNUM)*100);
    UARTSend(string1);
    sprintf(string1, "Tail: %d\n\r", PWMPulseWidthGet(PWM_SEC_BASE, PWM_SEC_OUTNUM)/PWMGenPeriodGet(PWM_SEC_BASE, PWM_SEC_OUTNUM)*100);
    UARTSend(string1);
}

/*
 * Check button state
 */
void check_button(){
    if ((checkButton (DOWN) == PUSHED) && dest_height < base_line)    //check the state of down button
    { dest_height += Heli_Height_StepSize; }                          //change the destnation of height of the helicopter

    if ((checkButton (UP) == PUSHED) && dest_height > Heli_Height_MAX) //check the state of up button
    { dest_height -= Heli_Height_StepSize; }                           //change the destnation height of the helicopter

    if ((checkButton (RIGHT) == PUSHED))                               //check the state of right button
    { dest_yaw += Heli_Yaw_StepSize; }                                 //change the destnation yaw of the helicopter

    if ((checkButton (LEFT) == PUSHED))                                //check the state of left button
    { dest_yaw -= Heli_Yaw_StepSize; }                                 //change the destnation yaw of the helicopter
}


int main(void)
{
    //--------------------------- set up --------- run once -------------------------------
    initClock ();
    SysCtlDelay(100); //Crude delay to make sure hardware things are settled before proceeding
    //OLEDInitialise();
    initADC ();
    initButtons ();
    initCircBuf (&g_inBuffer, BUF_SIZE);
    initGPIO();
    last_state_A = GPIOPinRead(GPIO_PORTB_BASE, GPIO_INT_PIN_0);
    last_state_B = GPIOPinRead(GPIO_PORTB_BASE, GPIO_INT_PIN_1);
    initialiseUSB_UART();
    initSwitch();
    initPWM();
    IntMasterEnable();
    //----------------- Check mode button and Initial helicopter --------------------
    PWMOutputState(PWM_MAIN_BASE, PWM_MAIN_OUTBIT, true); // enable PWM outputs
    PWMOutputState(PWM_SEC_BASE, PWM_SEC_OUTBIT, true);
    sys_init_flag = 1;                                    // the system was initialized, SW_1 was then enabled
    //---------------------------------- loop ---------------------------------------
    while (1)
    {
        if(SW_on_flag == 1)                                // if switch was on
        {
            land_flag = 0;                                 // then it means the helicopter should not on the ground

            while(!calibration_flag && SW_on_flag == 1) {  // if the calibration was not done and the helicopter should fly
                calibration_flag = calibration();          // then do the calibration
                mean_ADC = get_mean_ADC();                 // and updating the height
            }

            base_line = mean_ADC;                          // the base line was settled automatically after the calibration
            dest_height = base_line - Heli_Height_StepSize;// then take flight
            while(SW_on_flag == 1)                         // if the helicopter should fly
            {
                mean_ADC = get_mean_ADC();                 // update the height of the helicopter
                check_button();                            // check any button push
                HPIDcontrol(mean_ADC, dest_height);        // control the height with PID
                YawControl(yaw_count, dest_yaw);           // control the yaw with PID
                userInterfaceUpdate();                     // update key data
            }
        }
        else                                               // if the helicopter should not flying
        {
            land_flag = land(land_flag);                   // call landing program
            calibration_flag = 0;                          // the helicopter should be recaliberated after ther landing
            GPIOIntEnable(GPIO_PORTC_BASE, GPIO_PIN_4);    // thus Phase C interrupt was enabled
        }
    }
}
