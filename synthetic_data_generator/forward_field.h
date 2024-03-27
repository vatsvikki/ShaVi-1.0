float forward_field(long long int src_x, long long int *src_y, long long int rp, float dt, float dx, float *src_term, float *c, float *field_vec, long long int ns, long long int grid_x, long long int grid_y, long long int tid, long long int boundary_key, long long int absorb_layer, float *gather)
{
    float *w;
    w = (float *)calloc((grid_x * grid_y), sizeof(float));
    float *a;
    a = (float *)calloc((grid_x * grid_y), sizeof(float));
    float *b;
    b = (float *)calloc((grid_x * grid_y), sizeof(float));

    if (boundary_key == 1)
    {
        /*long long int N = absorb_layer / dx;*/
        long long int N = absorb_layer;
        long long int M = 5;
        boundary_coefficient(grid_x, grid_y, N, M, w, a, b);
    }

    

    long long int tid_ns_grid_x_grid_y = tid * ns * grid_x * grid_y;

    for (long long int i = 0; i < ns * grid_x * grid_y; i++)
    {
        field_vec[tid_ns_grid_x_grid_y + i] = 0.0;
    }
    
    for (long long int smpl = 1; smpl < ns - 1; smpl++)
    {
        field_vec[(tid * ns * grid_x * grid_y) + grid_y * (smpl * grid_x + src_x) + src_y[tid]] = field_vec[(tid * ns * grid_x * grid_y) + grid_y * (smpl * grid_x + src_x) + src_y[tid]] + src_term[smpl] / (dx * dx) * dt * dt;
        for (long long int i = 0; i < grid_x; i++)
        {
            for (long long int j = 0; j < grid_y; j++)
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
    for (long long int smpl = 0; smpl < ns; smpl++)
    {
        for(long long int j = 0; j < grid_y; j++)
        {
           gather[(tid * ns * grid_y) + grid_y * smpl + j] = field_vec[(tid * ns * grid_x * grid_y) + grid_y * (smpl * grid_x + rp) + j];
        }
    }
    free(w);
    free(a);
    free(b);
}
