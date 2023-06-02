/*
  Hardware_abstraction_layer.c

   Created on: 3 mar 2022
       Author: bertman38930
 */

#include "F2806x_Device.h"
#include "Constants.h"
#include "Hardware_abstraction_layer.h"
#define DUTY_CY 2000
extern interrupt void Timer0_isr(void);
extern interrupt void ADC_isr(void);

int pinMode(int,int);
int digitalWrite(int,int);
int digitalRead(int);

int speed_rades(int);

int Initialize_the_Clock(void);
int Initialize_the_Timer0(int);
int Initialize_the_Display(void);
int Initialize_the_ADC(void);
int Initialize_the_PWM(void);
int Initialize_the_Encoder(void);

int Enable_the_Peripherals(void);
int Initialize_the_Interrupts(void);
int ii=0;

int Initialize_the_Interrupts(){
    int Result=OK;

    EALLOW;
    DINT;   // Disable all the interrupts
    IER=0x0000;
    IFR=0x0000;

    PieVectTable.TINT0=&Timer0_isr;
    PieVectTable.ADCINT1=&ADC_isr;

    PieCtrlRegs.PIECTRL.bit.ENPIE=1;//abilito il PIE
    PieCtrlRegs.PIEIER1.bit.INTx7=1;//timer intrrupt


//    PieCtrlRegs.PIEIER10.bit.INTx1=1;
    PieCtrlRegs.PIEIER1.bit.INTx1=1;//Adc isr

    IER|=M_INT1;//abilita il gruppo 1 di interrupt
//    IER|=M_INT10;

    EINT;

    EDIS;

    return Result;

}


int Enable_the_Peripherals(void){
    int Result=OK;

    EALLOW;
    SysCtrlRegs.PCLKCR3.bit.CPUTIMER0ENCLK=1;   // Enable the clock of Timer0
    SysCtrlRegs.PCLKCR0.bit.ADCENCLK=1;         // Enable the clock of ADC

    SysCtrlRegs.PCLKCR1.bit.EPWM1ENCLK=1;
    SysCtrlRegs.PCLKCR1.bit.EPWM2ENCLK=1;// x disply


    SysCtrlRegs.PCLKCR1.bit.EPWM4ENCLK=1;       // Enable the clock of PWM modules 4, 5, and 6
    SysCtrlRegs.PCLKCR1.bit.EPWM5ENCLK=1;
    SysCtrlRegs.PCLKCR1.bit.EPWM6ENCLK=1;


    SysCtrlRegs.PCLKCR1.bit.EQEP2ENCLK=1;// do clk signal al encoder

    EDIS;

    return Result;
}


int Initialize_the_Encoder(){
    int Result=OK;

    EALLOW;

    GpioCtrlRegs.GPAMUX2.bit.GPIO24=2;//per usare l pin come encoder pg 128
    GpioCtrlRegs.GPAMUX2.bit.GPIO25=2;
    GpioCtrlRegs.GPAMUX2.bit.GPIO26=2;

    GpioCtrlRegs.GPAQSEL2.bit.GPIO24=0;//sto sincronizzando i pin di encoder
    GpioCtrlRegs.GPAQSEL2.bit.GPIO25=0;
    GpioCtrlRegs.GPAQSEL2.bit.GPIO26=0;


    GpioCtrlRegs.GPADIR.bit.GPIO24=0;
    GpioCtrlRegs.GPADIR.bit.GPIO25=0;
    GpioCtrlRegs.GPADIR.bit.GPIO26=0;//pin come input


    EQep2Regs.QEPCTL.bit.QPEN=0;            // Disable the counter
    EQep2Regs.QPOSINIT=ENCODER_INITIAL_COUNT;
    EQep2Regs.QPOSMAX=ENCODER_MAXIMUM_COUNT;

    EQep2Regs.QEPCTL.bit.PCRM=1;            // Position counter reset on an index event
    EQep2Regs.QDECCTL.bit.QSRC=0;           // Quadrature count mode
    EQep2Regs.QDECCTL.bit.SWAP=1;           // Quadrature-clock inputs are swapped

    EQep2Regs.QEPCTL.bit.SWI=1;             // Software init position counter
    EQep2Regs.QEPCTL.bit.SWI=0;             // Disable software init position counter
    EQep2Regs.QEPCTL.bit.IEI=2;             // Initialize the position counter on the rising edge of the QEPI signal
    EQep2Regs.QEINT.all=0;                  // Disable all interrupts
    EQep2Regs.QCLR.all=0xFFFF;              // Clear all interrupt flags
    //LEEON ADDED

    EQep2Regs.QCAPCTL.bit.CEN=1;
    EQep2Regs.QCAPCTL.bit.CCPS=16;
    EQep2Regs.QCAPCTL.bit.UPPS=4;

    //fine di leeon code

    //
    EQep2Regs.QEPCTL.bit.QCLM=1;
    EQep2Regs.QEPCTL.bit.QPEN=1;            // Enable the counter



    EDIS;

    return Result;
}


