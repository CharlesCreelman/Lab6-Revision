// CONFIG1L
#pragma config FEXTOSC = HS     // External Oscillator mode Selection bits (HS (crystal oscillator) above 8 MHz; PFM set to high power)
#pragma config RSTOSC = EXTOSC_4PLL// Power-up default value for COSC bits (EXTOSC with 4x PLL, with EXTOSC operating per FEXTOSC bits)

// CONFIG3L
#pragma config WDTCPS = WDTCPS_31// WDT Period Select bits (Divider ratio 1:65536; software control of WDTPS)
#pragma config WDTE = OFF        // WDT operating mode (WDT enabled regardless of sleep)

#include <xc.h>
#include "rc_servo.h"
#include "ADC.h"
#include "dc_motor.h"

#define _XTAL_FREQ 64000000 //note intrinsic _delay function is 62.5ns at 64,000,000Hz  

//Exercise 1

//void main(void){
//    Timer0_init();
//    Interrupts_init();
//    TRISDbits.TRISD5 = 0;
//	//don't forget TRIS for your output!
//
//    while(1){
//		//write your code to call angle2PWM() to set the servo angle
//        int p_angle;
//        for (p_angle=-90;p_angle<91;p_angle++){
//            angle2PWM(p_angle);
//            __delay_ms(30);    // takes 20ms for 1 degree, and 20 degrees/s = 1 degree in 50ms, so require delay of extra 30ms
//        }
//    }
//}

//Exercise 2

//void main(void){
//    Timer0_init();
//    Interrupts_init();
//    ADC_init();
//    TRISDbits.TRISD5 = 0;
//	//don't forget TRIS for your output!
//    while(1){
//
//        unsigned int light = ADC_getval();
//		//write your code to call angle2PWM() to set the servo angle
//        //int p_angle;
//        
//        if (light < 30){
//            angle2PWM(-30);
//            //__delay_ms(30);    // takes 20ms for 1 degree, and 20 degrees/s = 1 degree in 50ms, so require delay of extra 30ms
//        }else{
//            angle2PWM(30);
//        }
//
//    }
//        
//
//}


//Exercise 3

void main(void){
    int PWMcycle = 200;
    initDCmotorsPWM();

    struct DC_motor motor_Left;
    struct DC_motor motor_Right;
    struct DC_motor *mL;
    struct DC_motor *mR;
    mL = &motor_Left;
    mR = &motor_Right;

    //struct DC_motor *mL,*mR;
   // mL=&motor_Left;
    //mR=&motor_Right;   
    
    mL->power=0; 						//zero power to start
    mL->direction=1; 					//set default motor direction
    mL->dutyHighByte=(unsigned char *)(&PWM6DCH);	//store address of PWM duty high byte
    mL->dir_LAT=(unsigned char *)(&LATE); 		//store address of LAT
    mL->dir_pin=4; 						//pin RE4 controls direction
    mL->PWMperiod=PWMcycle; 			//store PWMperiod for motor

    //same for motorR but different PWM register, LAT and direction pin
    mR->power=0; 						//zero power to start
    mR->direction=1; 					//set default motor direction
    mR->dutyHighByte=(unsigned char *)(&PWM7DCH);	//store address of PWM duty high byte
    mR->dir_LAT=(unsigned char *)(&LATG); 		//store address of LAT
    mR->dir_pin=6; 						//pin RG6 controls direction
    mR->PWMperiod=PWMcycle; 			//store PWMperiod for motor
    
    while(1){
        mL->power=0;
        mR->power=0; 
        turnLeft(mL, mR);
    }
}
