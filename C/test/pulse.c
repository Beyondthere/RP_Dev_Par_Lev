#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


#include "rp.h"
#include "rp_sweep.h"

using namespace rp_sweep_api;

int main(int argc, char **argv){


if(rp_Init() != RP_OK){
	fprintf(stderr, "Rp api init failed!\n");
}

float start_freq = 100000; //in hertz
float end_freq = 300000; //in hertz
float sweep_time = 100000; //in micro seconds

rp_GenReset();


rp_GenSweepStartFreq(RP_CH_1, start_freq);
rp_GenSweepEndFreq(RP_CH_1, end_freq);
rp_SWSetTime(RP_CH_1, sweep_time);


rp_sw_SetMode(RP_CH_1, RP_GEN_SWEEP_MODE_LINEAR);


rp_GenAmp(RP_CH_1, .3);
rp_GenOutEnable(RP_CH_1);


usleep(sweep_time); //sleep in micro seconds
rp_GenOutDisable(RP_CH_1);

rp_Release();
return 0;

}
