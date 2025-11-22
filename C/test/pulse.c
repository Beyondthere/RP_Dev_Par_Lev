#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


#include "rp.h"

int main(int argc, char **argv){


if(rp_Init() != RP_OK){
	fprintf(stderr, "Rp api init failed!\n");
}

float start_freq = 100000;
float end_freq = 300000;
float sweep_time = 0.1;

rp_GenReset();


rp_GenSweepStartFreq(RP_CH_1, start_freq);
rp_GenSweepEndFreq(RP_CH_1, end_freq);
rp_GenSweepTime(RP_CH_1, sweep_time);
rp_GenSweepTime(RP_CH_1, RP_SWEEP_TYPE_LINEAR);

rp_GenMode(RP_CH_1, RP_GEN_MODE_SWEEP);


rp_GenAmp(RP_CH_1, .3);
rp_GenOutEnable(RP_CH_1);


sleep(sweep_time);
rp_GenOutDisable(RP_CH_1);

rp_Release();
return 0;

}
