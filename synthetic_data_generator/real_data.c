
// Eighth order

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <omp.h>
#include "boundary_cond.h"
#include "forward_field.h"
#include "data_collect_modi.h"

int main(int argc, void **argv)
{
    if (argc < 6)
    {
        printf("Usage: excecutable <parameter> <source_amp> <real_model> <source_pos> <receiver_pos>\n");
        exit(-1);
    }
    char *parameter = argv[1];
    char *source_amp = argv[2];
    char *real_model = argv[3]; 
    char *source_pos = argv[4];
    char *receiver_pos = argv[5];
    /******************************************** read parameters from parameter file ****************************************************/
    /******************************************** read parameters from parameter file ****************************************************/
    char ts[100];
    long long int ns, grid_x, grid_y, src_x, rp, n_src, n_rcr, thread, maxitr, boundary_key, absorb_layer, obj_fun, upper_avoid, bound_avoid, data_order;
    float dt, dx;
    FILE *para;
    para = fopen(parameter, "r"); 
    fscanf(para, "%s %lld %s %f %s %lld %s %lld %s %f %s %lld %s %lld %s %lld %s %lld %s %lld %s %lld %s %lld %s %lld %s %lld %s %lld %s %lld %s %lld", ts, &ns, ts, &dt, ts, &grid_x, ts, &grid_y, ts, &dx, ts, &src_x, ts, &rp, ts, &n_src, ts, &n_rcr, ts, &thread, ts, &maxitr, ts, &boundary_key, ts, &absorb_layer, ts, &obj_fun, ts, &upper_avoid, ts, &bound_avoid, ts, &data_order);
    fclose(para);
    /*************************************************************************************************************************************/
    /***************** print number of source and recivers ******************/
    printf("Number of sources are: %lld\nNumber of receivers for each shot are: %lld\n", n_src, n_rcr);
    /************************************************************************/
    long long int n_g_s = n_src / thread;
    float *observe_data;
    observe_data = (float *)calloc((thread * ns * n_rcr), sizeof(float));

    float *src_term;
    src_term = (float *)calloc((ns), sizeof(float));

    float *gather_real;
    gather_real = (float *)calloc((thread * ns * grid_y), sizeof(float));

    float *vel_real;
    vel_real = (float *)calloc((grid_x * grid_y), sizeof(float));


    float *gather_vec;
    gather_vec = (float *)calloc((thread * ns * grid_x * grid_y), sizeof(float));

    FILE *read_source;
    read_source = fopen(source_amp, "r");
    for (long long int i = 0; i < ns; i++)
    {
        fscanf(read_source, "%f", &src_term[i]);
    }
    fclose(read_source);

    FILE *read_real;
    read_real = fopen(real_model, "r");
    for (long long int i = 0; i < grid_x; i++)
    {
        for (long long int j = 0; j < grid_y; j++)
        {
            fscanf(read_real, "%f", &vel_real[grid_y * i + j]);
        }
    }
    fclose(read_real);

    omp_set_num_threads(thread);
    FILE *real_data_write;
    real_data_write = fopen("../input/observe.txt", "w");


    long long int *src_pos;
    src_pos = (long long int *)calloc(n_src, sizeof(long long int));
    FILE *read_source_pos;
    read_source_pos = fopen(source_pos, "r");
    for(long long int i = 0; i < n_src; i++)
    {
	    fscanf(read_source_pos, "%lld", &src_pos[i]);
    }
    fclose(read_source_pos);

    long long int *rcr_position_collect;
    rcr_position_collect = (long long int *)calloc(n_src * n_rcr, sizeof(long long int));
    FILE *read_receiver_pos;
    read_receiver_pos = fopen(receiver_pos, "r");
    for(long long int i = 0; i < n_src * n_rcr; i++)
    {   
	    fscanf(read_receiver_pos, "%lld", &rcr_position_collect[i]);
        printf("%lld,", rcr_position_collect[i]);
    }
printf("\n");
fclose(read_receiver_pos);

    long long int src_y[thread];
    for (long long int n = 0; n < n_g_s; n++)
    {
#pragma omp parallel
        {
            long long int tid;
            tid = omp_get_thread_num();

            src_y[tid] = src_pos[tid + thread * (n)];

            // *********** REAL GATHER ************** //

            /*********************************** forward modeling of wave ********************************/
            //  forward_field(source_depth_position, source_position_on_surface,                         //
            //  depth_of_receiver, temporal_sampling, spatial_sampling,                                  //
            //  source_signature(array), velocity_model, field_vector(array), no_samples, depth_model,   //
            //  length_model, thread_ID, boundary_key, grids_reserved_boundary, gather_for_whole_model); //
            forward_field(src_x, src_y, rp, dt, dx, src_term, vel_real, gather_vec, ns, grid_x, grid_y, tid, boundary_key, absorb_layer, gather_real);
            /*********************************************************************************************/   
            
            data_collect_modi(tid, ns, grid_y, thread, gather_real, observe_data, rcr_position_collect, n, n_src, n_rcr);
#pragma omp barrier
        }
        for (long long int i = 0; i < (thread * ns * n_rcr); i++)
        {
            fprintf(real_data_write, "%0.50f\n", observe_data[i]);
        }
        

    }
    fclose(real_data_write);

    free(src_term);
    free(gather_real);
    free(vel_real);
    free(gather_vec);
    return 0;
}
