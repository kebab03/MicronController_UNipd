/**
 * main.c
 */
#include "Hardware_abstraction_layer.h"
#include "F2806x_Device.h"
#include "Constants.h"


extern int pinMode(int,int);
extern int digitalWrite(int,int);
extern int digitalRead(int);
extern int Initialize_the_Clock(void);
extern int Initialize_the_Timer0(int);
extern int Initialize_the_Display(void);
extern int Enable_the_Peripherals(void);
extern int Show_Character_on_Digit(char, int);
extern int Initialize_the_Interrupts(void);
extern int Initialize_the_ADC(void);
extern int Initialize_the_PWM(void);
extern int Initialize_the_Encoder(void);
extern float Sinus_Interp(float);

extern int speed_rades(int);



interrupt void Timer0_isr(void);
interrupt void ADC_isr(void);


int Num_to_Digits(int, char*, char*, char*, char*);

#define MAX_COUNTER 10

#define speed_cov 350038

char Digit_1;
char Digit_2;
char Digit_3;
char Digit_4;
char Char_to_Show;

float Phase_Red_Angle;
float Phase_Blue_Angle;
float Phase_Green_Angle;
float Sin_Red_Angle;
float Sin_Blue_Angle;
float Sin_Green_Angle;
float velocita;

int Counter=MAX_COUNTER;
int ADC_Sample;
int PushButton;
int Selected_Digit=1;
int Period_Elapsed=1;
int Sin_to_Display;
int Encoder_Position;
int Filtered_ADC_Sample;
int speed;
int Red_Duty;
int Green_Duty;
int Blue_Duty;
int period_latch;
int Red_Duty1;
int Green_Duty3;
int Blue_Duty2;
int DIGIT1_ON;
int DIGIT2_ON;
int DIGIT3_ON;
int DIGIT4_ON;

int main(void)
 {   Enable_the_Peripherals();
    if(Initialize_the_Clock()==OK){
        Initialize_the_Interrupts();
        Initialize_the_ADC();
        Initialize_the_Timer0(50);
        Initialize_the_PWM();
        Initialize_the_Display();
        Initialize_the_Encoder();
        ACTIVATE_TIMER0;
        pinMode(52,INPUT);
        pinMode(34,OUTPUT);
        pinMode(39,OUTPUT);
    while(1){
        // velocita =4/16*2*Pi/(359)*f/QCPRD; f=80MHz
        if(Period_Elapsed==1){
        Period_Elapsed=0;
                if(digitalRead(52)==HIGH){//normalmente è LOW , SE PREMO ALLORA è HIGH
                    //Encoder_Position
                   Num_to_Digits(Encoder_Position, &Digit_1, &Digit_2, &Digit_3, &Digit_4);
                                        //Num_to_Digits(speed, &Digit_1, &Digit_2, &Digit_3, &Digit_4);
                    //Num_to_Digits(Filtered_ADC_Sample, &Digit_1, &Digit_2, &Digit_3, &Digit_4);
                    digitalWrite(34,0);// 34 ROSSO
                    digitalWrite(39,1);// 39 BLUE    è ON SE è LOW

                    PHASE_RED_DUTY=Red_Duty;
                    PHASE_BLUE_DUTY=Blue_Duty;
                    PHASE_GREEN_DUTY=Green_Duty;

                    }
                else{digitalWrite(34,1);
                velocita =speed_cov/period_latch;//4/16*2*Pi/(359)*f/QCPRD;
                digitalWrite(39,HIGH);
                    Sin_to_Display=(int)(1000*Sin_Red_Angle);

                    //Num_to_Digits(Sin_to_Display, &Digit_1, &Digit_2, &Digit_3, &Digit_4);

                    Num_to_Digits(velocita, &Digit_1, &Digit_2, &Digit_3, &Digit_4);
                    //Num_to_Digits(ENCODER_PERIOD, &Digit_1, &Digit_2, &Digit_3, &Digit_4);
//Num_to_Digits(speed, &Digit_1, &Digit_2, &Digit_3, &Digit_4);


                    DIGIT1_ON=ADC_Sample+338;
                    DIGIT2_ON=ADC_Sample+338;
                    DIGIT3_ON=ADC_Sample+338;
                    DIGIT4_ON=ADC_Sample+338;



                }
            }
        }
 }

//
}

