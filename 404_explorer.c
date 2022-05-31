#include "mr32.h"

// Compile and Run
// pcompile 404_explorer.c mr32.c && ldpic32 -w 404_explorer.hex -p /dev/ttyS3

/**
 * The function takes in a direction and a velocity and turns the robot in the direction specified by
 * the direction parameter.
 * 
 * @param direction The direction the robot is turning.
 * @param rotateVel The speed at which the robot will rotate.
 */
void turnDir(char direction, int rotateVel)
{
    switch(direction)
    {
        case 'S':
        /* The robot is going straight. */
            break;
        case 'L':
        /* Turning the robot to the left. */
            setVel2(-rotateVel, rotateVel);
            delay(2500);
            break;
        case 'R':
        /* Turning the robot to the right. */
            setVel2(rotateVel, -rotateVel);
            delay(2500);
            break;
        case 'O':
        /* Turning the robot around. */
            setVel2(rotateVel, -rotateVel);
            delay(5000);
            break;     
    }  
}

/**
 * It takes an integer, converts it to binary, and returns an array of the binary digits
 * 
 * @param sensorVal the value of the sensor
 * 
 * @return The address of the first element of the array.
 */
int *sensorBinary(int sensorVal)
{
    int i, j, bits_all;
    int bit[5];
    int bits = 0;

    for(i = 0; i < 5; i++){
      bit[i] = (sensorVal >> i) & 0x01;
    }
    bits = (bit[4] << 4) | (bit[3] << 3) | (bit[2] << 2) | (bit[1] << 1) | (bit[0] << 0);
    printInt(bits, 2 | 5 << 16);
    putChar('\n');
    return bit;
}

int main(void){
    
    /* Initializing the robot. */
    initPIC32();
    closedLoopControl( true );
    setVel2(0, 0);
    printf("404: Failure Not Found\n\tReady!\n\n");

    /* Declaring the variables that will be used in the program. */
    int sensorRead;
    int maxVel = 100, normalVel = 50, normalMinVel = 20, rotateVel = 40, rotateMinVel = 30;
    int *sensorBin;
    int straight = 0, right = 0, left = 0, deadEnd = 0;
    char direction;
    //unsigned int sensorsVal[10];
        
    while(1)
    {
        printf("Press start to continue\n");
        while(!startButton())
        {
            /* Reading the line sensors and printing the value. */
            waitTick80ms();
            sensorRead = readLineSensors(0);
            printInt(sensorRead, 2 | 5 << 16);
            printf("\n");
        }
        do
        {   
            setVel2(normalVel,normalVel);
            waitTick80ms();
            sensorRead = readLineSensors(0);
            /* The 'waitTick80ms()' function waits for 80ms and the 'readLineSensors(0)' function
            takes 50ms to read the sensors. So, the total time is 130ms. */

            sensorBin = sensorBinary(sensorRead);
            //printInt(sensorRead, 2 | 5 << 16);
            /* Checking if the robot is at a dead end, a right turn, or a left turn. */
            if((sensorRead) == 0x00){
                /* Dead end reached 
                        X000X
                */
                deadEnd = 1;
                printStr("DeadEnd\n");
            }
            else if(sensorRead = 0x01){
                /* Right Turn reached 
                        XXXX1          
                */
                right = 1;
                printStr("Right\n");
            }
            else if(sensorRead = 0x10){
                /* Left Turn reached 
                        1XXXX          
                */
                left = 1;
                printStr("Left\n");
            }
            

            if(sensorRead == 0x11){
                /* Intersection reached 
                        1XXX1          
                */
                right = 1; 
                printStr("Intersect\n");               
                // TODO
            }
            else if(sensorBin[3] == 1 || sensorBin[2] == 1 || sensorBin[1] == 1){
                /* Straight Line Ahead 
                        X111X          
                */
                straight = 1;
                printStr("Straight\n");
            }

           
            /* Checking if the robot is at a dead end, a right turn, or a left turn. */
            if(right == 1)
                direction = 'R';
            else if(straight == 1)
                direction = 'S';
            else if(left == 1)
                direction = 'L';
            else if(deadEnd == 1)
                direction = 'O';
            
            
            turnDir(direction,rotateVel);

            /* switch(sensorRead){
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
            } */

        }   while(!stopButton());
        setVel2(0, 0);
    }
    return 0;

}