#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
/********************************************************************/
//  boundary_cond.h                                                 //
//  header to calculate boundary coefficients, here IHABC is used.  //
// Reference: An improved hybrid absorbing boundary condition       //
// for wave equation modeling. (Yang Liu, et al., 2018)             //
/********************************************************************/
#include "boundary_cond.h"
/*********************************************************************************************/
//  forward_field.h                                                                          //
//  header for forward field calculation using 6th order finite differnce method             //                                                                 
//  forward_field(source_depth_position, source_position_on_surface,                         //
//  depth_of_receiver, temporal_sampling, spatial_sampling,                                  //
//  source_signature(array), velocity_model, field_vector(array), no_samples, depth_model,   //
//  length_model, thread_ID, boundary_key, grids_reserved_boundary, gather_for_whole_model); //
/*********************************************************************************************/
#include "forward_field.h"
/********************************************************************************************/
//  backward_field.h                                                                        //
//  header for backward field calculation using 6th order finite differnce method           //
//  the only difference backward and forward header us source location in backward          //
//  field the source location is same as receiver position and source signature is          //  
//  time fliped residual.                                                                   //
//  backward_field(backward_source_depth_position(same as receiver),                        //
//  receiver_position_on_surface (for back propagation),                                    //
//  temporal_sampling, spatial_sampling, flip_residual, velocity_model                      //
//  back_field_vector, no_samples, depth_model, length_model, thread_ID, boundary_key       //
//  grids_reserved_boundary, );                                                             //        
/********************************************************************************************/
#include "backward_field.h"
/*********************************************************/
//  largest.h                                            //
//  header to find the largest real number in a vector   //
//  largest(array, length of model, width of model)      //
/*********************************************************/
#include "largest.h"
/************************************************************************************/
//  data_collect_modi.h                                                             //
//  header to active the recivers for recording.                                    //
//  data_collect_modi(thread_ID, no_samples, lenght_model, no_processor,            //
//  gather_for_whole_model(input array), data_for_active_receivers(output array),   //
//  receiver_positions, current_group_of_processors, no_source, no_receiver);       //
/************************************************************************************/
#include "data_collect_modi.h"


