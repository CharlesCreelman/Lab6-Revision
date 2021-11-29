#include <xc.h>
#include "dc_motor.h"

// function initialise T2 and PWM for DC motor control
void initDCmotorsPWM(int PWMcycle){
	//initialise your TRIS and LAT registers for PWM
    TRISEbits.TRISE2 = 0;
    TRISCbits.TRISC7 = 0;
    TRISEbits.TRISE4 = 0;
    TRISGbits.TRISG6 = 0;
    LATEbits.LATE2 = 0;
    LATCbits.LATC7 = 0;
    LATEbits.LATE4 = 0;
    LATGbits.LATG6 = 0;

    // timer 2 config
    T2CONbits.CKPS=0b100; // 1:??? prescaler
    T2HLTbits.MODE=0b00000; // Free Running Mode, software gate only
    T2CLKCONbits.CS=0b0001; // Fosc/4

    // Tpwm*(Fosc/4)/prescaler - 1 = PTPER
    T2PR = PWMcycle; //Period reg 10kHz base period
    T2CONbits.ON=1;

    PWM6DCH=0; //0% power
    PWM7DCH=0; //0% power
  
    
    PWM6CONbits.EN = 1;
    PWM7CONbits.EN = 1;
    
    RE2PPS=0x0A; //PWM6 on RE2
    RC7PPS=0x0B; //PMW7 on RC7
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
void stop(struct DC_motor *mL, struct DC_motor *mR)
{
    while (mL->power > 0 && mR->power > 0) {
        mL->power--;
        mR->power--;
        setMotorPWM(mL);
        setMotorPWM(mR);
        
        __delay_ms(5);
    }
    __delay_ms(200);
}

//function to make the robot turn left 
void turnLeft(struct DC_motor *mL, struct DC_motor *mR)
{
    mL->direction = 0;
    mR->direction = 1;

    while (mL->power < 80 && mR->power < 80){
        mL->power++;
        mR->power++;
        setMotorPWM(mL);
        setMotorPWM(mR);  
        __delay_ms(5);
    };
    
    __delay_ms(200);
    stop(mL,mR);

}

//function to make the robot turn right 
void turnRight(struct DC_motor *mL, struct DC_motor *mR)
{
    mL->direction = 1;
    mR->direction = 0;
    
    while (mL->power < 80 && mR->power < 80){
        mL->power++;
        mR->power++;
        setMotorPWM(mL);
        setMotorPWM(mR);
        __delay_ms(5);  
    };
    
    __delay_ms(200);
    stop(mL,mR);
}

//function to make the robot go straight
void fullSpeedAhead(struct DC_motor *mL, struct DC_motor *mR)
{
    mL->direction = 1;
    mR->direction = 1;
    
    while (mL->power < 100 && mR->power < 100){
        mL->power++;
        mR->power++;
        setMotorPWM(mR);
        setMotorPWM(mL);
        __delay_ms(5);  
    };
    
    __delay_ms(200);
    stop(mL,mR);
}

void Square (struct DC_motor *mL, struct DC_motor *mR) {
    
    fullSpeedAhead(mL,mR);
    turnLeft(mL,mR);
    fullSpeedAhead(mL,mR);
    turnLeft(mL,mR);
    fullSpeedAhead(mL,mR);
    turnLeft(mL,mR);
    fullSpeedAhead(mL,mR);
    __delay_ms(1000);
    turnRight(mL,mR);
    turnRight(mL,mR);
    fullSpeedAhead(mL,mR);
    turnRight(mL,mR);
    fullSpeedAhead(mL,mR);
    turnRight(mL,mR);
    fullSpeedAhead(mL,mR);
    turnRight(mL,mR);
    fullSpeedAhead(mL,mR);
    turnRight(mL,mR);
    turnRight(mL,mR);
}