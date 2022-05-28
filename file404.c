
#include "mr32.h"

int main(void){
    
    initPIC32();
    closedLoopControl( true );
    setVel2(0, 0);
    printf("404: Failure Not Found\n\tReady!\n\n");
    
    int sensorRead;
    int maxVel = 60, normalVel = 50, rotateVel = 40, rotateVelMin = 20;
    int i;
    int CurveFlag = 0;
    int sample, front, stop; 
    int sampleRight, sampleLeft;
    int velLevel;
    int adeus = 15;
    int rightFlag =0;

    while(1)
    {
        while(!startButton())
        {
            waitTick80ms();
            sensorRead = readLineSensors(0);
        }

        do{

            sensorRead = readLineSensors(0);

            printInt(sensorRead, 2 | 5 << 16);
            printf("\n");




            switch(sensorRead)
            {
                /****************************************************************
                 * @brief Follow the straight line
                 * 
                 */
                case 0x04:  // 00100:
                    setVel2(maxVel, maxVel);
                    velLevel=0;
                    break;
                case 0x0C:  // 01100:
                    setVel2(normalVel, maxVel);
                    velLevel=1;
                    break;
                case 0x08:  // 01000:
                    setVel2(30, maxVel);
                    velLevel=2;
                    break;         
                // case 0x18:  // 11000:
                //     setVel2(rotateVel, maxVel);
                //     velLevel=2;
                    break;            
                case 0x06:  // 00110:
                    setVel2(maxVel, normalVel);
                    velLevel=3;
                    break;
                case 0x02:  // 00010:
                    setVel2(maxVel, 30);
                    velLevel=4;
                    break;
                // case 0x03:  // 00011:
                //     setVel2(maxVel, rotateVel);
                //     velLevel=4;
                //     break;

                /****************************************************************/

                // case 0x04:  // 00100:
                //     // The velocity needs to be smaller than the max vel
                //     setVel2(maxVel,maxVel);
                //     break;

                // case 0x0E:  // 01110:
                //     // The velocity needs to be smaller than the max vel
                //     setVel2(maxVel,maxVel);
                //     break;

                // case 0x0C:  // 01100:
                //     setVel2(normalVel, maxVel);                   
                //     break; 

                // case 0x06:  // 00110:
                //     setVel2(maxVel, normalVel);
                //     break;

                // case 0x03:  // 00011:
                //     setVel2(maxVel, rotateVel);
                //     break;

                // case 0x18:  // 11000:
                //     setVel2(rotateVel, maxVel);
                //     break;                
                //-----------------------------------------------------------------------------

                // /**
                //  * @brief Right Curve
                //  * 
                //  */       
                // case 0x0F:  // 01111:
                //     delay(400);
                //     sensorRead = readLineSensors(0);
                //     if (sensorRead == 0x00) //
                //     {
                //         setVel2(maxVel, -rotateVel);
                //         delay(1300);
                //     }
                //     break;

                // case 0x07:  // 00111:
                //     delay(400);
                //     sensorRead = readLineSensors(0);
                //     if (sensorRead == 0x00)
                //     {
                //         setVel2(maxVel, -rotateVel);
                //         delay(1300);
                //     }
                    
                //     break;

                /***********************************************************************
                 * @brief 180 graus
                 * 
                 */

                case 0x00: // 00000
                    do
                    {
                        setVel2(normalVel,-normalVel);
                        velLevel=5;
                        sample = readLineSensors(0); 
                        sample = sample & 0x02;
                    } while (sample!=0x02);
                    break;

                /***********************************************************************/


                // /************************************************************************
                //  * @brief Left Turn
                //  * 
                //  */

                
                case 0x18:
                    delay(500);
                    sensorRead = readLineSensors(0);
                    front = (sensorRead & 0x04) >> 2;
                    if (front == 0)
                    {
                        delay(600);
                        setVel2(-35,45);
                        delay(4000); 
                    }
                    break;


                case 0x10:
                    delay(500);
                    sensorRead = readLineSensors(0);
                    front = (sensorRead & 0x04) >> 2;
                    if (front == 0)
                    {
                        delay(600);
                        setVel2(-35,45);
                        delay(4000); 
                    }
                    break;

                case 0x1C:
                    delay(500);
                    sensorRead = readLineSensors(0);
                    front = (sensorRead & 0x04) >> 2;
                    if (front == 0)
                    {
                        delay(600);
                        setVel2(-35,45);
                        delay(4000); 
                    }
                    break;


                // case 0x1:
                //     sampleRight = sensorRead & 0x01;
                //     sampleLeft = sensorRead >> 4;

                //     if (sampleRight == 0x1)
                //     {
                //         //delay = 600
                //         delay(600);
                //         setVel2(50,-40);
                //         delay(1100);
                //         rightFlag = 1;

                //     }
                //     else if (sampleRight == 0x1 && sampleLeft == 0x1) // T
                //     {
                //         delay(600);
                //         setVel2(50,-40);
                //         delay(1100);
                //     }
                //     break;
                // case 0x1C: // 11100    
                //     do
                //     {
                //         setVel2(rotateVelMin,rotateVelMin);
                //         sample = readLineSensors(0);                  
                //     } while (sample!=0x00);

                //     do
                //     {
                //         setVel2(-normalVel,normalVel);
                //         sample = readLineSensors(0);
                //         sample = sample & 0x08;
                //     } while (sample!=0x08);
                //     break;
                
                // case 0x1E: // 11110    
                //     do
                //     {
                //         setVel2(rotateVelMin,rotateVelMin);
                //         sample = readLineSensors(0);                  
                //     } while (sample!=0x00);

                //     do
                //     {
                //         setVel2(-normalVel,normalVel);
                //         sample = readLineSensors(0);
                //         sample = sample & 0x08;
                //     } while (sample!=0x08);
                //     break;


                // /***********************************************************************/
                

                // /*************************************************************************
                //  * @brief Right Turn
                //  * 
                //  */

                // case 0x07: // 00111
                //     do
                //     {
                //         setVel2(rotateVelMin,rotateVelMin);
                //         sample = readLineSensors(0);                  
                //     } while (sample!=0x00);

                //     do
                //     {
                //         setVel2(normalVel,-normalVel);
                //         sample = readLineSensors(0);
                //         sample = sample & 0x02;
                //     } while (sample!=0x02);
                //     break;

                // case 0x0F: // 01111
                //     do
                //     {
                //         setVel2(rotateVelMin,rotateVelMin);
                //         sample = readLineSensors(0);                  
                //     } while (sample!=0x00);

                //     do
                //     {
                //         setVel2(normalVel,-normalVel);
                //         sample = readLineSensors(0);
                //         sample = sample & 0x02;
                //     } while (sample!=0x02);
                //     break;                

                /***************************************************************************/
                
                default:
                    sampleRight = sensorRead & 0x01;
                    stop = sensorRead & 0x10;

                    if (sampleRight == 0x1 && stop!=0x10)
                    {
                        //delay = 600
                        delay(600);
                        setVel2(45,-35);
                        delay(995);
                        rightFlag = 1;

                    }
                    else if (sensorRead == 0x1F)
                    {
                        delay(500);
                        stop = readLineSensors(0);
                        if (sensorRead == stop)
                        {
                            setVel2(0,0);
                            setServoPos(adeus);
                            if (adeus==15)
                            {
                                adeus = -15;
                            }
                            else
                            {
                                adeus = 15;
                            }
                        }

                    }        
                    break;
            }


        } while(!stopButton());
        setVel2(0, 0);
    }
}