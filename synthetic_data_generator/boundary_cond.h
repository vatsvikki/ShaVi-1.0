float boundary_coefficient(int grid_x, int grid_y, float N, float M, float *w, float *a, float *b)
{
    float alpha = 1.5 + 0.07 * (N - M);
    for (int i = 0; i < grid_x; i++)
    {
        for (int j = 0; j < grid_y; j++)
        {
            if (j < N)
            {
                w[i * grid_y + j] = pow((N - j) / (N - M), alpha);
            }
            w[i * grid_y + (grid_y - j - 1)] = w[i * grid_y + j];
        }
    }

    for (int i = 0; i < grid_x; i++)
    {
        for (int j = i; j < grid_y - i; j++)
        {
            if (i < N)
            {
                w[(grid_x - i - 1) * grid_y + j] = pow((N - i) / (N - M), alpha);
            }
        }
    }

    for (int i = 0; i < grid_x; i++)
    {
        for (int j = 0; j < grid_y; j++)
        {
            if (i >= grid_x - M)
                w[i * grid_y + j] = 1.0; 
            else if (j < M)
                w[i * grid_y + j] = 1.0; 
            else if (j >= grid_y - M)
                w[i * grid_y + j] = 1.0; 
        }
    }    

    /*FILE *fabc;
    fabc = fopen("../output/temp_data/abc.txt", "w");
    for (int i = 0; i < grid_x; i++)
    {
        for (int j = 0; j < grid_y; j++)
        {
            fprintf(fabc, "%f\t", w[i * grid_y + j]);
        }
        fprintf(fabc, "\n");
    }*/

    /*FILE *aw;
    aw = fopen("../output/temp_data/a.txt", "w");
    FILE *bw;
    bw = fopen("../output/temp_data/b.txt", "w");*/
    for (int i = 0; i < grid_x; i++)
    {
        for (int j = 0; j < grid_y; j++)
        {
            if (i <= grid_x / 2 && j <= grid_y / 2)
            {
                a[i * grid_y + j] = 1.0;
                b[i * grid_y + j] = 1.0;
            }
            else if (i <= grid_x / 2 && j > grid_y / 2)
            {
                a[i * grid_y + j] = 1.0;
                b[i * grid_y + j] = -1.0;
            }
            else if (i > grid_x / 2 && j <= grid_y / 2)
            {
                a[i * grid_y + j] = -1.0;
                b[i * grid_y + j] = +1.0;
            }
            else
            {
                a[i * grid_y + j] = -1.0;
                b[i * grid_y + j] = -1.0;
            }
            //fprintf(aw, "%f\t", a[i * grid_y + j]);
            //fprintf(bw, "%f\t", b[i * grid_y + j]);
        }
        //fprintf(aw, "\n");
        //fprintf(bw, "\n");        
    }
    //fclose(aw);
    //fclose(bw);
    return 0;
}