int Num_to_Digits(int Number, char *p_cUnits, char *p_cTens, char *p_cHundreds, char *p_cThousands){
    int Result=OK;
    int Sign=1;
    int Units;
    int Tens;
    int Hundreds;
    int Thousands;
    if((Number>=-999)&&(Number<=9999)){
        if(Number<0){
            Number=-Number;
            Sign=-1;
        }
        Units=Number-(Number/10)*10;
        Number=Number/10;
        Tens=Number-(Number/10)*10;
        Number=Number/10;
        Hundreds=Number-(Number/10)*10;
        Number=Number/10;
        Thousands=Number-(Number/10)*10;
        if(Thousands>0){
            *p_cThousands='0'+(char)Thousands;
            *p_cHundreds='0'+(char)Hundreds;
            *p_cTens='0'+(char)Tens;
            *p_cUnits='0'+(char)Units;
        }
        else{
            if(Hundreds>0){
                *p_cThousands=' ';
                *p_cHundreds='0'+(char)Hundreds;
                *p_cTens='0'+(char)Tens;
                *p_cUnits='0'+(char)Units;
            }
            else{
                if(Tens>0){
                    *p_cThousands=' ';
                    *p_cHundreds=' ';
                    *p_cTens='0'+(char)Tens;
                    *p_cUnits='0'+(char)Units;
                }
                else{
                    *p_cThousands=' ';
                    *p_cHundreds=' ';
                    *p_cTens=' ';
                    *p_cUnits='0'+(char)Units;
                }
            }
        }


        if(Sign<0){
            *p_cThousands='-';
        }

    }
    else{
        if(Number<-999){
            *p_cUnits='o';
            *p_cTens='l';
        }
        else{
            *p_cUnits='i';
            *p_cTens='h';
        }
        *p_cHundreds='r';
        *p_cThousands='E';
        Result=ERROR;
    }

    return Result;
}


