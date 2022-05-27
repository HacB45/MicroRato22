
#include "mr32.h"

int main(void){
    
    initPIC32();
    closedLoopControl( true );
    setVel2(0, 0);
    printf("404: Failure Not Found\n\tReady!\n\n");

    int sensorRead;
    int maxVel = 60, normalVel = 50, rotateVel = 40, rotateMinVel = 20;

    while(1)
    {
        while(!startButton())
        {
            waitTick80ms();
            sensorRead = readLineSensors(0);
        }

        do{
            waitTick40ms();
            sensorRead = readLineSensors(0);

            printInt(sensorRead, 2 | 5 << 16);
            printf("\n");


            switch(sensorRead)
            {
                /**
                 * @brief Follow the straight line
                 * 
                 */

                case 0x0E:  // 01110:
                    // The velocity needs to be smaller than the max vel
                    setVel2(maxVel,maxVel);
                    break;

                case 0x0C:  // 01100:
                    setVel2(normalVel, maxVel);                   
                    break; 

                case 0x06:  // 00110:
                    setVel2(maxVel, normalVel);
                    break;

                //-----------------------------------------------------------------------------

                /**
                 * @brief Right Curve
                 * 
                 */

            
                case 0x0F:  // 01111:
                    delay(400);
                    sensorRead = readLineSensors(0);
                    if (sensorRead == 0x00) //
                    {
                        setVel2(maxVel, -rotateVel);
                        delay(1300);
                    }
                    
                    break;

                case 0x07:  // 00111:
                    delay(400);
                    sensorRead = readLineSensors(0);
                    if (sensorRead == 0x00)
                    {
                        setVel2(maxVel, -rotateVel);
                        delay(1300);
                    }
                    
                    break;

                
                case 0x03:  // 00011:
                    delay(400);
                    sensorRead = readLineSensors(0);
                    if (sensorRead == 0x00)
                    {
                        setVel2(maxVel, -rotateVel);
                        delay(1300);
                    }
                    
                    break;

                
                
                
                default:
                    break;
            }


        } while(!stopButton());
        setVel2(0, 0);
    }
}