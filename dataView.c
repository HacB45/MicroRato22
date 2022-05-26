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

/**
 * @file example_pathfinder.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-05-26
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "mr32.h"
#include "math.h"


int bin_mean(unsigned int *array, int length){
   int i, j, bits_all;
   int bits[3];
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
   int vel_max, vel_max_2, vel_curva_1, vel_curva_2, vel_curva_3, vel_rotation;
   int data[3];
   int i = 0;

   vel_max = 60;
   vel_max_2 = 45;
   vel_curva_1 = 30;
   vel_curva_2 = 20;
   vel_curva_3 = 10;
   vel_rotation = 60;


   initPIC32();
   closedLoopControl( true );
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
            
        for(i = 0; i < 3; i++){
            data[i] = readLineSensors(0);
         }
         
         groundSensor = bin_mean(data, 3);
         printInt(groundSensor, 2 | 5 << 16);
         printf("\n");

      
      } while(!stopButton());
      setVel2(0, 0);
   }
   return 0;
}

