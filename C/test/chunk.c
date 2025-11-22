#include <stdio.h>

#include <stdlib.h>

#include <unistd.h>

#include "rp.h"



#define BUFFER_SIZE 1024



int main() {

	    if (rp_Init() != RP_OK) return -1;



	        float *buffer = malloc(BUFFER_SIZE * sizeof(float));

		    if (!buffer) return -1;



		        rp_AcqReset();

			    rp_AcqSetDecimation(RP_DEC_1);

			        rp_AcqSetTriggerSrc(RP_TRIG_SRC_DISABLED);

				    rp_AcqStart();



				        rp_GenReset();

					    rp_GenOutEnable(RP_CH_1);

					        rp_GenWaveform(RP_CH_1, RP_WAVEFORM_ARBITRARY);

						    rp_GenAmp(RP_CH_1, 1.0);



						        float alpha = 0.05;

							    float last = 0;



							        while (1) {

									        uint32_t size = BUFFER_SIZE;

										        rp_AcqGetLatestDataV(RP_CH_1, &size, buffer);



											        for (int i = 0; i < BUFFER_SIZE; i++) {

													            last = last + alpha * (buffer[i] - last);

														                buffer[i] = last * 0.5;

																        }



												        rp_GenArbWaveform(RP_CH_1, buffer, BUFFER_SIZE);

													        rp_GenTriggerOnly(RP_CH_1);

														    }



								    rp_GenOutDisable(RP_CH_1);

								        rp_Release();

									    free(buffer);

									        return 0;

}