int Initialize_the_Display(void){
    int Result=OK;

    EALLOW;

 // Enable outputs to drive the digits
/*
    GpioCtrlRegs.GPAMUX1.bit.GPIO0=0;
    GpioCtrlRegs.GPAMUX1.bit.GPIO1=0;
    GpioCtrlRegs.GPAMUX1.bit.GPIO2=0;
    GpioCtrlRegs.GPAMUX1.bit.GPIO3=0;
    GpioCtrlRegs.GPADIR.bit.GPIO0=1;
    GpioCtrlRegs.GPADIR.bit.GPIO1=1;
    GpioCtrlRegs.GPADIR.bit.GPIO2=1;
    GpioCtrlRegs.GPADIR.bit.GPIO3=1;*/

 // Enable outputs to drive the segments

    GpioCtrlRegs.GPAMUX1.bit.GPIO4=0;//SEGMENT_F
    GpioCtrlRegs.GPAMUX1.bit.GPIO5=0; //SEGMENT_D
    GpioCtrlRegs.GPAMUX1.bit.GPIO13=0;//SEGMENT_C
    GpioCtrlRegs.GPAMUX2.bit.GPIO16=0;//SEGMENT_DP
    GpioCtrlRegs.GPAMUX2.bit.GPIO17=0;//SEGMENT_E
    GpioCtrlRegs.GPAMUX2.bit.GPIO19=0;//SEGMENT_B
    GpioCtrlRegs.GPBMUX1.bit.GPIO44=0;//SEGMENT_G
    GpioCtrlRegs.GPBMUX2.bit.GPIO50=0;//SEGMENT_A

    GpioCtrlRegs.GPADIR.bit.GPIO4=1;
    GpioCtrlRegs.GPADIR.bit.GPIO5=1;
    GpioCtrlRegs.GPADIR.bit.GPIO13=1;
    GpioCtrlRegs.GPADIR.bit.GPIO16=1;
    GpioCtrlRegs.GPADIR.bit.GPIO17=1;
    GpioCtrlRegs.GPADIR.bit.GPIO19=1;
    GpioCtrlRegs.GPBDIR.bit.GPIO44=1;
    GpioCtrlRegs.GPBDIR.bit.GPIO50=1;
    /*
    GpioDataRegs.GPACLEAR.bit.GPIO4=1;
    GpioDataRegs.GPACLEAR.bit.GPIO5=1;
    GpioDataRegs.GPACLEAR.bit.GPIO13=1;
    GpioDataRegs.GPACLEAR.bit.GPIO16=1;
    GpioDataRegs.GPACLEAR.bit.GPIO17=1;
    GpioDataRegs.GPACLEAR.bit.GPIO19=1;
    GpioDataRegs.GPBCLEAR.bit.GPIO44=1;
    GpioDataRegs.GPBCLEAR.bit.GPIO50=1;

    */
    GpioDataRegs.GPASET.bit.GPIO4=1;
    GpioDataRegs.GPASET.bit.GPIO5=1;
    GpioDataRegs.GPASET.bit.GPIO13=1;
    GpioDataRegs.GPASET.bit.GPIO16=1;
    GpioDataRegs.GPASET.bit.GPIO17=1;
    GpioDataRegs.GPASET.bit.GPIO19=1;
    GpioDataRegs.GPBSET.bit.GPIO44=1;
    GpioDataRegs.GPBSET.bit.GPIO50=1;


    EDIS;

    return Result;
}

