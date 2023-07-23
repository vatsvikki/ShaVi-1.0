//float backward_field(int src_x, int *rcr_position, float dt, float dx, float *src_term, float *c, float *field_vec, int ns, int grid_x, int grid_y, int tid, int boundary_key, int absorb_layer, int upper_avoid, int bound_avoid, float *normal_field_vec, float *gradu)
float backward_field(int src_x, int *rcr_position_collect, float dt, float dx, float *residual, float *c, float *field_vec, int ns, int grid_x, int grid_y, int tid, int boundary_key, int absorb_layer, int upper_avoid, int bound_avoid, float *normal_field_vec, float *gradu, int thread, int n, int n_rcr, int n_src)
{
    float *w;
    w = (float *)calloc((grid_x * grid_y), sizeof(float));
    float *a;
    a = (float *)calloc((grid_x * grid_y), sizeof(float));
    float *b;
    b = (float *)calloc((grid_x * grid_y), sizeof(float));

    if (boundary_key == 1)
    {
        /*int N = absorb_layer / dx;*/
        int N = absorb_layer;
        int M = 5;
        boundary_coefficient(grid_x, grid_y, N, M, w, a, b);
    }



    int tid_ns_grid_x_grid_y = tid * ns * grid_x * grid_y;

    for (int i = 0; i < ns * grid_x * grid_y; i++)
    {
        field_vec[tid_ns_grid_x_grid_y + i] = 0.0;
    }
    
    int rcr_position[thread];
    
    float *src_term;                                 
    src_term = (float *)calloc((thread * ns), sizeof(float)); 
    int rcr[thread];
    //for(rcr[tid] = 0; rcr[tid] < n_rcr; rcr[tid]++)
    //{
    for (int smpl = 0; smpl < ns; smpl++)
    {
        for(rcr[tid] = 0; rcr[tid] < n_rcr; rcr[tid]++)
        {
        src_term[tid * ns + smpl] = residual[(tid * ns * n_rcr) + (ns - smpl - 1) * n_rcr + rcr[tid]];
        rcr_position[tid] = rcr_position_collect[(n) * thread + rcr[tid] * n_src + tid];
        field_vec[tid_ns_grid_x_grid_y + grid_y * (smpl * grid_x + src_x) + rcr_position[tid]] = field_vec[tid_ns_grid_x_grid_y + grid_y * (smpl * grid_x + src_x) + /*src_y[tid]*/ + rcr_position[tid]] + src_term[tid * ns + smpl] / (dx * dx) * dt * dt;
        }
        for (int i = 0; i < grid_x; i++)
        {
            for (int j = 0; j < grid_y; j++)
            {
                if (smpl == 0 || smpl == ns - 1 || i <= 2 || i >= grid_x - 3 || j <= 2 || j >= grid_y - 3)
                {
                    field_vec[tid_ns_grid_x_grid_y + grid_y * (smpl * grid_x + i) + j] = 0.0;
                }
                else
                {
                    float z_r = pow((c[grid_y * i + j] * dt / dx), 2);
                    if (boundary_key == 1)
                        field_vec[tid_ns_grid_x_grid_y + grid_y * ((smpl + 1) * grid_x + i) + j] = (1.0 - w[i * grid_y + j]) * (z_r * (((2 * field_vec[tid_ns_grid_x_grid_y + grid_y * (smpl * grid_x + (i - 3)) + j] - 27 * field_vec[tid_ns_grid_x_grid_y + grid_y * (smpl * grid_x + (i - 2)) + j] + 270 * field_vec[tid_ns_grid_x_grid_y + grid_y * (smpl * grid_x + (i - 1)) + j] - 490 * field_vec[tid_ns_grid_x_grid_y + grid_y * (smpl * grid_x + i) + j] + 270 * field_vec[tid_ns_grid_x_grid_y + grid_y * (smpl * grid_x + (i + 1)) + j] - 27 * field_vec[tid_ns_grid_x_grid_y + grid_y * (smpl * grid_x + (i + 2)) + j] + 2 * field_vec[tid_ns_grid_x_grid_y + grid_y * (smpl * grid_x + (i + 3)) + j]) / 180) + ((2 * field_vec[tid_ns_grid_x_grid_y + grid_y * (smpl * grid_x + i) + (j - 3)] - 27 * field_vec[tid_ns_grid_x_grid_y + grid_y * (smpl * grid_x + i) + (j - 2)] + 270 * field_vec[tid_ns_grid_x_grid_y + grid_y * (smpl * grid_x + i) + (j - 1)] - 490 * field_vec[tid_ns_grid_x_grid_y + grid_y * (smpl * grid_x + i) + j] + 270 * field_vec[tid_ns_grid_x_grid_y + grid_y * (smpl * grid_x + i) + (j + 1)] - 27 * field_vec[tid_ns_grid_x_grid_y + grid_y * (smpl * grid_x + i) + (j + 2)] + 2 * field_vec[tid_ns_grid_x_grid_y + grid_y * (smpl * grid_x + i) + (j + 3)]) / 180)) + 2.0 * field_vec[tid_ns_grid_x_grid_y + grid_y * (smpl * grid_x + i) + j] - field_vec[tid_ns_grid_x_grid_y + grid_y * ((smpl - 1) * grid_x + i) + j]) + w[i * grid_y + j] * (((c[grid_y * i + j] * dt) / (sqrt(2) * dx)) * ((1 / 30.0) * a[i * grid_y + j] * (-1 * field_vec[tid_ns_grid_x_grid_y + grid_y * (smpl * grid_x + (i - 3)) + j] + 9 * field_vec[tid_ns_grid_x_grid_y + grid_y * (smpl * grid_x + (i - 2)) + j] - 45 * field_vec[tid_ns_grid_x_grid_y + grid_y * (smpl * grid_x + (i - 1)) + j] + 0 * field_vec[tid_ns_grid_x_grid_y + grid_y * (smpl * grid_x + (i)) + j] + 45 * field_vec[tid_ns_grid_x_grid_y + grid_y * (smpl * grid_x + (i + 1)) + j] - 9 * field_vec[tid_ns_grid_x_grid_y + grid_y * (smpl * grid_x + (i + 2)) + j] + field_vec[tid_ns_grid_x_grid_y + grid_y * (smpl * grid_x + (i + 3)) + j]) + (1 / 30.0) * b[i * grid_y + j] * (-1 * field_vec[tid_ns_grid_x_grid_y + grid_y * (smpl * grid_x + i) + (j - 3)] + 9 * field_vec[tid_ns_grid_x_grid_y + grid_y * (smpl * grid_x + i) + (j - 2)] - 45 * field_vec[tid_ns_grid_x_grid_y + grid_y * (smpl * grid_x + i) + (j - 1)] + 0 * field_vec[tid_ns_grid_x_grid_y + grid_y * (smpl * grid_x + i) + (j)] + 45 * field_vec[tid_ns_grid_x_grid_y + grid_y * (smpl * grid_x + i) + (j + 1)] - 9 * field_vec[tid_ns_grid_x_grid_y + grid_y * (smpl * grid_x + i) + (j + 2)] + field_vec[tid_ns_grid_x_grid_y + grid_y * (smpl * grid_x + i) + (j + 3)])) + field_vec[tid_ns_grid_x_grid_y + grid_y * ((smpl - 1) * grid_x + i) + (j)]);
                    else
                        field_vec[tid_ns_grid_x_grid_y + grid_y * ((smpl + 1) * grid_x + i) + j] = z_r * (((2 * field_vec[tid_ns_grid_x_grid_y + grid_y * (smpl * grid_x + (i - 3)) + j] - 27 * field_vec[tid_ns_grid_x_grid_y + grid_y * (smpl * grid_x + (i - 2)) + j] + 270 * field_vec[tid_ns_grid_x_grid_y + grid_y * (smpl * grid_x + (i - 1)) + j] - 490 * field_vec[tid_ns_grid_x_grid_y + grid_y * (smpl * grid_x + i) + j] + 270 * field_vec[tid_ns_grid_x_grid_y + grid_y * (smpl * grid_x + (i + 1)) + j] - 27 * field_vec[tid_ns_grid_x_grid_y + grid_y * (smpl * grid_x + (i + 2)) + j] + 2 * field_vec[tid_ns_grid_x_grid_y + grid_y * (smpl * grid_x + (i + 3)) + j]) / 180) + ((2 * field_vec[tid_ns_grid_x_grid_y + grid_y * (smpl * grid_x + i) + (j - 3)] - 27 * field_vec[tid_ns_grid_x_grid_y + grid_y * (smpl * grid_x + i) + (j - 2)] + 270 * field_vec[tid_ns_grid_x_grid_y + grid_y * (smpl * grid_x + i) + (j - 1)] - 490 * field_vec[tid_ns_grid_x_grid_y + grid_y * (smpl * grid_x + i) + j] + 270 * field_vec[tid_ns_grid_x_grid_y + grid_y * (smpl * grid_x + i) + (j + 1)] - 27 * field_vec[tid_ns_grid_x_grid_y + grid_y * (smpl * grid_x + i) + (j + 2)] + 2 * field_vec[tid_ns_grid_x_grid_y + grid_y * (smpl * grid_x + i) + (j + 3)]) / 180)) + 2.0 * field_vec[tid_ns_grid_x_grid_y + grid_y * (smpl * grid_x + i) + j] - field_vec[tid_ns_grid_x_grid_y + grid_y * ((smpl - 1) * grid_x + i) + j];
                }
            }
        }
        
    }
    for (int smpl = 0; smpl < ns; smpl++)
    {
        for (int i = upper_avoid; i < grid_x - bound_avoid - 1; i++)
        {
            for (int j = bound_avoid; j < grid_y - bound_avoid - 1; j++)
            {
                    //if (i >= upper_avoid && i <= grid_x - bound_avoid - 1 && j >= bound_avoid && j <= grid_y - bound_avoid - 1)
                        gradu[(tid * grid_x * grid_y) + grid_y * i + j] = gradu[(tid * grid_x * grid_y) + grid_y * i + j] - (2 / pow(c[grid_y * i + j], 3)) * field_vec[(tid * grid_x * grid_y * ns) + grid_y * (((ns - 1) - smpl) * grid_x + i) + j] * ((normal_field_vec[(tid * grid_x * grid_y * ns) + grid_y * ((smpl + 1) * grid_x + i) + j] - 2 * normal_field_vec[(tid * grid_x * grid_y * ns) + grid_y * (smpl * grid_x + i) + j] + normal_field_vec[(tid * grid_x * grid_y * ns) + grid_y * ((smpl - 1) * grid_x + i) + j]) / (dt * dt));
            }
        }
    }
    //}
}
