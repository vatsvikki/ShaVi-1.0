int data_collect_modi(long long int  tid, long long int  ns, long long int  grid_y, long long int  thread, float *gather, float *data, long long int  *rcr_position_collect, long long int  n, long long int  n_src, long long int  n_rcr)
{
    for(long long int  i = 0; i < ns; i++)
    {
        for(long long int  j = 0; j < n_rcr; j++)
        {
            data[(tid * ns * n_rcr) + n_rcr * i + j] = gather[tid * ns * grid_y + rcr_position_collect[(n) * thread + (tid + n_src * j)] + grid_y * i];
        }
    }  
}
