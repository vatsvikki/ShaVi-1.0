float largest(float *vec, long long int  grid_x, long long int  grid_y)
{
    float max = 0;	//Initialize maximum
    for (long long int  i = 0; i < grid_x; i++)
    {
        for (long long int  j = 0; j < grid_y; j++)
        {
            if (vec[grid_y * i + j] >= max)
                max = vec[grid_y * i + j];
        }
    }
    return max;		//return maximum of a vector 
}
