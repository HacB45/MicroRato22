#include "mr32.h"

// Compile and Run
// pcompile 404_explorer.c mr32.c && ldpic32 -w 404_explorer.hex -p /dev/ttyS3

int main(void){
    
    initPIC32();
    closedLoopControl( true );
    setVel2(0, 0);
    printf("404: Failure Not Found\n\tReady!\n\n");

    int sensorRead;
    int maxVel = 100, normalVel = 50, rotateVel = 40, rotateMinVel = 20;

    while(1)
    {
        printf("Press start to continue\n");
        while(!startButton())
        {
            waitTick80ms();
            sensorRead = readLineSensors(0);
            //printInt(sensorRead, 2 | 5 << 16);
            //printf("\n");
        }
        do
        {   
            waitTick80ms();
            sensorRead = readLineSensors(10);

            switch(sensorRead){
                //  ------ Extreme Warns ------ 
                case 0x00:          
                // Possible End of the road     00000
                    setVel2(rotateVel, -rotateVel);
                    break;
                case 0x1F: 
                // Possible Intercection        11111     
                    setVel2(rotateVel, -rotateVel);
                    break;

                //  ------ Go Ahead ------ 
                case 0x04:                      
                // Straight Line                00100 
                    setVel2(normalVel, normalVel);
                    break;
                case 0x06:                      
                // Straight Line                00110 
                    setVel2(normalVel, normalVel);
                    break;
                case 0x0C:                      
                // Straight Line                01100 
                    setVel2(normalVel, normalVel);
                    break;
                case 0x0E:  
                    // The velocity needs to be smaller than the max vel
                    setVel2(normalVel,normalVel);
                    break;

                //  ------ Left Side ------ 
                case 0x1C:                      
                // Left Side                    11100 
                    setVel2(-rotateVel, normalVel);
                    break;
                case 0x1E:                      
                // Left Side                    11110 
                    setVel2(-rotateVel, normalVel);
                    break;
                case 0x18:                      
                // Left Side                    11000 
                    setVel2(-rotateMinVel, normalVel);
                    break;
                case 0x10:                      
                // Left Side                    10000 
                    setVel2(-rotateMinVel, normalVel);
                    break;

                //  ------ Right Side ------ 
                case 0x07:                      
                // Left Side                    00111 
                    setVel2(normalVel, -rotateVel);
                    break;
                case 0x0F:                      
                // Left Side                    01111 
                    setVel2(normalVel, -rotateVel);
                    break;
                case 0x03:                      
                // Left Side                    00011 
                    setVel2(normalVel, -rotateMinVel);
                    break;
                case 0x01:                      
                // Left Side                    00001 
                    setVel2(normalVel, -rotateMinVel);
                    break;
            }

         
        }   while(!stopButton());
        setVel2(0, 0);
    }
    return 0;

}