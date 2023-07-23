//int data_collect_modi(int tid, int no_left_rcr, int left_offset, int no_right_rcr, int right_offset, int spacing, int row, int col, int thread, int *shot, float *gather, float *data, int flag0, int flag, int *rcr_position_collect, int n)
int data_collect_modi(int tid, int ns, int grid_y, int thread, float *gather, float *data, int *rcr_position_collect, int n, int n_src, int n_rcr)
{
    for(int i = 0; i < ns; i++)
    {
        for(int j = 0; j < n_rcr; j++)
        {
            data[(tid * ns * n_rcr) + n_rcr * i + j] = gather[tid * ns * grid_y + rcr_position_collect[(n) * thread + (tid + n_src * j)] + grid_y * i];
        }
    }  
}
