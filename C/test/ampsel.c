#include <stdio.h>

#include <stdlib.h>

#include <unistd.h>

#include <math.h>

#include "rp.h"

                                                                                                                                                                                                                   

//(must be 16384 for the Red Pitaya)

#define BUFF_SIZE 16384
#define ADC_RATE 125000000.0
#define AWG_NATIVE_FREQ (ADC_RATE / BUFF_SIZE)
                                                                                                                                                                                                                   

int main(int argc, char **argv) {

    //startup                                                                                                                                                                                                               

    // adc and dac buffers to hold the digital data

    float *adc_buf = (float *)malloc(BUFF_SIZE * sizeof(float));

    float *dac_buf = (float *)malloc(BUFF_SIZE * sizeof(float));                                                                                                                                                                                                       

    if (rp_Init() != RP_OK) {

        fprintf(stderr, "The RP is not plugged in or other error.\n"); //displays error if RP is not conmnected

        return 1;

    }                                                                                                                                                                                                              

                                                                                                                                                                                                                   

    //loads the adc from in1 and sets the propper settings

    rp_AcqReset();                                                                                                                                                                                                 

    rp_AcqSetDecimation(RP_DEC_1); // Set decimation to 1 (125 MS/s) this is the full rate the in1 can read

    rp_AcqSetTriggerSrc(RP_TRIG_SRC_AWG_NE);

    rp_AcqStart();      // starts reading data now!

    

    rp_GenReset();                                                                                                                                                                                                 

    rp_GenOutEnable(RP_CH_1);                      // Enable OUT1

    rp_GenWaveform(RP_CH_1, RP_WAVEFORM_ARBITRARY); // Set mode to arbitrary so we can control output with input

	rp_GenFreq(RP_CH_1, AWG_NATIVE_FREQ);


    rp_GenAmp(RP_CH_1, 1.0);                        // Set output amplitude to 1V                                                                                                                                                                                                                   



    // (125,000,000 samples/sec) / (16384 samples/buffer) = 7630 Hz 

    //due to the adc frequency we want this to be the same change buff_size to change this



    rp_GenTriggerSource(RP_CH_1, RP_GEN_TRIG_SRC_INTERNAL);




    printf("Starting amplitude ratio. Press Ctrl+C to stop.\n");     

    

    for(int i = 0; i < BUFF_SIZE; i++) dac_buf[i] = 0;

    rp_GenArbWaveform(RP_CH_1, dac_buf, BUFF_SIZE);

   rp_GenOutEnable(RP_CH_1);
    rp_GenTriggerOnly(RP_CH_1);




    

    // loop will add control

    while (1) {



                                                                                                                                                                                                                   

        // Get the acquired data latest since its contious this is important

        uint32_t buf_size = BUFF_SIZE;

        rp_AcqGetLatestDataV(RP_CH_1, &buf_size, adc_buf);                                                                                                                                                         

                                                                                                                                                                                                                   

        // Filtering and ratio generation                                                                                                                                                                                                                   

        // This is the filter coefficient. helps with nosie

        float alpha = 0.05;                                                                                                                                                                                                                

        dac_buf[0] = adc_buf[0]; // Set the first sample

        for (int i = 1; i < BUFF_SIZE; i++) { // Apply the low-pass filter

            dac_buf[i] = dac_buf[i-1] + alpha * (adc_buf[i] - dac_buf[i-1]);

        }

                                                                                                                                                                                                                   

        for (int i=0; i  < BUFF_SIZE; i++){

               dac_buf[i] = dac_buf[i] * 3.0;
	       if (dac_buf[i] > 1.0)  dac_buf[i] = 1.0; 
   	       if (dac_buf[i] < -1.0) dac_buf[i] = -1.0;
        }                                                                                                                                                                                                          

                                                                                                                                                                                                                   

        //output to out1                                                                                                                                                                                                         

        // Load the processed buffer into the Arbitrary Waveform Generator

        rp_GenArbWaveform(RP_CH_1, dac_buf, BUFF_SIZE);                                                                                                                                                                             

                                                                                                                                                                                  

    }

                                                                                                                                                                                                                   

    // need to add code to read this

    rp_GenOutDisable(RP_CH_1);                                                                                                                                                                                     

    rp_Release();                                                                                                                                                                                                  

    free(adc_buf);                                                                                                                                                                                                 

    free(dac_buf);                                                                                                                                                                                                 

                                                                                                                                                                                                                   

    return 0;

}