int main(int argc, void **argv)
{
    printf("%s\n", "************************************* Full Wave-form Inversion (FWI) *************************************");
    /***************************Input files**************************/
    if (argc < 7)
    {
        printf("Usage: fwi <parameter> <source_amp> <initial_model> <observe> <source_pos> <receiver_pos");
        exit(-1);
    }
    char *parameter = argv[1];
    char *source_amp = argv[2];
    char *initial_model = argv[3];
    char *observe = argv[4];
    char *source_pos = argv[5]; 
    char *receiver_pos = argv[6];
    /******************************************************************/

    /******************************************** read parameters from parameter file ****************************************************/
    char ts[100];
    int ns, grid_x, grid_y, src_x, rp, n_src, n_rcr, thread, maxitr, boundary_key, absorb_layer, obj_fun, upper_avoid, bound_avoid, data_order;
    float dt, dx;
    FILE *para;
    para = fopen(parameter, "r"); 
    fscanf(para, "%s %d %s %f %s %d %s %d %s %f %s %d %s %d %s %d %s %d %s %d %s %d %s %d %s %d %s %d %s %d %s %d %s %d", ts, &ns, ts, &dt, ts, &grid_x, ts, &grid_y, ts, &dx, ts, &src_x, ts, &rp, ts, &n_src, ts, &n_rcr, ts, &thread, ts, &maxitr, ts, &boundary_key, ts, &absorb_layer, ts, &obj_fun, ts, &upper_avoid, ts, &bound_avoid, ts, &data_order);
    fclose(para);
    /*************************************************************************************************************************************/

    /***************** print number of source and recivers ******************/
    printf("Number of sources are: %d\nNumber of receivers for each shot are: %d\n", n_src, n_rcr);
    /************************************************************************/

    /**************************** group of processors (decided on the basis of available processors) *****************************/
    int n_g_s = n_src / thread;
    /*****************************************************************************************************************************/

    /******************** read source signature **********************/
    float *src_term;                                 
    src_term = (float *)calloc((ns), sizeof(float)); 
    FILE *read_source;
    read_source = fopen(source_amp, "r");
    for (int i = 0; i < ns; i++)
    {
        fscanf(read_source, "%f", &src_term[i]);
    }
    fclose(read_source);
    /*****************************************************************/

    /******************* read initial velocity ***********************/
    float *vel_synt;                                              
    vel_synt = (float *)calloc((grid_x * grid_y), sizeof(float)); 
    if(data_order == 1)         //row_major == 1 or column_major == 0
    {
        printf("Array layout: Row-Major.\n");
        FILE *read_synthetic;
        read_synthetic = fopen(initial_model, "r");
        for(int i = 0; i < (grid_x * grid_y); i++)
        {
            fscanf(read_synthetic, "%f", &vel_synt[i]);
        }
        fclose(read_synthetic);
    }
    else
    {
        printf("Array layout: Column-Major.\n");
        float *vel_synt_temp;                                              
        vel_synt_temp = (float *)calloc((grid_x * grid_y), sizeof(float)); 
        FILE *read_synthetic_temp;
        read_synthetic_temp = fopen(initial_model, "r");
        for(int i = 0; i < (grid_x * grid_y); i++)
        {
            fscanf(read_synthetic_temp, "%f", &vel_synt_temp[i]);
        }
        fclose(read_synthetic_temp);
        for (int i = 0; i < grid_x; i++)
        {
            for (int j = 0; j < grid_y; j++)
            {
            vel_synt[grid_y * i + j] = vel_synt_temp[grid_x * j + i];
            }
        }
        free(vel_synt_temp);
    }
    /*****************************************************************/

    /****************************** read observe data ************************/
    float *gather_real;                                                 
    gather_real = (float *)calloc((n_src * ns * n_rcr), sizeof(float)); 
    if(data_order == 1)         //row_major == 1 or column_major == 0
    {
        FILE *read_real_data;
        read_real_data = fopen(observe, "r");
        for (int i = 0; i < (n_src * ns * n_rcr); i++)
        {
            fscanf(read_real_data, "%f", &gather_real[i]);
        }
        fclose(read_real_data);
    }
    else
    {
        float *temp_gather;
        temp_gather = (float *)calloc((n_src * ns * n_rcr), sizeof(float));
        FILE *read_temp;
        read_temp = fopen(observe, "r");
        for (int i = 0; i < (n_src * ns * n_rcr); i++)
        {
            fscanf(read_temp, "%f", &temp_gather[i]);
        }
        fclose(read_temp);
        for(int k = 0; k < n_src; k++)
        {
            for (int i = 0; i < ns; i++)
            {
                for (int j = 0; j < n_rcr; j++)
                {
                    gather_real[(k * ns * (n_rcr)) + ((n_rcr) * i + j)] = temp_gather[(k * ns * n_rcr) + (ns * j + i)];
                }
            }
        }
        free(temp_gather);
    } 
    /***************************************************************************/

    /***********************read source position file*************************/
    int *src_pos;
    src_pos = (int *)calloc(n_src, sizeof(int));
    FILE *read_source_pos;
    read_source_pos = fopen(source_pos, "r");
    for(int i = 0; i < n_src; i++)
    {
	    fscanf(read_source_pos, "%d", &src_pos[i]);
    }
    fclose(read_source_pos);
    /*************************************************************************/

    /***********************read receiver position file************************/
    int *rcr_position_collect;
    rcr_position_collect = (int *)calloc(n_src * n_rcr, sizeof(int));
    FILE *read_receiver_pos;
    read_receiver_pos = fopen(receiver_pos, "r");
    for(int i = 0; i < n_src * n_rcr; i++)
    {
	    fscanf(read_receiver_pos, "%d", &rcr_position_collect[i]);
    }
    fclose(read_receiver_pos);
    /*************************************************************************/

    /************* print receivers correspond to each shot*************/
    printf("*******************************************************************************\n");
    printf("S*(position of source) -> R#(position of receiver 1) ... R#(position of receiver N)\n");
    for(int i = 0; i < n_src; i++)
    {
        printf("S*%d -> ", src_pos[i]);
        for(int j = 0; j < n_rcr; j++)
        {
            printf("R#%d ", rcr_position_collect[j * n_src + i]);
        }
        printf("\n");
    }
    printf("********************************************************************************\n");
    /******************************************************************/

    /***********************************************/

    /************* set active processors ***********/
    omp_set_num_threads(thread);
    /***********************************************/
    
    float total_time = 0.0;
    /*************************** iterative process started ***************************/
    for (int itr = 0; itr < maxitr; itr++)
    {
	printf("****************** Iteration %d is started *********************\n", itr); 
        /***** start computation time calculation ******/
        double itime, ftime, exec_time;
        itime = omp_get_wtime();

        int src_y[thread];      // source positions on surface for each group of source
        
        float *normal_field_vec;
        normal_field_vec = (float *)calloc((thread * ns * grid_x * grid_y), sizeof(float));      // initialize the forward field

        float *gradient;                                              
        gradient = (float *)calloc((grid_x * grid_y), sizeof(float));                           // initialize the gradient for every iteration

        float *gather_step_length;
        gather_step_length = (float *)calloc((n_src * ns * n_rcr), sizeof(float));

        float *misfit;
        misfit = (float *)calloc(thread, sizeof(float));                                        // initialize the misfit for every iteration 
 
        float norm = 0.0;                                                                       // misfit value delearation


        /*************** synthetic data file **************/
        FILE *synt_data_write;
        synt_data_write = fopen("../output/synthetic_data/synthetic_data.txt", "w");
        /**************************************************/

        // part: 1
        /************************ divide the work among the group of processors for gradient calculation ******************************/
        for (int n = 0; n < n_g_s; n++) // n = number of group of processor, n_g_s = total number of groups.
        {
            float *gradu;                                                               // temporary memory to store gradient for n procesors
            gradu = (float *)calloc((thread * grid_x * grid_y), sizeof(float));         // temp file to store gradient for n processors
            
            float *gather_synt;                                                         // synthetic gather pointer
            gather_synt = (float *)calloc((thread * ns * grid_y), sizeof(float));       // initialize the synthetic gather vector for all sources, all row of gather are saved as column vector
            
            float *synt_data;                                                           // synthetic gather for selected receivers
            synt_data = (float *)calloc((thread * ns * n_rcr), sizeof(float));          // initialization
            
            float *residual;
            residual = (float *)calloc((thread * ns * n_rcr), sizeof(float));           // data difference between observed and synthetic data

            float *flip_residual;
            flip_residual = (float *)calloc((thread * ns), sizeof(float));              // flip the residual along the time axis and propagate the field

            float *back_field;
            back_field = (float *)calloc((thread * ns * grid_x * grid_y), sizeof(float));

            /******************* memory used to calculate the crosscorrelation objective function*************/
            float *sum_obs;
            sum_obs = (float *)calloc((thread * n_rcr), sizeof(float));
            float *sum_pred;
            sum_pred = (float *)calloc((thread * n_rcr), sizeof(float));
            float *sum_obs_pred;
            sum_obs_pred = (float *)calloc((thread * n_rcr), sizeof(float));
            /*************************************************************************************************/

            /********* prallel block is started to calculate the gradient for n shots ******/
            #pragma omp parallel
            {
                int tid;                                                            // thread ID
                tid = omp_get_thread_num();                                         // set thread ID
                misfit[tid] = 0.0;                                                  // misfit initialize
                src_y[tid] = src_pos[tid + thread * (n)];                           // source position allocated to each processor                                

                /*********************************** forward modeling of wave ********************************/
                //  forward_field(source_depth_position, source_position_on_surface,                         //
                //  depth_of_receiver, temporal_sampling, spatial_sampling,                                  //
                //  source_signature(array), velocity_model, field_vector(array), no_samples, depth_model,   //
                //  length_model, thread_ID, boundary_key, grids_reserved_boundary, gather_for_whole_model); //
                forward_field(src_x, src_y, rp, dt, dx, src_term, vel_synt, normal_field_vec, ns, grid_x, grid_y, tid, boundary_key, absorb_layer, gather_synt);
                /*********************************************************************************************/

                /*************** gather preparation for receivers as per input file *****************/
                //  data_collect_modi.h                                                             //
                //  header to active the recivers for recording.                                    //
                //  data_collect_modi(thread_ID, no_samples, lenght_model, no_processor,            //
                //  gather_for_whole_model(input array), data_for_active_receivers(output array),   //
                //  receiver_positions, current_group_of_processors, no_source, no_receiver);       //
                /************************************************************************************/
                data_collect_modi(tid, ns, grid_y, thread, gather_synt, synt_data, rcr_position_collect, n, n_src, n_rcr);
                /************************************************************************************/

                /***************************** cross-correlation objective calculation ************************/
                // helpfull in calculation of CFWI                                                            // 
                // reference:                                                                                //
                /*********************************************************************************************/
                for (int smpl = 0; smpl < ns; smpl++)
                {
                    for (int j = 0; j < n_rcr; j++)
                    {
                        sum_obs[n_rcr * tid + j] = sum_obs[n_rcr * tid + j] + pow(gather_real[((n) * thread * (ns * n_rcr)) + (tid * ns * n_rcr) + n_rcr * smpl + j], 2);
                        sum_pred[n_rcr * tid + j] = sum_pred[n_rcr * tid + j] + pow(synt_data[(tid * ns * n_rcr) + n_rcr * smpl + j], 2);
                        sum_obs_pred[n_rcr * tid + j] = sum_obs_pred[n_rcr * tid + j] + (synt_data[(tid * ns * n_rcr) + n_rcr * smpl + j] * gather_real[((n) * thread * (ns * n_rcr)) + (tid * ns * n_rcr) + n_rcr * smpl + j]);
                    }
                }
                /**********************************************************************************************/

                /*********************************** objective function calculation ****************************************/
                // if (obj_fn == 1), objecitve function is l2 norm, otherwise cross-correlation objective function        //
                /***********************************************************************************************************/
                for (int smpl = 0; smpl < ns; smpl++)
                {
                    for (int j = 0; j < n_rcr; j++)
                    {
                        if (obj_fun == 1)        // 1 for L2 norm and 0 for cros-correlation
                        {
                            residual[(tid * ns * n_rcr) + n_rcr * smpl + j] = synt_data[(tid * ns * n_rcr) + n_rcr * smpl + j] - gather_real[((n) * thread * (ns * n_rcr)) + (tid * ns * n_rcr) + n_rcr * smpl + j];
                            misfit[tid] = misfit[tid] + pow(residual[(tid * ns * n_rcr) + n_rcr * smpl + j], 2);
                        }
                        else
                        {
                            residual[(tid * ns * n_rcr) + n_rcr * smpl + j] = (1 / (sqrt(sum_obs[n_rcr * tid + j]) * sqrt(sum_pred[n_rcr * tid + j]))) * ((sum_obs_pred[n_rcr * tid + j] / sum_pred[n_rcr * tid + j]) * synt_data[(tid * ns * n_rcr) + n_rcr * smpl + j] - gather_real[((n) * thread * (ns * n_rcr)) + (tid * ns * n_rcr) + n_rcr * smpl + j]);
                            misfit[tid] =  (1 / (n_src * (n_rcr))) * (misfit[tid] - (sum_obs_pred[n_rcr * tid + j] / (sqrt(sum_obs[n_rcr * tid + j]) * sqrt(sum_pred[n_rcr * tid + j]))));                       
                        }
                    }
                }
                /*************************************************************************************************************/

                /*************************************************************************************************************/
                // calculation of time reversal of residual field & receiver position from input file                        //
                /*************************************************************************************************************/
                //int rcr[thread];
                //for(rcr[tid] = 0; rcr[tid] < n_rcr; rcr[tid]++)
                //{
			        //rcr_position[tid] = rcr_position_collect[(n) * thread + rcr[tid] * n_src + tid];
                    /*for (int smpl = 0; smpl < ns; smpl++)
                    {
                        flip_residual[tid * ns + smpl] = residual[(tid * ns * n_rcr) + (ns - smpl - 1) * n_rcr + rcr[tid]];
                    }*/
                    /*********************************************************************************************************/

                    /****************************** back propagation of field ***********************************/
                    //  field back propagated where each receiever position is used as the source position      //
                    //  backward_field(backward_source_depth_position(same as receiver),                        //
                    //  receiver_position_on_surface (for back propagation),                                    //
                    //  temporal_sampling, spatial_sampling, flip_residual, velocity_model                      //
                    //  back_field_vector, no_samples, depth_model, length_model, thread_ID, boundary_key       //
                    //  grids_reserved_boundary, );                                                             //  
                    /********************************************************************************************/
                    //backward_field(rp, rcr_position, dt, dx, flip_residual, vel_synt, back_field, ns, grid_x, grid_y, tid, boundary_key, absorb_layer);
                    /********************************************************************************************/
         
                    //backward_field(rp, rcr_position, dt, dx, flip_residual, vel_synt, back_field, ns, grid_x, grid_y, tid, boundary_key, absorb_layer, upper_avoid, bound_avoid, normal_field_vec, gradu);

                    backward_field(rp, rcr_position_collect, dt, dx, residual, vel_synt, back_field, ns, grid_x, grid_y, tid, boundary_key, absorb_layer, upper_avoid, bound_avoid, normal_field_vec, gradu, thread, n, n_rcr, n_src);

                    /*************************** gradient calculation *******************************************/
                    // gardient is calculated for a group of sources according to the available processors      //
                    // and save as gradu                                                                        //
                    // correlation of the forward and backward field                                            //
                    /********************************************************************************************/
                    /*for (int smpl = 1; smpl < ns - 1; smpl++)
                    {
                        for (int i = 0; i < grid_x; i++)
                        {
                            for (int j = 0; j < grid_y; j++)
                            {
                                if (i >= upper_avoid && i <= grid_x - bound_avoid - 1 && j >= bound_avoid && j <= grid_y - bound_avoid - 1)
                                    gradu[(tid * grid_x * grid_y) + grid_y * i + j] = gradu[(tid * grid_x * grid_y) + grid_y * i + j] - (2 / pow(vel_synt[grid_y * i + j], 3)) * back_field[(tid * grid_x * grid_y * ns) + grid_y * (((ns - 1) - smpl) * grid_x + i) + j] * ((normal_field_vec[(tid * grid_x * grid_y * ns) + grid_y * ((smpl + 1) * grid_x + i) + j] - 2 * normal_field_vec[(tid * grid_x * grid_y * ns) + grid_y * (smpl * grid_x + i) + j] + normal_field_vec[(tid * grid_x * grid_y * ns) + grid_y * ((smpl - 1) * grid_x + i) + j]) / (dt * dt));
                            }
                        }
                    } */   
                    /*******************************************************************************************/ 		
                //}
            #pragma omp barrier                
            }
            
            /***************** gradient is calculated for n shots ************************/

            for (int i = 0; i < (thread * ns * (n_rcr)); i++)
            {
                fprintf(synt_data_write, "%f\n", synt_data[i]);
                gather_step_length[n * (thread * ns * (n_rcr)) + i] = synt_data[i];                 // synthetic gather saved for step length calculation
            }
            
            for (int tid = 0; tid < thread; tid++)
            {
                norm = norm + misfit[tid];
            }	        

            // gradient summation for all sources
            for (int tid = 0; tid < thread; tid++)
            {
                for (int i = 0; i < grid_x; i++)
                {
                    for (int j = 0; j < grid_y; j++)
                    {
                        gradient[grid_y * i + j] = gradient[grid_y * i + j] + gradu[(tid * grid_x * grid_y) + grid_y * i + j];
                    }
                }
            }
            free(gradu);
            free(gather_synt);
            free(synt_data);
            free(residual);
            free(flip_residual);   
            free(back_field);        
            free(sum_obs);
            free(sum_pred);
            free(sum_obs_pred);
        }
        /*********************************************************** gradient calculated for every source completed ************************************************************************/
	    
        // part:2
        /************************************ step length calculation ******************************/

        /**************** absolute value of gradient ***************************/
        float *grad_abs;
        grad_abs = (float *)calloc((grid_x * grid_y), sizeof(float)); // absolute value of the gradient
        for (int i = 0; i < grid_x; i++)
        {
            for (int j = 0; j < grid_y; j++)
            {
                grad_abs[grid_y * i + j] = fabs(gradient[grid_y * i + j]);
            }
        }
        /**********************************************************************/
	
        float maximum_gradient;
        maximum_gradient = largest(grad_abs, grid_x, grid_y);               // maximum value in gradient matrix
        free(grad_abs);
        
        float maximum_vel;
        maximum_vel = largest(vel_synt, grid_x, grid_y);                    // maximum value from the velocity profile
        float epsilon;
        epsilon = maximum_vel / (100 * maximum_gradient);
        
        float alpha11 = 0.0;
        float alpha22 = 0.0;

	    for (int n = 0; n < n_g_s; n++)
        {
            float *mk_plus_edk;
            mk_plus_edk = (float *)calloc((grid_x * grid_y), sizeof(float)); // perturbed velocity for step length calculation

            float *gather_mkpedk; // gather pointer for step length
            gather_mkpedk = (float *)calloc((thread * ns * grid_y), sizeof(float));

            float *synt_data_mkpedk;
            synt_data_mkpedk = (float *)calloc((thread * ns * n_rcr), sizeof(float));

            float *jkdek;
            jkdek = (float *)calloc((thread * ns * grid_y), sizeof(float)); // small value

            float *alpha1;
            alpha1 = (float *)calloc(thread, sizeof(float)); // numerator for step length calculation

            float *alpha2;
            alpha2 = (float *)calloc(thread, sizeof(float)); // denominator for step length calculation

            #pragma omp parallel
            {
                int tid;
                tid = omp_get_thread_num();
                alpha1[tid] = 0.0;
                alpha2[tid] = 0.0;
                src_y[tid] = src_pos[tid + thread * (n)];                                       // source position allocated to each processor

                /*****************************velocity_model + epsilon X gradientt***************************/
                for (int i = 0; i < grid_x; i++)
                {
                    for (int j = 0; j < grid_y; j++)
                    {
                        mk_plus_edk[grid_y * i + j] = vel_synt[grid_y * i + j] + epsilon * gradient[grid_y * i + j];
                    }
                }
                
                /********************************************************************************************/
                
                /*********************************** forward modeling of wave ********************************/
                //  forward_field(source_depth_position, source_position_on_surface,                         //
                //  depth_of_receiver, temporal_sampling, spatial_sampling,                                  //
                //  source_signature(array), velocity_model, field_vector(array), no_samples, depth_model,   //
                //  length_model, thread_ID, boundary_key, grids_reserved_boundary, gather_for_whole_model); //
                forward_field(src_x, src_y, rp, dt, dx, src_term, mk_plus_edk, normal_field_vec, ns, grid_x, grid_y, tid, boundary_key, absorb_layer, gather_mkpedk);
                /*********************************************************************************************/

                /*************** gather preparation for receivers as per input file *****************/
                //  data_collect_modi.h                                                             //
                //  header to active the recivers for recording.                                    //
                //  data_collect_modi(thread_ID, no_samples, lenght_model, no_processor,            //
                //  gather_for_whole_model(input array), data_for_active_receivers(output array),   //
                //  receiver_positions, current_group_of_processors, no_source, no_receiver);       //
                /************************************************************************************/
                data_collect_modi(tid, ns, grid_y, thread, gather_mkpedk, synt_data_mkpedk, rcr_position_collect, n, n_src, n_rcr);
                /************************************************************************************/

                /*************************** synthetic_data(mkplusedk) - gather_step_length*****************************/
                for (int smpl = 0; smpl < ns; smpl++)
                {
                    for (int j = 0; j < n_rcr; j++)
                    {
                        jkdek[(tid * n_rcr * ns) + n_rcr * smpl + j] = (synt_data_mkpedk[(tid * n_rcr * ns) + n_rcr * smpl + j] - gather_step_length[((n) * thread * (ns * n_rcr)) + (tid * ns * n_rcr) + n_rcr * smpl + j]) / epsilon;
                    }
                }
                /*******************************************************************************************************/

                for (int smpl = 0; smpl < ns; smpl++)
                {
                    for (int j = 0; j < n_rcr; j++)
                    {
                        alpha2[tid] = alpha2[tid] + (jkdek[(tid * n_rcr * ns) + n_rcr * smpl + j] * jkdek[(tid * n_rcr * ns) + n_rcr * smpl + j]);
                        alpha1[tid] = alpha1[tid] + (jkdek[(tid * n_rcr * ns) + n_rcr * smpl + j] * (gather_real[((n) * thread * (ns * n_rcr)) + (tid * n_rcr * ns) + n_rcr * smpl + j] - gather_step_length[((n) * thread * (ns * n_rcr)) + (tid * ns * n_rcr) + n_rcr * smpl + j]));
                    }
                }

            #pragma omp barrier
            }

            for (int tid = 0; tid < thread; tid++)
            {
                alpha11 = alpha11 + alpha1[tid];
                alpha22 = alpha22 + alpha2[tid];
            }
            free(mk_plus_edk);
            free(gather_mkpedk);
            free(synt_data_mkpedk);
            free(jkdek);
            free(alpha1);
            free(alpha2);
        }
        
        float alpha = 0.0;
        alpha = alpha11 / alpha22;    

        /*********************** model updation ***************************/
        for (int i = 0; i < grid_x; i++)
        {
            for (int j = 0; j < grid_y; j++)
            {
                vel_synt[grid_y * i + j] = vel_synt[grid_y * i + j] + alpha * gradient[grid_y * i + j];
            }
        }
        /******************************************************************/

        /*char c;
        FILE *status_gui_file;
        status_gui_file = fopen("current_status_gui.txt", "r");
        c = fgetc(status_gui_file);
        while(c == 'b')
        {
            rewind(status_gui_file);
            c = fgetc(status_gui_file);
        }
        fclose(status_gui_file);
        FILE *status_main_file;
        status_main_file = fopen("current_status_main.txt", "w");
        fputc('b', status_main_file);  
        rewind(status_gui_file);
        fclose(status_main_file);
        */
    	FILE *inverted_file;
    	inverted_file = fopen("../output/inverted/inverted_velocity.txt", "w");
    	for (int i = 0; i < grid_x; i++)
    	{
        	for (int j = 0; j < grid_y; j++)
        	{
            	fprintf(inverted_file, "%0.50f\t", vel_synt[grid_y * i + j]);
        	}
        	fprintf(inverted_file, "\n");
    	}
    	fclose(inverted_file);
        /*status_main_file = fopen("current_status_main.txt", "w");
	    if(itr == maxitr - 1)
		fputc('e', status_main_file);
	    else
        fputc('d', status_main_file);  
        fclose(status_main_file);*/

        /************ open file to write objective function (globally to add every iteration output) ***********/
        FILE *convergence;
        convergence = fopen("../output/objectivefn/objective_function.txt", "a+"); 
        fprintf(convergence, "%0.20f\n", norm);
        fclose(convergence);
        /******************************************************************************************************/
        
        free(gather_step_length);
        free(normal_field_vec);
        fclose(synt_data_write);
        /************computation time*****************/
        ftime = omp_get_wtime();
        exec_time =((double) (ftime - itime));
        /**********************************************/
        total_time = total_time + exec_time;
        /**************** print output *********************/
        printf("############################# Iteration %d #############################\n", itr);
        printf("Objective function is: %0.3f\n", norm);
        printf("Progress ");
        printf("%0.1lf percent\n", ceil((itr + 1) * (100.0 / maxitr)));
        printf("Computation time is %lf second\n", exec_time);
    }        
    
    printf("Process is completed in %f seconds\n", total_time);
    printf("#######################################################################\n");
    /************************************ end of iterations *******************************/

    free(src_term);
	free(gather_real);
	free(vel_synt);    
    return 0;
}
