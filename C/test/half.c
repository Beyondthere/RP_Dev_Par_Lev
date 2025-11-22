#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "rp.h"

int main() {

    if (rp_Init() != RP_OK) {
        fprintf(stderr, "Failed to init Red Pitaya.\n");
        return 1;
    }

    // ========================================
    // Acquire from CH1
    // ========================================
    rp_AcqReset();
    rp_AcqSetDecimation(RP_DEC_64);
    rp_AcqSetTriggerLevel(RP_CH_1, 0.0);
    rp_AcqSetTriggerSrc(RP_TRIG_SRC_NOW);
    rp_AcqStart();

    usleep(200000);

    uint32_t N = 16384;
    float *data = malloc(N * sizeof(float));
    if (!data) return 1;

    rp_AcqGetOldestDataV(RP_CH_1, &N, data);

    // ========================================
    // Manipulate the signal: scale by 0.5
    // ========================================
    for (uint32_t i = 0; i < N; i++) {
        data[i] *= 0.5f;
        // Optional: clip to DAC range [-1, +1]
        if (data[i] > 1.0f) data[i] = 1.0f;
        if (data[i] < -1.0f) data[i] = -1.0f;
    }

    // ========================================
    // Output to OUT1 using ARBITRARY WAVEFORM
    // ========================================
    rp_GenReset();
    rp_GenWaveform(RP_CH_1, RP_WAVEFORM_ARBITRARY);
    rp_GenArbWaveform(RP_CH_1, data, N);
    rp_GenFreq(RP_CH_1, 119.0);  // Playback frequency (Hz)
    rp_GenAmp(RP_CH_1, 1.0);    // DAC amplitude scaling
    rp_GenOutEnable(RP_CH_1);

    printf("Outputting processed signal to OUT1...\n");

    sleep(10); // keep running so output stays active

    rp_GenOutDisable(RP_CH_1);
    rp_Release();
    free(data);
    return 0;
}

