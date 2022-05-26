/**
 * \file
 * \brief example_pathfinder.c (PathFinder - basic example)
 */

// ****************************************************************************
// EXAMPLE_PATHFINDER.C
//
// To compile this example: pcompile example_pathfinder.c mr32.c && ldpic32 -w example_pathfinder.hex -p /dev/ttyS3
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
   int groundSensor, Sensors, SensorsLeft, SensorsRight;
   int vel_max, vel_max_2, vel_curva_1, vel_curva_2, vel_curva_3, vel_rotation;
   int data[3];
   int i = 0;
   int endStreetFlag = 0, endStreetCount = 0;

   vel_max = 55;
   vel_max_2 = 35;
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
         
         Sensors = bin_mean(data, 3);
         printInt(Sensors, 2 | 5 << 16);
         printf("\n");

         groundSensor = readLineSensors(0);
         switch(groundSensor)
         {
            // Robot was to go straight ahead
            case 0x04:  // 00100:
               setVel2(vel_max, vel_max);
               break;
            case 0x0E:  // 01110:
               // The velocity needs to be smaller than the max vel
               setVel2(vel_max_2,vel_max_2);
               break;

            // Robot is going to the right
            case 0x0C:  // 01100:
               setVel2(vel_curva_1, vel_max);
               break;
            case 0x08:  // 01000:
               setVel2(vel_curva_2, vel_max);
               break;         
            case 0x18:  // 11000:
               setVel2(vel_curva_3, vel_max);
               break;

            // Robot is going to the left               
            case 0x06:  // 00110:
               setVel2(vel_max, vel_curva_1);
               break;
            case 0x02:  // 00010:
               setVel2(vel_max, vel_curva_2);
               break;
            case 0x03:  // 00011:
               setVel2(vel_max, vel_curva_3);
               break;

            /**
             * @brief Condition to see if exists street on the left 
             * 
             */
            case 0x1C:  //11100:
               setVel2(-vel_curva_3, vel_max);
               delay(160);
               printStr("Possible left street\n");
               break;

            // case 0x1E:  //11110:
            //    setVel2(-vel_max_2, vel_max);
            //    printStr("Possible left street 2\n");
            //    break;
            
            case 0x10:  // 10000:
               setVel2(-vel_curva_3, vel_max);
               waitTick40ms();
               break;
            

            /**
             * @brief Condition to see if exists street on the right
             * 
             */
            case 0x07:  // 00111:
               setVel2(vel_max, -vel_curva_3);
               delay(160);
               break;

            // case 0x0F:  //01111:
            //    setVel2(-vel_max_2, vel_max);
            //    printStr("Possible left street 2\n");
            //    break;

            case 0x01:  // 00001:
               setVel2(vel_max, -vel_curva_3);
               waitTick40ms();
               break;


            /**
             * @brief End of street (turn 180º)
             * 
             */
            case 0x00:  // 00000
               // SensorsLeft = (Sensors & 0x1C) >> 2 ;
               // SensorsRight = (Sensors & 0x07);

               // // If avg xxx00
               // if (SensorsLeft == 0x07)
               // {
               //    setVel2(-30,30);
               //    waitTick40ms();
               // }
               // else if (SensorsRight == 0x18) 
               // {
               //    setVel2(50, -50); 
               // }
               // else
               // {
               //    setVel2(50, -50);
               // }
               // waitTick80ms();
               // break;
               
               //endStreetCount++;

               // if(endStreetCount == 1){
               //    setVel2(70, -70);
               //    waitTick80ms();
               //    endStreetCount = 0;
               // }
               // else{
               //    setVel2(-20, -20);
                  
               // }
               setVel2(50, -50);
               waitTick80ms();
               break;

            case 0x1F:  // 11111
               setVel2(-vel_curva_2, vel_curva_2);
               // delay(5000);
               // if (groundSensor == 0x1F)
               //    setVel2(0,0);    
               // break; 
               // groundSensor = readLineSensors(0);
               // delay(10000);
               // setVel2(vel_curva_3, -vel_curva_3);
               // if (groundSensor == 0x1F)
               //    setVel2(0,0);    
               // break; 
               
               // groundSensor = readLineSensors(0);
               // if (groundSensor == 0x1F)
               //    setVel2(0,0);    
               // break;   

            default:
               break;
         }
      } while(!stopButton());
      setVel2(0, 0);
   }
   return 0;
}