int Initialize_the_PWM(void){
    int Result=OK;

    EALLOW;


    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC=0;    //Stops the clock of the enabled PWM modules
        EPwm1Regs.TBCTL.bit.CLKDIV=0;
        EPwm1Regs.TBCTL.bit.HSPCLKDIV=0;
        EPwm1Regs.TBCTL.bit.CTRMODE=2;// cnta up & down
        EPwm1Regs.TBPRD=PWM_TIME_BASE_PERIOD;
        EPwm1Regs.CMPCTL.bit.LOADAMODE=0;       // Loads CMPA from shadow register at the beginning of the PWM period
        EPwm1Regs.AQCTLA.bit.CAU=1;             // Clear PWM4A output when the counter is equal to CMPA while rising
        EPwm1Regs.AQCTLA.bit.CAD=2;             // Sets PWM4A output when the counter is equal to CMPA while falling
        EPwm1Regs.CMPA.half.CMPA=DUTY_CY;
        GpioCtrlRegs.GPAMUX1.bit.GPIO0=0;           // Sets GPIO08 as general purposes digital I/O
        GpioCtrlRegs.GPADIR.bit.GPIO0=1;            // Configures GPIO08 as output
        GpioDataRegs.GPACLEAR.bit.GPIO0=1;          // Clears pin GPIO08    (Blue)
        GpioCtrlRegs.GPAMUX1.bit.GPIO0=1;
        //X GPIO 1 è A PWM 1B
        EPwm1Regs.CMPCTL.bit.LOADBMODE=0;       // Loads CMPA from shadow register at the beginning of the PWM period
        EPwm1Regs.AQCTLB.bit.CBU=1;             // Clear PWM4A output when the counter is equal to CMPA while rising
        EPwm1Regs.AQCTLB.bit.CBD=2;
        EPwm1Regs.CMPB=DUTY_CY;             // Sets CMPA to 0 to avoid turning on of the transistors
        GpioCtrlRegs.GPAMUX1.bit.GPIO1=0;           // Sets GPIO08 as general purposes digital I/O
        GpioCtrlRegs.GPADIR.bit.GPIO1=1;            // Configures GPIO08 as output
        GpioDataRegs.GPACLEAR.bit.GPIO1=1;          // Clears pin GPIO08    (Blue)
        GpioCtrlRegs.GPAMUX1.bit.GPIO1=1;           // Sets GPIO08 as PWM5A output


        EPwm2Regs.TBCTL.bit.CLKDIV=0;
        EPwm2Regs.TBCTL.bit.HSPCLKDIV=0;
        EPwm2Regs.TBCTL.bit.CTRMODE=2;
        EPwm2Regs.TBPRD=PWM_TIME_BASE_PERIOD;
        EPwm2Regs.CMPCTL.bit.LOADAMODE=0;       // Loads CMPA from shadow register at the beginning of the PWM period
        EPwm2Regs.AQCTLA.bit.CAU=1;             // Clear PWM4A output when the counter is equal to CMPA while rising
        EPwm2Regs.AQCTLA.bit.CAD=2;             // Sets PWM4A output when the counter is equal to CMPA while falling
        EPwm2Regs.CMPA.half.CMPA=DUTY_CY;

        GpioCtrlRegs.GPAMUX1.bit.GPIO2=0;           // Sets GPIO08 as general purposes digital I/O
        GpioCtrlRegs.GPADIR.bit.GPIO2=1;            // Configures GPIO08 as output
        GpioDataRegs.GPACLEAR.bit.GPIO2=1;          // è IL DIGIT 2 DA SX O IL 3 PARTENDO DA DEX SE USB TI ENTRA IN BOCCA DI BOARD,,,,,Clears pin GPIO08    (Blue)
        GpioCtrlRegs.GPAMUX1.bit.GPIO2=1;
        //PER GPIO 3 è A PWM 2B
        EPwm2Regs.CMPCTL.bit.LOADBMODE=0;       // Loads CMPA from shadow register at the beginning of the PWM period
        EPwm2Regs.AQCTLB.bit.CBU=1;             // Clear PWM4A output when the counter is equal to CMPA while rising
        EPwm2Regs.AQCTLB.bit.CBD=2;
        EPwm2Regs.CMPB=DUTY_CY;             // Sets CMPA to 0 to avoid turning on of the transistors
        GpioCtrlRegs.GPAMUX1.bit.GPIO3=0;           // Sets GPIO10 as general purposes digital I/O
        GpioCtrlRegs.GPADIR.bit.GPIO3=1;            // Configures GPIO10 as output
        GpioDataRegs.GPACLEAR.bit.GPIO3=1;          // Clears pin GPIO10    (Red)
        GpioCtrlRegs.GPAMUX1.bit.GPIO3=1;           // Sets GPIO10 as PWM6A output
//led
    EPwm4Regs.TBCTL.bit.CLKDIV=0;//prescale di sbase dei tempi
    EPwm4Regs.TBCTL.bit.HSPCLKDIV=0;//high speed
    EPwm4Regs.TBCTL.bit.CTRMODE=2;//counter mode up && down =b10

    EPwm4Regs.TBPRD=PWM_TIME_BASE_PERIOD_LED;// periodo di pwm cioè il max contegio determina il periodo

    EPwm4Regs.CMPCTL.bit.LOADAMODE=0;       // Loads CMPA from shadow register at the beginning of the PWM period

    EPwm4Regs.AQCTLA.bit.CAU=1;             // Clear PWM4A output when the counter is equal to CMPA while rising
    EPwm4Regs.AQCTLA.bit.CAD=2;             // Sets PWM4A output when the counter is equal to CMPA while falling

    EPwm4Regs.CMPA.half.CMPA=0;             // Sets CMPA to 0 to avoid turning on of the transistors
// uso pwm  per triggerare adc
    EPwm4Regs.ETSEL.bit.SOCASEL=1;  // genera il segnale di trigger di soc al inizio di periodo di pwm        // Pulse SOCA is triggered when the counter of PWM4 is 0
    EPwm4Regs.ETSEL.bit.SOCAEN=1;           // Enable SOCA pulse generation
    EPwm4Regs.ETPS.bit.SOCAPRD=1;           // Generates SOCA pulse every time the counter of PWM4 is 0


    GpioCtrlRegs.GPAMUX1.bit.GPIO6=0;           // Sets GPIO06 as general purposes digital I/O
    GpioCtrlRegs.GPADIR.bit.GPIO6=1;            // Configures GPIO06 as output
    GpioDataRegs.GPACLEAR.bit.GPIO6=1;          // Clears pin GPIO06    (Red)
    //serve impostare come gpio out put x dare valore 0, se lo uso come pwm non posso impostare valore 0

    GpioCtrlRegs.GPAMUX1.bit.GPIO6=1;           // Sets GPIO06 as PWM4A output
    EPwm5Regs.TBCTL.bit.CLKDIV=0;
    EPwm5Regs.TBCTL.bit.HSPCLKDIV=0;
    EPwm5Regs.TBCTL.bit.CTRMODE=2;
    EPwm5Regs.TBPRD=PWM_TIME_BASE_PERIOD_LED;
    EPwm5Regs.CMPCTL.bit.LOADAMODE=0;       // Loads CMPA from shadow register at the beginning of the PWM period
    EPwm5Regs.AQCTLA.bit.CAU=1;             // Clear PWM4A output when the counter is equal to CMPA while rising
    EPwm5Regs.AQCTLA.bit.CAD=2;             // Sets PWM4A output when the counter is equal to CMPA while falling
    EPwm5Regs.CMPA.half.CMPA=0;             // Sets CMPA to 0 to avoid turning on of the transistors

    GpioCtrlRegs.GPAMUX1.bit.GPIO8=0;           // Sets GPIO08 as general purposes digital I/O
    GpioCtrlRegs.GPADIR.bit.GPIO8=1;            // Configures GPIO08 as output
    GpioDataRegs.GPACLEAR.bit.GPIO8=1;          // Clears pin GPIO08    (Blue)
    GpioCtrlRegs.GPAMUX1.bit.GPIO8=1;           // Sets GPIO08 as PWM5A output


    EPwm6Regs.TBCTL.bit.CLKDIV=0;
    EPwm6Regs.TBCTL.bit.HSPCLKDIV=0;
    EPwm6Regs.TBCTL.bit.CTRMODE=2;

    EPwm6Regs.TBPRD=PWM_TIME_BASE_PERIOD_LED;

    EPwm6Regs.CMPCTL.bit.LOADAMODE=0;       // Loads CMPA from shadow register at the beginning of the PWM period

    EPwm6Regs.AQCTLA.bit.CAU=1;             // Clear PWM4A output when the counter is equal to CMPA while rising
    EPwm6Regs.AQCTLA.bit.CAD=2;             // Sets PWM4A output when the counter is equal to CMPA while falling

    EPwm6Regs.CMPA.half.CMPA=0;             // Sets CMPA to 0 to avoid turning on of the transistors

    GpioCtrlRegs.GPAMUX1.bit.GPIO10=0;           // Sets GPIO10 as general purposes digital I/O
    GpioCtrlRegs.GPADIR.bit.GPIO10=1;            // Configures GPIO10 as output
    GpioDataRegs.GPACLEAR.bit.GPIO10=1;          // Clears pin GPIO10    (Red)
    GpioCtrlRegs.GPAMUX1.bit.GPIO10=1;           // Sets GPIO10 as PWM6A output


    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC=1;    //Starts and synchronizes the clocks of the enabled PWM blocks


    EDIS;


    return Result;
}




