#include <stdio.h>

#include <stdlib.h>

#include <stdbool.h>

#include "rp.h"



int main() {

	    if (rp_Init() != RP_OK) return -1;



	        rp_AcqSetGain(RP_CH_1, RP_LOW);

		    rp_AcqSetDecimation(RP_DEC_8);

		        rp_AcqReset();



			    rp_GenReset();

			        rp_GenWaveform(RP_CH_1, RP_WAVEFORM_ARBITRARY);

				    rp_GenOutEnable(RP_CH_1);



				        uint32_t buff_size;

					    rp_AcqGetBufSize(&buff_size);

					        float *in_buffer = malloc(buff_size * sizeof(float));

						    if (!in_buffer) {

							            rp_Release();

								            return -1;

									        }



						        while (1) {

								        bool filled = false;

									        rp_AcqStart();

										        rp_AcqSetTriggerSrc(RP_TRIG_SRC_NOW);

											        while (!filled)

													            rp_AcqGetBufferFillState(&filled);

												        rp_AcqGetDataV(RP_CH_1, 0, &buff_size, in_buffer);

													        rp_GenArbWaveform(RP_CH_1, in_buffer, buff_size);

														        rp_GenAmp(RP_CH_1, 1.0);

															        rp_GenFreq(RP_CH_1, 1000);

																    }



							    free(in_buffer);

							        rp_Release();

								    return 0;

}
