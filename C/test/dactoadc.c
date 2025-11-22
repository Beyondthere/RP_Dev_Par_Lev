#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "rp.h"

int main() {
    if (rp_Init() != RP_OK) return 1;

    // --- Configure DAC output ---
    rp_GenReset();                     // Reset generator
    rp_GenWaveform(RP_CH_1, RP_WAVEFORM_SINE); // Sine wave
    rp_GenFreq(RP_CH_1, 1000.0);      // 1 kHz
    rp_GenAmp(RP_CH_1, 0.5);          // 0.5 V amplitude
    rp_GenOffset(RP_CH_1, 0.0);       // 0 V offset
    rp_GenOutEnable(RP_CH_1);         // Enable DAC1 output

    // --- Configure ADC acquisition ---
    rp_AcqReset();                     // Reset ADC buffer
    rp_AcqSetDecimation(RP_DEC_64);   // Sample rate ~125 MSPS / 64
    rp_AcqStart();                     // Start acquisition
    usleep(100*1000);                  // Wait 100 ms to collect samples

    // --- Read ADC1 data ---
    uint32_t buff_size = 16384;
    float *data = malloc(buff_size * sizeof(float));
    uint32_t ret_count;
    rp_AcqGetOldestDataV(RP_CH_1, &ret_count, data);

    // --- Print first 20 samples ---
    printf("ADC1 samples (V):\n");
    for (uint32_t i = 0; i < 200 && i < ret_count; i++) {
        printf("%f\n", data[i]);
    }

    free(data);

    // --- Stop DAC ---
    rp_GenOutDisable(RP_CH_1);

    rp_Release();
    return 0;
}