int Initialize_the_ADC(){
    int Result=OK;
    int Attesa=1000;
    int Temp_Reg;

    EALLOW;

    AdcRegs.ADCCTL1.bit.ADCREFSEL=0;    // Selects the internal voltage reference
    Temp_Reg=AdcRegs.ADCCTL1.all;
    Temp_Reg|=0x00E0;
    AdcRegs.ADCCTL1.all=Temp_Reg;
    //AdcRegs.ADCCTL1.bit.ADCPWDN=1;      // Enable the power to the ADC circuitry
    //AdcRegs.ADCCTL1.bit.ADCBGPWD=1;
    //AdcRegs.ADCCTL1.bit.ADCREFPWD=1;
    AdcRegs.ADCCTL1.bit.ADCENABLE=1;

    while(Attesa-->0){};

    AdcRegs.ADCCTL1.bit.INTPULSEPOS=1;      // Interrupt pulses at the end of the conversion
    AdcRegs.ADCCTL1.bit.TEMPCONV=1;         // Input 5 connected internally to the temperature sensor

    AdcRegs.ADCCTL2.bit.ADCNONOVERLAP=1;    // Sampling is not overlapped
    AdcRegs.ADCCTL2.bit.CLKDIV2EN=1;        // ADCCLK = SYSCLK / 4
    AdcRegs.ADCCTL2.bit.CLKDIV4EN=1;

    AdcRegs.ADCINTFLGCLR.all=0xFFFF;        // Clear all the interrupt flags

    AdcRegs.INTSEL1N2.bit.INT1CONT=0;       // Disable continuous conversion
    AdcRegs.INTSEL1N2.bit.INT1E=1;          // Enables the interrupt
    AdcRegs.INTSEL1N2.bit.INT1SEL=3;        // Interrupt 1 is triggered by EOC3

    AdcRegs.INTSEL1N2.bit.INT2CONT=0;       // Disable continuous conversion
    AdcRegs.INTSEL1N2.bit.INT2E=0;          // Interrupt not enebled
    AdcRegs.INTSEL1N2.bit.INT2SEL=5;        // Interrupt 2 is triggered by EOC 5


    AdcRegs.ADCSOC3CTL.bit.TRIGSEL=0x0B;    // SOC triggered by ePWM4, ADCSOCA
//    AdcRegs.ADCSOC3CTL.bit.TRIGSEL=0x01;    // SOC triggered by Timer 0
    AdcRegs.ADCSOC3CTL.bit.CHSEL=6;
    AdcRegs.ADCSOC3CTL.bit.ACQPS=0x3F;// lunghezza dellafinestra di conversione.è il max valore

//    AdcRegs.ADCSOC5CTL.bit.TRIGSEL=0;       // SOC triggered by software
//    AdcRegs.ADCSOC5CTL.bit.CHSEL=5;
//    AdcRegs.ADCSOC5CTL.bit.ACQPS=0x3F;


    return Result;
}


