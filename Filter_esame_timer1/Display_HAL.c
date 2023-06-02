/*
 * Display_Management.c
 *
 *  Created on: 16 mar 2022
 *      Author: bertman38930
 */

#include "F2806x_Device.h"
#include "Display_HAL.h"
#include "Constants.h"
extern int ADC_Sample;
int Show_Character_on_Digit(char, int);
int Clear_All_Digits(void);



extern int DIGIT1_ON;
extern int DIGIT2_ON;
extern int DIGIT3_ON;
extern int DIGIT4_ON;


int Show_Character_on_Digit(char Character, int Digit){
    int Result=OK;

    Clear_All_Digits();

    EALLOW;

    switch(Character){
    case '1':
        SET_SEGMENT_B;
        SET_SEGMENT_C;
        break;
    case '2':
        SET_SEGMENT_A;
        SET_SEGMENT_B;
        SET_SEGMENT_G;
        SET_SEGMENT_E;
        SET_SEGMENT_D;
        break;
    case '3':
        SET_SEGMENT_A;
        SET_SEGMENT_B;
        SET_SEGMENT_G;
        SET_SEGMENT_C;
        SET_SEGMENT_D;
        break;
    case '4':
        SET_SEGMENT_F;
        SET_SEGMENT_G;
        SET_SEGMENT_B;
        SET_SEGMENT_C;
        break;
    case '5':
        SET_SEGMENT_A;
        SET_SEGMENT_F;
        SET_SEGMENT_G;
        SET_SEGMENT_C;
        SET_SEGMENT_D;
        break;
    case '6':
        SET_SEGMENT_A;
        SET_SEGMENT_F;
        SET_SEGMENT_G;
        SET_SEGMENT_C;
        SET_SEGMENT_D;
        SET_SEGMENT_E;
        break;
    case '7':
        SET_SEGMENT_A;
        SET_SEGMENT_B;
        SET_SEGMENT_C;
        break;
    case '8':
        SET_SEGMENT_A;
        SET_SEGMENT_F;
        SET_SEGMENT_G;
        SET_SEGMENT_C;
        SET_SEGMENT_D;
        SET_SEGMENT_E;
        SET_SEGMENT_B;
        break;
    case '9':
        SET_SEGMENT_A;
        SET_SEGMENT_F;
        SET_SEGMENT_G;
        SET_SEGMENT_C;
        SET_SEGMENT_D;
        SET_SEGMENT_B;
        break;
    case '0':
        SET_SEGMENT_A;
        SET_SEGMENT_F;
        SET_SEGMENT_C;
        SET_SEGMENT_D;
        SET_SEGMENT_E;
        SET_SEGMENT_B;
        break;
    case '-':
        SET_SEGMENT_G;
        break;
    case 'E':
        SET_SEGMENT_A;
        SET_SEGMENT_F;
        SET_SEGMENT_G;
        SET_SEGMENT_D;
        SET_SEGMENT_E;
        break;
    case 'r':
        SET_SEGMENT_G;
        SET_SEGMENT_E;
        break;
    case 'l':
        SET_SEGMENT_E;
        SET_SEGMENT_F;
        break;
    case 'o':
        SET_SEGMENT_E;
        SET_SEGMENT_G;
        SET_SEGMENT_C;
        SET_SEGMENT_D;
        break;
    case 'h':
        SET_SEGMENT_E;
        SET_SEGMENT_G;
        SET_SEGMENT_C;
        SET_SEGMENT_F;
        break;
    case 'i':
        SET_SEGMENT_E;
        break;
    default:
        Result=ERROR;
        break;
    }

    if (Result==OK){
        switch(Digit){
        case 1:
            //SET_DIGIT_1;
            //EPwm1Regs.CMPA.half.CMPA=ADC_Sample;
            GpioCtrlRegs.GPAMUX1.bit.GPIO0=1;
            EPwm1Regs.CMPA.half.CMPA=DIGIT1_ON;
            break;
        case 2:
            //SET_DIGIT_2;
            //EPwm1Regs.CMPB=ADC_Sample;
            GpioCtrlRegs.GPAMUX1.bit.GPIO1=1;
            EPwm1Regs.CMPB=DIGIT2_ON;
            break;
        case 3:
            //SET_DIGIT_3;
            GpioCtrlRegs.GPAMUX1.bit.GPIO2=1;
            EPwm2Regs.CMPA.half.CMPA=DIGIT3_ON;
            break;
        case 4:
            //SET_DIGIT_4;
            GpioCtrlRegs.GPAMUX1.bit.GPIO3=1;
            EPwm2Regs.CMPB=DIGIT4_ON;
            break;
        default:
            Result=ERROR;
            break;
        }
    }

    EDIS;

    return Result;
}


int Clear_All_Digits(){
    int Result=OK;
    EALLOW;

    CLEAR_SEGMENTS_BCDEFdp;
    CLEAR_SEGMENTS_AG;

    GpioCtrlRegs.GPAMUX1.bit.GPIO0=0;
    GpioCtrlRegs.GPAMUX1.bit.GPIO1=0;
    GpioCtrlRegs.GPAMUX1.bit.GPIO2=0;
    GpioCtrlRegs.GPAMUX1.bit.GPIO3=0;
    GpioCtrlRegs.GPADIR.bit.GPIO0=1;
    GpioCtrlRegs.GPADIR.bit.GPIO1=1;
    GpioCtrlRegs.GPADIR.bit.GPIO2=1;
    GpioCtrlRegs.GPADIR.bit.GPIO3=1;

    CLEAR_DIGITS_1234;
    EDIS;
    return Result;
}




