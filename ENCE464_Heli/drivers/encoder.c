#include "encoder.h"

encoder YawEncoder;
uint8_t lastState;

int getEncoderCount() {return YawEncoder.count;}
uint8_t getEncoderState() {return YawEncoder.state;}

/*  encoder states
 *  bin  |  dec  |   * 
 * ------|------ |   *
 *  A  B | state |   *
 *  0  0 |   0   |   *    
 *  0  1 |   1   |   *
 *  1  1 |   3   |   *
 *  1  0 |   2   V   *
 *              inc  */

void encoderInit(digPin pinA, digPin pinB)
{
	YawEncoder.A = pinA;
	YawEncoder.B = pinB;
	YawEncoder.count = 0;
	digPinIntAttach(pinA,encoderIntHandler,BOTH);
	digPinIntAttach(pinB,encoderIntHandler,BOTH);
    lastState = digPinRead(YawEncoder.A)<<1|digPinRead(YawEncoder.B);
    YawEncoder.state = lastState;
}

void encoderIntHandler()
{
	lastState = YawEncoder.state;
	YawEncoder.state  =  digPinRead(YawEncoder.A)<<1|digPinRead(YawEncoder.B);
	YawEncoder.count +=  ((lastState&1) == (YawEncoder.state & 2));
	YawEncoder.count -=  ((lastState&2) == (YawEncoder.state & 1));
    GPIOIntClear(GPIO_PORTB_BASE,GPIO_INT_PIN_0 | GPIO_INT_PIN_1);
}

