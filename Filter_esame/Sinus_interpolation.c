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


float const Sin_Table[TABLE_SIZE+2]={1.000000000000000, 0.999847695156391, 0.999390827019096, 0.998629534754574, 0.997564050259824, 0.996194698091746, 0.994521895368273, 0.992546151641322,
                                     0.990268068741570, 0.987688340595138, 0.984807753012208, 0.981627183447664, 0.978147600733806, 0.974370064785235, 0.970295726275996, 0.965925826289068,
                                     0.961261695938319, 0.956304755963035, 0.951056516295154, 0.945518575599317, 0.939692620785908, 0.933580426497202, 0.927183854566787, 0.920504853452440,
                                     0.913545457642601, 0.906307787036650, 0.898794046299167, 0.891006524188368, 0.882947592858927, 0.874619707139396, 0.866025403784439, 0.857167300702112,
                                     0.848048096156426, 0.838670567945424, 0.829037572555042, 0.819152044288992, 0.809016994374947, 0.798635510047293, 0.788010753606722, 0.777145961456971,
                                     0.766044443118978, 0.754709580222772, 0.743144825477394, 0.731353701619170, 0.719339800338651, 0.707106781186548, 0.694658370458997, 0.681998360062499,
                                     0.669130606358858, 0.656059028990507, 0.642787609686539, 0.629320391049838, 0.615661475325658, 0.601815023152048, 0.587785252292473, 0.573576436351046,
                                     0.559192903470747, 0.544639035015027, 0.529919264233205, 0.515038074910054, 0.500000000000000, 0.484809620246337, 0.469471562785891, 0.453990499739547,
                                     0.438371146789077, 0.422618261740699, 0.406736643075800, 0.390731128489274, 0.374606593415912, 0.358367949545300, 0.342020143325669, 0.325568154457157,
                                     0.309016994374947, 0.292371704722737, 0.275637355816999, 0.258819045102521, 0.241921895599668, 0.224951054343865, 0.207911690817759, 0.190808995376545,
                                     0.173648177666930, 0.156434465040231, 0.139173100960065, 0.121869343405147, 0.104528463267653, 0.087155742747658, 0.069756473744125, 0.052335956242944,
                                     0.034899496702501, 0.017452406437284, 0.000000000000000,-0.017452406437284
};

float Sinus_Interp(float angle){
    int Sign=1;
    float Table_Index_float;
    int Table_Index_int;
    float Table_Index_fract;
    float Left_Approximation;
    float Right_Approximation;
    float Interpolation;
    /*
    while(angle>PI){
        angle=TWO_PI-angle;
        Sign=-1;}
    while(angle<-PI){
        angle=angle+TWO_PI;
        Sign=-1;
    }*/
    if (angle < 0) {

      if( angle>-PI){

      angle=-angle;
      //printf("angle>-PI\n");
      }

      else if(angle>-TWO_PI && angle<-PI)
                                    {angle=-angle;
                                    angle=angle-PI;
                                    //printf("angle>-2PI  angle <-PI");
                                    Sign=-1;}}


       if(angle>PI && angle<TWO_PI)
      {    //printf("angle>PI && angle <2PI\n");
      // if(angle>((3/2)*PI))
      if(angle>PI_3_2)
                     {//printf("angle>3/2*PI==%f.\n",angle);
                     angle=TWO_PI-angle;
                     //printf("angle=TWO_PI-angle\n");
                     //printf("angle>3/2*PI==%f.\n",angle);
                         Sign=1;

                     }
                  else if (angle<PI_3_2){//printf("angle<3/2*PI\n");

                          //angle=PI_3_2-angle;
                          angle=angle-PI;

                          Sign=-1;
                          //printf("\n angle <PI_3_2==%f.\n",angle);
                          }



        //angle=TWO_PI-angle;

        //printf("\n angle <2PI angle==%f.",angle);
      }
      else if(angle >PI_OVER_TWO  && angle<PI) {
          //printf("\n angle>PI_OVER_TWO && angle <PI\n");
          //printf("\n angle=%f.\n",angle);
      angle=PI-angle;
      //printf("angle dopo sottra==%f.\n",angle);
      Sign=-1;
      }
      Table_Index_float=angle*RAD_TO_INDEX_RATIO;
      //printf("\n");

      //Table_Index_int=(int)Table_Index_float;
      Table_Index_int=Table_Index_float;
      Table_Index_fract=Table_Index_float-Table_Index_int;

      //printf("Table_Index_int==%d\n",Table_Index_int);
      //printf("\n");
      //printf("Table_Index_fract==%f\n",Table_Index_fract);
      //printf("\n");
      Left_Approximation=Sin_Table[Table_Index_int];
      Right_Approximation=Sin_Table[Table_Index_int+1];


      //printf("Right_Approximation:::%f\n",Right_Approximation);

      //printf("Table_Index_float ==%f .\n",Table_Index_float);
      //printf("\n");

  //degrees = radians × 57.29578

      // int degrees ;
  //degrees= angle*57.29578;
  //printf("degrees ==%f .\n",degrees);

      Interpolation=Sign*(Left_Approximation+(Right_Approximation-Left_Approximation)*Table_Index_fract);
      // printf("Interpolation ==%f .\n",Interpolation);

      return Interpolation;

  }
