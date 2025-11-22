#include <stdio.h>

#include <stdlib.h>

#include <math.h>

#include <unistd.h>

#include "rp.h"



#define SAMPLE_RATE 125000000

#define DURATION    0.1

#define AMPLITUDE   0.3

#define FREQ_START  100000

#define FREQ_END    300000



int main() {

	    if(rp_Init() != RP_OK) return -1;



	        int samples = (int)(SAMPLE_RATE * DURATION);

		    float *waveform = (float*) malloc(sizeof(float) * samples);

		        if (!waveform){

				        rp_Release();

					        return -1;

						    }



			    for (int i = 0; i < samples; i++){

				            double t = (double)i / SAMPLE_RATE;

					            double f = FREQ_START + (FREQ_END - FREQ_START) * (t / DURATION);

						            waveform[i] = AMPLITUDE * sin(2.0 * M_PI * f * t);

							        }



			        rp_GenReset();

				    rp_GenWaveform(RP_CH_1, RP_WAVEFORM_ARBITRARY);

				        rp_GenArbWaveform(RP_CH_1, waveform, samples);

					    rp_GenAmp(RP_CH_1, AMPLITUDE);

					        rp_GenOffset(RP_CH_1, 0.0);

						    rp_GenMode(RP_CH_1, RP_GEN_MODE_CONTINUOUS);

						        rp_GenOutEnable(RP_CH_1);



							    usleep((int)(DURATION * 1e6));



							        rp_GenOutDisable(RP_CH_1);

								    free(waveform);

								        rp_Release();



									    return 0;

}
