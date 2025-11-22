#include <stdio.h>

#include <stdlib.h>

#include <unistd.h>

#include <math.h>

#include "rp.h" // red pitaya API



// defines buffer size, provided by red pitaya

#define BUFF_SIZE 16384



int main(int argc, char **argv) {



    // buffers to allocate data

    float *adc_buf = (float *)malloc(BUFF_SIZE * sizeof(float));

    float *dac_buf = (float *)malloc(BUFF_SIZE * sizeof(float));

    

    // red pitaya API initializes

    rp_Init();



    printf("Program initialized.\n");



    // acquisition configuration (IN)

    rp_AcqReset(); 

    rp_AcqSetDecimation(RP_DEC_1);

    rp_AcqSetTriggerDelay(0);



    // signal generator configuration (OUT)

    rp_GenReset();

    rp_GenWaveform(RP_CH_1, RP_WAVEFORM_SINE);

    rp_GenAmp(RP_CH_1, 1.0);

    rp_GenFreq(RP_CH_1, 7630.0);

    rp_GenOutEnable(RP_CH_1);



    float cumulative_factor = 1.0; // defines comulative amplification factor



    // begins loop

    while (1) {

       

        float current_factor; // defines current amplification factor

        printf("\nEnter modification factor (enter 0 to reset amplification, or -1 to end program:");

        scanf("%f", &current_factor);



        // if statement for amplitude reset

        if (current_factor == 0) {

            cumulative_factor = 1.0;

            printf("Amplitude has been reset");

        } else if (current_factor  == -1){ // else if program for loop break

            break;

        } else { // else statement for modifying amplification factor

            cumulative_factor = cumulative_factor * current_factor;

            printf("Amplitude has been modified by %.4f\n", cumulative_factor);

        }



        // data acquisition

        rp_AcqStart();

        rp_AcqSetTriggerSrc(RP_TRIG_SRC_CHA_PE); // data acquisition is triggered at rising edge

        rp_acq_trig_state_t state = RP_TRIG_STATE_TRIGGERED;



        // edit filter strength

        float alpha = 0.025; // lower = more filter, higher = less filter (up to 1.0)



        // program waits for trigger

        while (1) {

            rp_AcqGetTriggerState(&state);

            if (state == RP_TRIG_STATE_TRIGGERED) {

                break;

            }

        }



        // stores acquired data in adc_buf

        uint32_t buf_size = BUFF_SIZE;

        rp_AcqGetOldestDataV(RP_CH_1, &buf_size, adc_buf);



        // low-pass filter

        dac_buf[0] = adc_buf[0]; // filter starting point

        for (int i = 1; i < BUFF_SIZE; i++) { // for loop for filter smoothing out

            dac_buf[i] = dac_buf[i-1] + alpha * (adc_buf[i] - dac_buf[i-1]);

        }

	printf("\nbefore%f", *dac_buf);

        // modifies amplification

        for (int i = 0; i < BUFF_SIZE; i++) {

            dac_buf[i] = dac_buf[i] * cumulative_factor;

        }

	printf("\nafter %f", *dac_buf);


        // data generation

        rp_GenArbWaveform(RP_CH_1, dac_buf, BUFF_SIZE); // loads dac_buff into output

        rp_GenTriggerOnly(RP_CH_1); // trigers output

    }



    // program cleanup (disables output, releases API, frees memory)

    rp_GenOutDisable(RP_CH_1);

    rp_Release();

    free(adc_buf);

    free(dac_buf);



    printf("Program complete.\n");



    return 0;

}
