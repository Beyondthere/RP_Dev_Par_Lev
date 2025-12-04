#include <stdio.h>
/* Red Pitaya C API example of Continuous generation on a specific channel */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

#include "rp.h"

int main(int argc, char **argv){

    /* Print error, if rp_Init() function failed */
    if(rp_Init() != RP_OK){
        fprintf(stderr, "Rp api init failed!\n");
    }

    /* Reset generator */
    rp_GenReset();

    /* Generation */
    rp_GenWaveform(RP_CH_2, RP_WAVEFORM_SINE);    // Waveform
    rp_GenFreq(RP_CH_2, 2000.0);                  // Frequency
    rp_GenAmp(RP_CH_2, 1.0);                      // Amplitude

    // Emable channel output
    rp_GenOutEnable(RP_CH_2);

    // Trigger generation
    rp_GenTriggerOnly(RP_CH_2);

    /* Releasing resources */
    rp_Release();

    return 0;
}