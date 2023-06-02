/*
 * Display_HAL.h
 *
 *  Created on: 16 mar 2022
 *      Author: bertman38930
 */

#ifndef DISPLAY_HAL_H_
#define DISPLAY_HAL_H_

#define CLEAR_SEGMENTS_BCDEFdp GpioDataRegs.GPACLEAR.all=0b00000000000010110010000000110000;
#define CLEAR_SEGMENTS_AG      GpioDataRegs.GPBCLEAR.all=0b00000000000001000001000000000000;
#define CLEAR_DIGITS_1234      GpioDataRegs.GPACLEAR.all=0b00000000000000000000000000001111;

#define SET_SEGMENT_A          GpioDataRegs.GPBSET.bit.GPIO50=1;
#define SET_SEGMENT_B          GpioDataRegs.GPASET.bit.GPIO19=1;
#define SET_SEGMENT_C          GpioDataRegs.GPASET.bit.GPIO13=1;
#define SET_SEGMENT_D          GpioDataRegs.GPASET.bit.GPIO5=1;
#define SET_SEGMENT_E          GpioDataRegs.GPASET.bit.GPIO17=1;
#define SET_SEGMENT_F          GpioDataRegs.GPASET.bit.GPIO4=1;
#define SET_SEGMENT_G          GpioDataRegs.GPBSET.bit.GPIO44=1;
#define SET_SEGMENT_dp         GpioDataRegs.GPASET.bit.GPIO16=1;

#define SET_DIGIT_1            GpioDataRegs.GPASET.bit.GPIO0=1;
#define SET_DIGIT_2            GpioDataRegs.GPASET.bit.GPIO1=1;
#define SET_DIGIT_3            GpioDataRegs.GPASET.bit.GPIO2=1;
#define SET_DIGIT_4            GpioDataRegs.GPASET.bit.GPIO3=1;

#endif /* DISPLAY_HAL_H_ */