int Initialize_the_Timer0(int Number_of_Tenths_of_Millisecond){
    int Result=OK;
    EALLOW;

    CpuTimer0Regs.TCR.bit.TSS=1;        // Stops the timer se metto 0 timer parte
    CpuTimer0Regs.TPR.bit.TDDR=0x3f;    // Sets the divider to 7999 (0x1f3f) so the frequency of the clock entering the
                                        // counter is 80MHz/8000 = 10kHz
    CpuTimer0Regs.TPRH.bit.TDDRH=0x1f;

    CpuTimer0Regs.PRD.half.LSW=Number_of_Tenths_of_Millisecond;
//serve per caricare i registri
    CpuTimer0Regs.TCR.bit.TRB=1;        // Initializes the counter and the divider
    CpuTimer0Regs.TCR.bit.TIE=1;        // ablito the interrupt
    CpuTimer0Regs.TCR.bit.TIF=1;        // Clear the timer flag

    EDIS;
    return Result;
}








int Initialize_the_Clock(void){
    int Result=OK;

    if(SysCtrlRegs.PLLSTS.bit.MCLKSTS==1){
        Result=ERROR;
    }
    else{

        EALLOW;

// Initializes the clock generation
        SysCtrlRegs.XCLK.bit.XCLKOUTDIV=3;      // Disables the clock output pin
        SysCtrlRegs.CLKCTL.bit.XCLKINOFF=1;     // Disables the input for the external clock
        SysCtrlRegs.CLKCTL.bit.INTOSC2OFF=1;    // Disables the internal oscillator 2

// Initialize the PLL (see pag. 82 of the TMS320x2806x Technical Reference Manual)
        SysCtrlRegs.PLLSTS.bit.DIVSEL=0;        // Sets the CPU clock frequency equal to the oscillator clock frequency/4
        SysCtrlRegs.PLLSTS.bit.MCLKOFF=1;       // Disable failed oscillator detect logic
        SysCtrlRegs.PLLCR.bit.DIV=8;            // Sets the PLL output frequency to 8*oscillator lock frequency

        while(SysCtrlRegs.PLLSTS.bit.PLLLOCKS==0){
                                                // Waits for the PLL to lock
        }

        SysCtrlRegs.PLLSTS.bit.MCLKOFF=0;       // Enables failed oscillator detect logic

        SysCtrlRegs.PLLSTS.bit.DIVSEL=3;        // Sets the CPU clock frequency equal to the oscillator clock frequency
                                     // In this case is 80 MHz. The maximum is 90 MHz

// Sets the peripheral clock frequency
        SysCtrlRegs.LOSPCP.bit.LSPCLK=2;        // Peripheral Low Speed Clock = System Clock/4

        EDIS;
    }

    return Result;
}





