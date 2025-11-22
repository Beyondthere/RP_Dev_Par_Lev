#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "rp.h"
#include "rp_acq.h"

// Define the buffer size (must be 16384 for the Red Pitaya)
#define BUFF_SIZE 16384

int main(int argc, char **argv) {

    // Buffers to hold the data
    float *adc_buf = (float *)malloc(BUFF_SIZE * sizeof(float));
    float *dac_buf = (float *)malloc(BUFF_SIZE * sizeof(float));
    if (!adc_buf || !dac_buf) return -1;

    if (rp_Init() != RP_OK) {
        fprintf(stderr, "Failed to initialize Red Pitaya API!\n");
        return -1;
    }


    double f_DAC_s = 125e6;
    double buff_time = BUFF_SIZE / f_DAC_s;
    
    rp_AcqReset();
    rp_AcqSetDecimation(RP_DEC_1); // Set decimation to 1 (125 MS/s)
    rp_AcqSetTriggerDelay(0);      // No delay after trigger

    
    rp_GenReset();
    rp_GenOutEnable(RP_CH_1);                      // Enable OUT1
    rp_GenWaveform(RP_CH_1, RP_WAVEFORM_ARBITRARY); // Set mode to arbitrayr
    rp_GenAmp(RP_CH_1, 1.0);
    rp_GenFreq(RP_CH_1, f_DAC_s  / BUFF_SIZE);         

    printf("Starting real-time filter loop. Press Ctrl+C to stop.\n");

    
    while (1) {
        
        
        rp_AcqReset();
	rp_AcqSetTriggerSrc(RP_TRIG_SRC_CHA_PE);
	rp_AcqStart();


	rp_acq_trig_state_t trig_state;
	do{
		usleep(1000);
		rp_AcqGetTriggerState(&trig_state);
	} while(trig_state != RP_TRIG_STATE_TRIGGERED);	
        


        // Get the acquired data
        uint32_t buf_size = BUFF_SIZE;
        rp_AcqGetOldestDataV(RP_CH_1, &buf_size, adc_buf);

        
        
        // This is the filter coefficient.
        // A smaller value means a *lower* cutoff frequency (more filtering).
        // A larger value (up to 1.0) means a *higher* cutoff (less filtering).
        // Start with 0.05 and experiment!
        float alpha = 0.05; 

        // Apply the low-pass filter (Exponential Moving Average)
        dac_buf[0] = adc_buf[0]; // Set the first sample
        for (int i = 1; i < BUFF_SIZE; i++) {
            dac_buf[i] = dac_buf[i-1] + alpha * (adc_buf[i] - dac_buf[i-1]);
        }

	for (int i=0; i  < BUFF_SIZE; i++){
	       dac_buf[i] = dac_buf[i] * .5;
	}	       

       
        
        // Load the processed buffer into the Arbitrary Waveform Generator
        rp_GenArbWaveform(RP_CH_1, dac_buf, BUFF_SIZE);
 
	rp_GenTriggerOnly(RP_CH_1);

	usleep((int)(buff_time * 1e6));
 }

    
    rp_GenOutDisable(RP_CH_1);
    rp_Release();
    free(adc_buf);
    free(dac_buf);

    return 0;
}
