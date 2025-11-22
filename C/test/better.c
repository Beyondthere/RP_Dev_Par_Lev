#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "rp.h"

// Define the buffer size (must be 16384 for the Red Pitaya)
#define BUFF_SIZE 16384

int main(int argc, char **argv) {

    // Buffers to hold the data
    float *adc_buf = (float *)malloc(BUFF_SIZE * sizeof(float));
    float *dac_buf = (float *)malloc(BUFF_SIZE * sizeof(float));

    // ======== 1. INITIALIZE THE RED PITAYA ========
    if (rp_Init() != RP_OK) {
        fprintf(stderr, "Failed to initialize Red Pitaya API!\n");
        return 1;
    }

    // ======== 2. CONFIGURE ACQUISITION (INPUT) ========
    rp_AcqReset();
    rp_AcqSetDecimation(RP_DEC_1); // Set decimation to 1 (125 MS/s)
    rp_AcqSetTriggerDelay(0);      // No delay after trigger

    // ======== 3. CONFIGURE GENERATION (OUTPUT) ========
    rp_GenReset();
    rp_GenOutEnable(RP_CH_1);                      // Enable OUT1
    rp_GenWaveform(RP_CH_1, RP_WAVEFORM_ARBITRARY); // Set mode to arbitrayr
    rp_GenAmp(RP_CH_1, 1.0);
    rp_GenFreq(RP_CH_1, 12e6 / BUFF_SIZE);         

    printf("Starting real-time filter loop. Press Ctrl+C to stop.\n");

    // ======== 4. START THE PROCESSING LOOP =======
    while (1) {
        
        // ---- A. ACQUIRE DATA ----
        rp_AcqStart();
        // Trigger on the positive edge of CH1 (IN1)
        rp_AcqSetTriggerSrc(RP_TRIG_SRC_NOW); 
        
        bool filled = false;
		rp_AcqGetBufferFillState(&filled);


        // Get the acquired data
        uint32_t buf_size = BUFF_SIZE;
        rp_AcqGetOldestDataV(RP_CH_1, &buf_size, adc_buf);

        // ---- B. PROCESS DATA (THE FILTER) ----
        
        // This is the filter coefficient.
        // A smaller value means a *lower* cutoff frequency (more filtering).
        // A larger value (up to 1.0) means a *higher* cutoff (less filtering).
        // Start with 0.05 and experiment!
        float alpha = 0.025; 

        // Apply the low-pass filter (Exponential Moving Average)
        dac_buf[0] = adc_buf[0]; // Set the first sample
        for (int i = 1; i < BUFF_SIZE; i++) {
            dac_buf[i] = dac_buf[i-1] + alpha * (adc_buf[i] - dac_buf[i-1]);
        }

	for (int i=0; i  < BUFF_SIZE; i++){
	       dac_buf[i] = dac_buf[i] * .5;
	}	       

        // ---- C. GENERATE DATA ----
        
        // Load the processed buffer into the Arbitrary Waveform Generator
        rp_GenArbWaveform(RP_CH_1, dac_buf, BUFF_SIZE);
        
        // Trigger the generator to output the buffer once
        rp_GenTriggerOnly(RP_CH_1);
    }

    // ======== 5. CLEANUP (never reached in this loop) ========
    rp_GenOutDisable(RP_CH_1);
    rp_Release();
    free(adc_buf);
    free(dac_buf);

    return 0;
}
