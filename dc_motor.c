#include <xc.h>
#include "dc_motor.h"

// function initialise T2 and PWM for DC motor control
void initDCmotorsPWM(int PWMcycle){
  //  struct DC_motor *mL,*mR
	//initialise your TRIS and LAT registers for PWM
    TRISEbits.TRISE2 = 0; //sets the pin as an output - this is for the PWM
    TRISEbits.TRISE4 = 0; //sets the pin as an output
    TRISCbits.TRISC7 = 0; //sets the pin as an output - this is for the PWM
    TRISGbits.TRISG6 = 0; //sets the pin as an output
    
    LATEbits.LATE2 = 0; //0 as initial output value
    LATCbits.LATC7 = 0; //0 as initail output value
    
    // timer 2 config
    T2CONbits.CKPS=101; // 1:??? prescaler
    T2HLTbits.MODE=0b00000; // Free Running Mode, software gate only
    T2CLKCONbits.CS=0b0001; // Fosc/4

    // Tpwm*(Fosc/4)/prescaler - 1 = PTPER
    T2PR=200; //Period reg 10kHz base period
    T2CONbits.ON=1;
    
    RE2PPS=0x0A; //PWM6 on RE2
    RC7PPS=0x0B; //PMW7 on RC7

    PWM6DCH=0; //0% power
    PWM7DCH=0; //0% power
    
    PWM6CONbits.EN = 1;
    PWM7CONbits.EN = 1;
}


// function to set PWM output from the values in the motor structure
void setMotorPWM(struct DC_motor *m)
{
	int PWMduty; //tmp variable to store PWM duty cycle

	if (m->direction){ //if forward
		// low time increases with power
		PWMduty=m->PWMperiod - ((int)(m->power)*(m->PWMperiod))/100;
	}
	else { //if reverse
		// high time increases with power
		PWMduty=((int)(m->power)*(m->PWMperiod))/100;
	}

	*(m->dutyHighByte) = PWMduty; //set high duty cycle byte
        
	if (m->direction){ // if direction is high
		*(m->dir_LAT) = *(m->dir_LAT) | (1<<(m->dir_pin)); // set dir_pin bit in LAT to high without changing other bits
	} else {
		*(m->dir_LAT) = *(m->dir_LAT) & (~(1<<(m->dir_pin))); // set dir_pin bit in LAT to low without changing other bits
	}
}

//function to stop the robot gradually 
//void stop(struct DC_motor *mL, struct DC_motor *mR)
//{
//
//}

//function to make the robot turn left 
void turnLeft(struct DC_motor *mL, struct DC_motor *mR)
{
    mR->direction = 1;
    mL->direction = 0;
    for i in range(0,5){
        mL->power+=10;
        mR->power+=10;
        __delay_ms(100);
    }
}

////function to make the robot turn right 
//void turnRight(struct DC_motor *mL, struct DC_motor *mR)
//{
// 
//}
//
////function to make the robot go straight
//void fullSpeedAhead(struct DC_motor *mL, struct DC_motor *mR)
//{
//
//}

