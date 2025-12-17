//<C and C++ compiler>
//<Matthew Krumholz>
//<11/24/2025> 

//<discription of code> Lets a user change durration of sweep in microseconds how manytimes the sweep happens and the the start and end of the sweep. The sweep is a sine wave that is linear.
//<Why?> This will be used to control the peizo electric powersupply to release nanoparticles.
//<5> It works!
//<Board functions used?>  sweep_api, rp.h, uses the 125Mhz fr channels.

#include <stdio.h>
#include <unistd.h>
#include "rp.h"
#include "rp_sweep.h"
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           
using namespace rp_sweep_api;                                                                                                                                                                                                                                                                                                                                                                                                                                                              
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           
int main() {
    // Init the base generator API
    if (rp_Init() != RP_OK) {
        fprintf(stderr, "rp_Init failed\n");
        return -1;
    }                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           
    if (rp_SWInit() != RP_OK) {
        fprintf(stderr, "rp_SWInit failed\n");
        return -1;
    }                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      
    
    
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           
    float start_freq;  // 100 kHz
    float stop_freq;  // 300 kHz
    int   sweep_time;  // 100,000 us = 100 ms
    int   sweep_amount; // number of times the sweep happens
    float   amp_amount; //amplitude in volts_output when through both 20x amplifiers
    float RP_volt;
    float offset ;
    
VALUES:

    printf("ENTER start frequency in (Hz): ");
    scanf("%f", &start_freq);

    printf("ENTER end frequency in (Hz): ");
    scanf("%f", &stop_freq);

    while (1){
        printf("ENTER P-P final voltage in (V)");
        scanf("%f", &amp_amount);

        RP_volt = (amp_amount / 800.0);

        if (RP_volt <= .25f && amp_amount <= 200){
            break;
        }
        else{
            printf("ERROR: too large of a value, dont break the R_P, RE-ENTER P-P amplitude:");

        }

    }

    printf("ENTER sweep time in (us) : ");
    scanf("%d", &sweep_time);

    printf("Number of times to sweep : ");
    scanf("%d", &sweep_amount);
   
    offset = RP_volt;

PLAY:

    int play;

    char input[16];
    while (1) {
    printf("Offset is %f, R_P voltage is %f.\n", offset, RP_volt);
    printf("Press 1 to sweep and 2 to reset values, 3 to exit: \n");

    if (!fgets(input, sizeof(input), stdin)) {
        // fgets failed, try again
        continue;
    }

    // remove newline
    input[strcspn(input, "\n")] = 0;

    // try to parse integer
    if (sscanf(input, "%d", &play) != 1) {
        printf("Invalid input, please enter a number.\n");
        continue;
    }

    if (play == 1 || play == 2 || play == 3) {
        break;  // valid choice
    } else {
        printf("Choice must be 1, 2, or 3.\n");
    }
    }

    if(play == 2) {
        goto VALUES;
    }

    if (play == 3) {
        goto EXIT;    
    }
    if(play !=1){
        printf("error please re-enter choice. \n");
        goto PLAY;
    }

    rp_GenReset();
    rp_SWResetAll();                                                                                                                                                                                                                                                                                                                                                                                                                                                                       

    //set basic parameters
    rp_GenOffset(RP_CH_1, offset);
    rp_GenWaveform(RP_CH_1, RP_WAVEFORM_SINE);                                                                                                                                                                                                                                                                                                                                                                                                                                             
    rp_GenAmp(RP_CH_1, RP_volt);

    // Set sweep parameters
    rp_SWSetStartFreq(RP_CH_1, start_freq);                                                                                                                                                                                                                                                                                                                                                                                                                                                
    rp_SWSetStopFreq (RP_CH_1, stop_freq);                                                                                                                                                                                                                                                                                                                                                                                                                                                 
    rp_SWSetTime     (RP_CH_1, sweep_time);                                                                                                                                                                                                                                                                                                                                                                                                                                                
    rp_SWSetMode     (RP_CH_1, RP_GEN_SWEEP_MODE_LINEAR);                                                                                                                                                                                                                                                                                                                                                                                                                                  
    rp_SWSetDir      (RP_CH_1, RP_GEN_SWEEP_DIR_NORMAL);// just sweep upward 
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           
    // configure wave output.                                                                                                                                                                                                                                                                                                                                                                                                                                                                       
    
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           
    // Enable sweep for this channel
    rp_SWGenSweep(RP_CH_1, true);
    rp_GenOffset(RP_CH_1, offset);                                                                                                                                                                                                                                                                                                                                                                                                                                 
    rp_GenOutEnable(RP_CH_1);     
    //rp_GenTriggerOnly(RP_CH_1);
    
    for (int i = 0; i < sweep_amount; i++) {
    rp_SWRun();
    usleep(sweep_time);

    
}                                                                                                                                                                                                                                                                                                                                                                                                                                                            
    
    rp_SWGenSweep(RP_CH_1, false);
    //stop a1nd zero the output
    rp_GenAmp(RP_CH_1, 0.0f);
    rp_GenOffset(RP_CH_1, 0.0f);
    rp_GenWaveform(RP_CH_1, RP_WAVEFORM_DC); // DC 0V
    rp_SWStop();
    rp_GenOutDisable(RP_CH_1);  // disable after setting to DC 0V

    goto PLAY;
    
EXIT:

    rp_SWRelease();                                                                                                                                                                                                                                                                                                                                                                                                                                                                        
    rp_Release();         
    
    printf("program closed!");
    return 0;
}
