float largest(float *vec, int grid_x, int grid_y)
{
    float max = 0;	//Initialize maximum
    for (int i = 0; i < grid_x; i++)
    {
        for (int j = 0; j < grid_y; j++)
        {
            if (vec[grid_y * i + j] >= max)
                max = vec[grid_y * i + j];
        }
    }
    return max;		//return maximum of a vector 
}
