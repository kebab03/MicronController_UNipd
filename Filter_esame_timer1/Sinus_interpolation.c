/*
 * Sinus_interpolation.c
 *
 *  Created on: 29 mar 2022
 *      Author: bertman38930
 */

#include "Constants.h"

#define TABLE_SIZE          90

//#define RAD_TO_INDEX_RATIO  57.295779513082323  // (TABLE_SIZE)/(pi/2)
float const RAD_TO_INDEX_RATIO=TABLE_SIZE/PI_OVER_TWO;

float Sinus_Interp(float);


float const Sin_Table[TABLE_SIZE+2]={
0.000000000000000,0.0174524064372835,0.034899496702501,0.0523359562429438,0.0697564737441253,0.0871557427476582,0.104528463267653,0.121869343405147,
0.139173100960065,0.156434465040231,0.17364817766693,0.190808995376545,0.207911690817759,0.224951054343865,0.241921895599668,0.258819045102521,
0.275637355816999,0.292371704722737,0.309016994374947,0.325568154457157,0.342020143325669,0.3583679495453,0.374606593415912,0.390731128489274,
0.4067366430758,0.422618261740699,0.438371146789077,0.453990499739547,0.469471562785891,0.484809620246337,0.500000000000000,0.515038074910054,
0.529919264233205,0.544639035015027,0.559192903470747,0.573576436351046,0.587785252292473,0.601815023152048,0.615661475325658,0.629320391049837,
0.642787609686539,0.656059028990507,0.669130606358858,0.681998360062498,0.694658370458997,0.707106781186547,0.719339800338651,0.73135370161917,
0.743144825477394,0.754709580222772,0.766044443118978,0.777145961456971,0.788010753606722,0.798635510047293,0.809016994374947,0.819152044288992,
0.829037572555042,0.838670567945424,0.848048096156426,0.857167300702112,0.866025403784439,0.874619707139396,0.882947592858927,0.891006524188368,
0.898794046299167,0.90630778703665,0.913545457642601,0.92050485345244,0.927183854566787,0.933580426497202,0.939692620785908,0.945518575599317,
0.951056516295154,0.956304755963035,0.961261695938319,0.965925826289068,0.970295726275996,0.974370064785235,0.978147600733806,0.981627183447664,
0.984807753012208,0.987688340595138,0.99026806874157,0.992546151641322,0.994521895368273,0.996194698091746,0.997564050259824,0.998629534754574,
0.999390827019096,0.999847695156391,1.000000000000000,0.999847695156391,
};

float Sinus_Interp(float Angle){
    int Sign=1;
    float Table_Index_float;
    int Table_Index_int;
    float Table_Index_fract;
    float Left_Approximation;
    float Right_Approximation;
    float Interpolation;

    while(Angle>PI)
        Angle=Angle-TWO_PI;

    while(Angle<-PI)
        Angle=Angle+TWO_PI;

    if (Angle<0){
        Angle=-Angle;
        Sign=-1;
    }

    if (Angle>PI_OVER_TWO)
        Angle=PI-Angle;

    Table_Index_float=Angle*RAD_TO_INDEX_RATIO;
    Table_Index_int=(int)Table_Index_float;
    Table_Index_fract=Table_Index_float-Table_Index_int;

    Left_Approximation=Sin_Table[Table_Index_int];
    Right_Approximation=Sin_Table[Table_Index_int+1];

    Interpolation=Sign*(Left_Approximation+(Right_Approximation-Left_Approximation)*Table_Index_fract);

    return Interpolation;

}