interrupt void ADC_isr(){


//   1Hz Bandwidth
//    #define K_FILTER_X      0.157054962543663e-3
//    #define K_FILTER_pX     0.157054962543663e-3
//    #define K_FILTER_pY     0.999685890074913

//  100 Hz Bandwidth
//    #define K_FILTER_X      0.015465039003347
//    #define K_FILTER_pX     0.015465039003347
//    #define K_FILTER_pY     0.969069921993306

// 10 Hz BandWidth
    #define K_FILTER_X      0.156833279540069e-2
    #define K_FILTER_pX     0.156833279540069e-2
    #define K_FILTER_pY     0.996863334409199


    float const ENCODER_TO_RADS_RATIO=TWO_PI/(ENCODER_MAXIMUM_COUNT+1);

    static float p_Filter_Input=0;
    static float p_Filter_Output=0;

    float Filter_Output;
    float Filter_Input;
/*
    int Red_Duty;
    int Green_Duty;
    int Blue_Duty;*/

    Encoder_Position=ENCODER_POSITION;
    ADC_Sample=GET_ADC_SAMPLE;

    Filter_Input=(float)ADC_Sample;
    Filter_Output=K_FILTER_pY*p_Filter_Output+K_FILTER_X*Filter_Input+K_FILTER_pX*p_Filter_Input;

    p_Filter_Input=Filter_Input;
    p_Filter_Output=Filter_Output;

    Filtered_ADC_Sample=(int)Filter_Output;

    /*
     *
     *

        EPwm1Regs.CMPA.half.CMPA=Filtered_ADC_Sample;
        EPwm1Regs.CMPB=Filtered_ADC_Sample;
        EPwm2Regs.CMPA.half.CMPA=Filtered_ADC_Sample;
        EPwm2Regs.CMPB=Filtered_ADC_Sample;*/

    //Phase_Red_Angle=Encoder_Position*ENCODER_TO_RADS_RATIO;
    Phase_Red_Angle=Encoder_Position*0.0175;
    Phase_Blue_Angle=Phase_Red_Angle-TWO_PI_OVER_THREE;
    Phase_Green_Angle=Phase_Red_Angle+TWO_PI_OVER_THREE;

    Sin_Red_Angle=Sinus_Interp(Phase_Red_Angle);
    //Sin_Red_Angle=Sinus_Interp(Phase_Red_Angle);
    Sin_Blue_Angle=Sinus_Interp(Phase_Blue_Angle);
    Sin_Green_Angle=Sinus_Interp(Phase_Green_Angle);


        Red_Duty1=(int)(PWM_HALF_TIME_BASE_PERIOD*(1+Sin_Red_Angle));
        Blue_Duty2=(int)(PWM_HALF_TIME_BASE_PERIOD*(1+Sin_Blue_Angle));
        Green_Duty3=(int)(PWM_HALF_TIME_BASE_PERIOD*(1+Sin_Green_Angle));
        /*
        Red_Duty=(int)(-(PWM_HALF_TIME_BASE_PERIOD*(1+Sin_Red_Angle))+((325+Filtered_ADC_Sample)*0.5*(1+Sin_Red_Angle)));
            Blue_Duty=(int)(-(PWM_HALF_TIME_BASE_PERIOD*(1+Sin_Blue_Angle))+((325+Filtered_ADC_Sample)*0.5*(1+Sin_Blue_Angle)));
            Green_Duty=(int)(-(PWM_HALF_TIME_BASE_PERIOD*(1+Sin_Green_Angle))+((320+Filtered_ADC_Sample)*0.5*(1+Sin_Green_Angle)));
        */
    Red_Duty=(int)((PWM_HALF_TIME_BASE_PERIOD*(1+Sin_Red_Angle))-((325+Filtered_ADC_Sample)*0.5*(1+Sin_Red_Angle)));
    Blue_Duty=(int)((PWM_HALF_TIME_BASE_PERIOD*(1+Sin_Blue_Angle))-((325+Filtered_ADC_Sample)*0.5*(1+Sin_Blue_Angle)));

    Green_Duty=(int)((PWM_HALF_TIME_BASE_PERIOD*(1+Sin_Green_Angle))-((325+Filtered_ADC_Sample)*0.5*(1+Sin_Green_Angle)));
    /*
    PHASE_RED_DUTY=Red_Duty;
    PHASE_BLUE_DUTY=Blue_Duty;
    PHASE_GREEN_DUTY=Green_Duty;
*/
    CLEAR_FLAG_ADC;
    RESET_INTERRUPT_ADC;

}



//interrupt void ADC_isr(){
//
//    int Red_Duty;
//    int Green_Duty;
//    int Blue_Duty;
//
//    Number=GET_ADC_SAMPLE;
//
//    Red_Duty=Number;
//    Blue_Duty=PWM_TIME_BASE_PERIOD-Number;
//    Green_Duty=0;
//
//    PHASE_RED_DUTY=Red_Duty;
//    PHASE_BLUE_DUTY=Blue_Duty;
//    PHASE_GREEN_DUTY=Green_Duty;
//
//    CLEAR_FLAG_ADC;
//    RESET_INTERRUPT_ADC;
//
//
//}


static int Encoder_Position_p=0;
interrupt void Timer0_isr(){

    Show_Character_on_Digit(Char_to_Show, Selected_Digit);

    switch (Selected_Digit){
        case 1:
            Selected_Digit++;
            Char_to_Show=Digit_2;
            break;
        case 2:
            Selected_Digit++;
            Char_to_Show=Digit_3;
            break;
        case 3:
            Selected_Digit++;
            Char_to_Show=Digit_4;
            break;
        case 4:
            Selected_Digit=1;
            Char_to_Show=Digit_1;
            break;
    }
    //    Show_Character_on_Digit(Char_to_Show, Selected_Digit);
    //  speed=speed_rades(Phase_Red_Angle);
    period_latch=ENCODER_PERIOD/100;//ENCODER_PERIOD_LATCH/100;
    /*if (Encoder_Position!=Encoder_Position_p){
    speed=speed_rades(Encoder_Position);
    Encoder_Position_p=Encoder_Position;
    }*/

    if(Counter--<0){
        Counter=MAX_COUNTER;
        Period_Elapsed=1;

    }

    CLEAR_FLAG_TIMER0;
    RESET_INTERRUPT_TIMER0;
}