int pinMode(int Pin_Number, int Mode){
    int Result=OK;
    long Mask_1bit=1;
    long Mask_2bits=3;

    long Temp_Reg;


    if((Pin_Number<0)|(Pin_Number>58)){
        Result=ERROR;
    }
    else{

        EALLOW;
        if(Pin_Number<32){
            // Porta A
            Mask_1bit=Mask_1bit<<Pin_Number;

            if(Pin_Number<16){
                Mask_2bits=Mask_2bits<<(Pin_Number*2);
                Temp_Reg=GpioCtrlRegs.GPAMUX1.all;
                Temp_Reg=(Temp_Reg)&(~Mask_2bits);
                GpioCtrlRegs.GPAMUX1.all=Temp_Reg;

            }
            else{
                Mask_2bits=Mask_2bits<<((Pin_Number-16)*2);
                Temp_Reg=GpioCtrlRegs.GPAMUX2.all;
                Temp_Reg=(Temp_Reg)&(~Mask_2bits);
                GpioCtrlRegs.GPAMUX2.all=Temp_Reg;

            }

            if(Mode==INPUT){
                // Devo mettere a 0 il bit corrispondente in GPADIR

                Temp_Reg=GpioCtrlRegs.GPADIR.all;
                Temp_Reg=(Temp_Reg)&(~Mask_1bit);
                GpioCtrlRegs.GPADIR.all=Temp_Reg;

                Temp_Reg=GpioCtrlRegs.GPAPUD.all;
                Temp_Reg=(Temp_Reg)|(Mask_1bit);
                GpioCtrlRegs.GPAPUD.all=Temp_Reg;

            }
            else{
                    if(Mode==OUTPUT){
                        // Devo mettere a 0 il bit corrispondente in GPADIR

                        Temp_Reg=GpioCtrlRegs.GPADIR.all;
                        Temp_Reg=(Temp_Reg)|(Mask_1bit);
                        GpioCtrlRegs.GPADIR.all=Temp_Reg;

                    }
                    else{
                        if(Mode==INPUT_PULLUP){

                            Temp_Reg=GpioCtrlRegs.GPADIR.all;
                            Temp_Reg=(Temp_Reg)&(~Mask_1bit);
                            GpioCtrlRegs.GPADIR.all=Temp_Reg;

                            Temp_Reg=GpioCtrlRegs.GPAPUD.all;
                            Temp_Reg=(Temp_Reg)&(~Mask_1bit);
                            GpioCtrlRegs.GPAPUD.all=Temp_Reg;


                        }
                        else{
                            Result=ERROR;
                        }
                    }
            }


        }
        else{

            Mask_1bit=Mask_1bit<<(Pin_Number-32);


            if(Pin_Number<48){
                Mask_2bits=Mask_2bits<<((Pin_Number-32)*2);
                Temp_Reg=GpioCtrlRegs.GPBMUX1.all;
                Temp_Reg=(Temp_Reg)&(~Mask_2bits);
                GpioCtrlRegs.GPBMUX1.all=Temp_Reg;

            }
            else{
                Mask_2bits=Mask_2bits<<((Pin_Number-48)*2);
                Temp_Reg=GpioCtrlRegs.GPBMUX2.all;
                Temp_Reg=(Temp_Reg)&(~Mask_2bits);
                GpioCtrlRegs.GPBMUX2.all=Temp_Reg;

            }



            switch(Mode){

            case INPUT:
                Temp_Reg=GpioCtrlRegs.GPBDIR.all;
                Temp_Reg=(Temp_Reg)&(~Mask_1bit);
                GpioCtrlRegs.GPBDIR.all=Temp_Reg;

                Temp_Reg=GpioCtrlRegs.GPBPUD.all;
                Temp_Reg=(Temp_Reg)|(Mask_1bit);
                GpioCtrlRegs.GPBPUD.all=Temp_Reg;
                break;

            case OUTPUT:
                Temp_Reg=GpioCtrlRegs.GPBDIR.all;
                Temp_Reg=(Temp_Reg)|(Mask_1bit);
                GpioCtrlRegs.GPBDIR.all=Temp_Reg;
                break;

            case INPUT_PULLUP:
                Temp_Reg=GpioCtrlRegs.GPBDIR.all;
                Temp_Reg=(Temp_Reg)&(~Mask_1bit);
                GpioCtrlRegs.GPBDIR.all=Temp_Reg;

                Temp_Reg=GpioCtrlRegs.GPBPUD.all;
                Temp_Reg=(Temp_Reg)&(~Mask_1bit);
                GpioCtrlRegs.GPBPUD.all=Temp_Reg;
                break;

            default:
                Result=ERROR;
                break;


            }


            // Porta B
        }

        EDIS;
    }
    return Result;
}



