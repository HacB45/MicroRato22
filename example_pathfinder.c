/**
 * \file
 * \brief example_pathfinder.c (PathFinder - basic example)
 */

// ****************************************************************************
// EXAMPLE_PATHFINDER.C
//
// To compile this example: pcompile example_pathfinder.c mr32.c
// ****************************************************************************
//
#include "mr32.h"
#include "math.h"

int bin_mean(unsigned int *array, int length){
   int i, j, bits_all;
   int bits[10];
   int bit[5];
   int sumbits = 0;
   double avg;

   for(i = 0; i < 5; i++){
      for(j = 0; j < length; j++){
         bits[j] = (array[j] >> i) & 0x01;
         sumbits+=bits[j];
      }
      avg = (double)sumbits/length;
      if (avg >=0.5){
         bit[i] = 1;
      } else {
         bit[i] = 0;
      }
      sumbits=0;
   }

   bits_all = (bit[4] << 4) | (bit[3] << 3) | (bit[2] << 2) | (bit[1] << 1) | (bit[0] << 0);
   return bits_all;
}

int main(void)
{
   int groundSensor;
   int vel_max, vel_curva_st, vel_curva_hd, vel_rotl, vel_roth;
   int data[10];
   int i = 0;

   vel_max = 50;
   vel_curva_st = 40;
   vel_curva_hd = 40;
   vel_rotl = 25;
   vel_roth = 50;


   initPIC32();
   closedLoopControl( false );
   setVel2(0, 0);

   printf("PathFinder example\n\n\n");

   while(1)
   {
      printf("Press start to continue\n");
      while(!startButton())
      {
         waitTick80ms();
         groundSensor = readLineSensors(0);	// Read ground sensor
         //printInt(groundSensor, 2 | 5 << 16);
         //printf("\n");
      }

      do
      {

         for(i = 0; i < 10; i++){
            data[i] = readLineSensors(0);
         }

         groundSensor = bin_mean(data, 10);
         printInt(groundSensor, 2 | 5 << 16);
         printf("\n");

         switch(groundSensor)
         {
            case 0x04:  // 00100:
               setVel2(vel_max, vel_max);
               break;
            case 0x0C:  // 01100:
               setVel2(vel_curva_st, vel_max);
               break;
            case 0x08:  // 01000:
               setVel2(vel_curva_hd, vel_max);
               break;
            case 0x10:  // 10000:
               setVel2(-vel_rotl, vel_roth);
               break;
            case 0x06:  // 00110:
               setVel2(vel_max, vel_curva_st);
               break;
            case 0x02:  // 00010:
               setVel2(vel_max, vel_curva_hd);
               break;
            case (0x01):  // 00001:
               setVel2(vel_roth, -vel_rotl);
               break;
            case 0x00:  // 00000
               setVel2(-70, 70);
               break;
            default:
               setVel2(vel_max, vel_max);
               break;
         }
      } while(!stopButton());
      setVel2(0, 0);
   }
   return 0;
}

