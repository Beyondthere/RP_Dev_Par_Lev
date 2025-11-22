nclude <stdio.h>

#include <stdlib.h>

#include <math.h>

#include <unistd.h>

#include "rp.h"



#define SAMPLE_RATE 125000000  // 125 MS/s
			       //
			       // #define DURATION    0.1        // seconds
			       //
			       // #define AMPLITUDE   0.3        // Volts
			       //
			       // #define FREQ_START  100000     // 100 kHz
			       //
			       // #define FREQ_END    300000     // 300 kHz
			       //
			       //
			       //
			       // int main() {
			       //
			       //     if(rp_Init() != RP_OK){
			       //
			       //             fprintf(stderr, "Red Pitaya initialization failed!\n");
			       //
			       //                     return -1;
			       //
			       //                         }
			       //
			       //
			       //
			       //                             int samples = (int)(SAMPLE_RATE * DURATION);
			       //
			       //                                 float *waveform = (float*) malloc(sizeof(float) * samples);
			       //
			       //                                     if (!waveform){
			       //
			       //                                             fprintf(stderr, "Memory allocation failed\n");
			       //
			       //                                                     rp_Release();
			       //
			       //                                                             return -1;
			       //
			       //                                                                 }
			       //
			       //
			       //
			       //                                                                     // Generate linear frequency sweep (chirp)
			       //
			       //                                                                         for (int i = 0; i < samples; i++){
			       //
			       //                                                                                 double t = (double)i / SAMPLE_RATE; // current time
			       //
			       //                                                                                         double f = FREQ_START + (FREQ_END - FREQ_START) * (t / DURATION); // linear sweep
			       //
			       //                                                                                                 waveform[i] = AMPLITUDE * sin(2.0 * M_PI * f * t);
			       //
			       //                                                                                                     }
			       //
			       //
			       //
			       //                                                                                                         // Load waveform into channel 1
			       //
			       //                                                                                                             rp_GenReset();
			       //
			       //                                                                                                                 rp_GenWaveform(RP_CH_1, RP_WAVEFORM_ARBITRARY);
			       //
			       //                                                                                                                     rp_GenArbWaveform(RP_CH_1, waveform, samples);
			       //
			       //                                                                                                                         rp_GenAmp(RP_CH_1, AMPLITUDE);    // max amplitude
			       //
			       //                                                                                                                             rp_GenOffset(RP_CH_1, 0.0);       // zero offset
			       //
			       //                                                                                                                                 rp_GenMode(RP_CH_1, RP_GEN_MODE_CONTINUOUS);
			       //
			       //                                                                                                                                     rp_GenOutEnable(RP_CH_1);
			       //
			       //
			       //
			       //                                                                                                                                         // Wait for duration
			       //
			       //                                                                                                                                             usleep((int)(DURATION * 1e6));
			       //
			       //
			       //
			       //                                                                                                                                                 // Stop output
			       //
			       //                                                                                                                                                     rp_GenOutDisable(RP_CH_1);
			       //
			       //                                                                                                                                                         free(waveform);
			       //
			       //                                                                                                                                                             rp_Release();
			       //
			       //
			       //
			       //                                                                                                                                                                 printf("Chirp pulse generated successfully.\n");
			       //
			       //                                                                                                                                                                     return 0;
			       //
			       //                                                                                                                                                                     }
