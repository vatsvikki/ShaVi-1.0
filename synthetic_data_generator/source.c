#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "source_ricker.h"
int main(int argc, void **argv)
{
	
    if (argc < 3)
    {
    	printf("Usage: excecutable <parameter> <frequency>");
    	exit(-1);
    }
    float freq = atof(argv[2]);
    char *parameter = argv[1];
	  /******************************************** read parameters from parameter file ****************************************************/
    char ts[100];
    int ns, grid_x, grid_y, src_x, rp, n_src, n_rcr, thread, maxitr, boundary_key, absorb_layer, obj_fun, upper_avoid, bound_avoid, data_order;
    float dt, dx;
    FILE *para;
    para = fopen(parameter, "r"); 
    fscanf(para, "%s %d %s %f %s %d %s %d %s %f %s %d %s %d %s %d %s %d %s %d %s %d %s %d %s %d %s %d %s %d %s %d %s %d", ts, &ns, ts, &dt, ts, &grid_x, ts, &grid_y, ts, &dx, ts, &src_x, ts, &rp, ts, &n_src, ts, &n_rcr, ts, &thread, ts, &maxitr, ts, &boundary_key, ts, &absorb_layer, ts, &obj_fun, ts, &upper_avoid, ts, &bound_avoid, ts, &data_order);
    fclose(para);

      

	float *src_amp;
	src_amp = (float *)calloc((ns), sizeof(float));

	src(dt, freq, src_amp, ns);

	FILE *source;
	char source_file[64];
	sprintf(source_file, "../input/source_amp_%dHz.txt", (int)freq);
	source = fopen(source_file, "w");
	//source = fopen(source, "w");
	for (int i = 0; i < ns; i++)
	{
		fprintf(source, "%0.50f\n", src_amp[i]);
	}
	fclose(source);
	return 0;
}
