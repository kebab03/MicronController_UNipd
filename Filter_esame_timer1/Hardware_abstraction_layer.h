/*
 * Hardware_abstraction_layer.h
 *
 *  Created on: 3 mar 2022
 *      Author: bertman38930
 */

#ifndef HARDWARE_ABSTRACTION_LAYER_H_
#define HARDWARE_ABSTRACTION_LAYER_H_

#define INPUT 1
#define OUTPUT 2
#define INPUT_PULLUP 3

#define HIGH 1
#define LOW 0

#define ACTIVATE_TIMER0         CpuTimer0Regs.TCR.bit.TSS=0
#define TIMER0_EXPIRED          (CpuTimer0Regs.TCR.bit.TIF==1)
#define CLEAR_FLAG_TIMER0       CpuTimer0Regs.TCR.bit.TIF=1
#define RESET_INTERRUPT_TIMER0  PieCtrlRegs.PIEACK.bit.ACK1=1


#define GET_ADC_SAMPLE          AdcResult.ADCRESULT3
#define CLEAR_FLAG_ADC          AdcRegs.ADCINTFLGCLR.bit.ADCINT1=1
#define RESET_INTERRUPT_ADC     PieCtrlRegs.PIEACK.bit.ACK1=1


#define PWM_TIME_BASE_PERIOD    4000
#define PWM_HALF_TIME_BASE_PERIOD 2000
#define PWM_TIME_BASE_PERIOD_LED  4000//7662

#define PHASE_RED_DUTY          EPwm4Regs.CMPA.half.CMPA
#define PHASE_BLUE_DUTY         EPwm5Regs.CMPA.half.CMPA
#define PHASE_GREEN_DUTY        EPwm6Regs.CMPA.half.CMPA

#define ENCODER_INITIAL_COUNT   180
#define ENCODER_MAXIMUM_COUNT   359
#define ENCODER_POSITION        EQep2Regs.QPOSCNT

#endif /* HARDWARE_ABSTRACTION_LAYER_H_ */