int digitalRead(int Pin_Number){
    int Result=OK;
    long Mask_1bit=1;

    long Temp_Reg;

    if((Pin_Number<0)|(Pin_Number>58)){
        Result=ERROR;
    }
    else{

        if(Pin_Number<32){
            // Porta A
            Mask_1bit=Mask_1bit<<Pin_Number;

            Temp_Reg=GpioDataRegs.GPADAT.all;
            Temp_Reg=(Temp_Reg)&(Mask_1bit);
            if(Temp_Reg==0)
                Result=LOW;
            else
                Result=HIGH;

        }
        else{

            Mask_1bit=Mask_1bit<<(Pin_Number-32);

            Temp_Reg=GpioDataRegs.GPBDAT.all;
            Temp_Reg=(Temp_Reg)&(Mask_1bit);
            if(Temp_Reg==0)
                Result=LOW;
            else
                Result=HIGH;
        }

    }
    return Result;
}


int digitalWrite(int Pin_Number, int Value){
    int Result=OK;
    long Mask_1bit=1;

    if((Pin_Number<0)||(Pin_Number>58)){
        Result=ERROR;
    }
    else{

        if(Pin_Number<32){
            // Porta A
            Mask_1bit=Mask_1bit<<Pin_Number;
            if(Value==HIGH)
                GpioDataRegs.GPASET.all=Mask_1bit;
            else
                if(Value==LOW)
                    GpioDataRegs.GPACLEAR.all=Mask_1bit;
                else
                    Result=ERROR;
        }
        else{

            Mask_1bit=Mask_1bit<<(Pin_Number-32);

            if(Value==HIGH)
                GpioDataRegs.GPBSET.all=Mask_1bit;
            else
                if(Value==LOW)
                    GpioDataRegs.GPBCLEAR.all=Mask_1bit;
                else
                    Result=ERROR;
        }

    }
    return Result;
}

#define N  5
const float t_sample=0.005;
int posizione[N]={1,0,0,0,2};
int speed_rades(int u)
{
float speed_rade;

//static int ii ;

float diff=0;
//%riempio il vettore con le posizioni
//for
//posizione[ii] = u;
/*
if (ii == N)
{
    //diff = posizione[N-1] - posizione[0];
    ii = 0;}*/
if (ii == N){
    diff = posizione[N-1] - posizione[0];
    ii = 0;
}else{
    diff = posizione[ii] - posizione[ii+1];

}
if(diff>pi){
    diff = diff - 2*pi;
}else if (diff < -pi){
        diff = diff+2*pi;
}
speed_rade= diff/((N-1)*t_sample*2);

posizione[ii] = u;
ii = ii + 1;
//return 0;
        //
return(int)speed_rade;

}
