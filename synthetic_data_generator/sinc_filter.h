float sinc_filter(int N, float fc, float d_t, float a_data[], int col, int row, int pr, int th_id)
{
    //number of samples
    int nsamp = col * row;
    //temp file to edit the data layout
    float *temp1_signal;
    temp1_signal = (float *)calloc(pr * nsamp, sizeof(float));
        for (int j = 0; j < col; j++)
        {
            for (int i = 0; i < row; i++)
            {
                temp1_signal[(th_id * col * row) + row * j + i] = a_data[(th_id * col * row) + j + col * i];
            }
        }

    float *temp_data;
    temp_data = (float *)calloc((N + pr * nsamp), sizeof(float));
    float d;
    //cutoff frequency
    float f_c;
    //filter length
    float data[N];
    float temp = 0.0;
    f_c = fc * d_t;
    for (int i = 0; i < N; i++)
    {
        d = 2 * f_c * ((i + 1.0) - N / 2.0);
        if (fabs(d) < 1.0020841800044864E-292)
        {
            d = 1.0;
        }
        else
        {
            d = d * 3.1415926535897931;
            d = sin(d) / d;
        }
        data[i] = d * ((0.42 - 0.5 * cos(6.2831853071795862 * (i + 1.0) / N)) + 0.08 * cos(12.566370614359172 * (i + 1.0) / N));
        temp = temp + data[i];
    }
    for (int i = 0; i < N; i++)
    {
        data[i] = data[i] / temp;
    }
    int nApnB;
    int a_size = nsamp * pr;
    int b_size = N;

    //filtered data
    for (int k = 0; k < a_size; k++)
    {
        for (int b_k = 0; b_k < b_size; b_k++)
        {
            nApnB = k + b_k;
            temp_data[nApnB] += temp1_signal[k] * data[b_k];
        }
    }

    //change the data layout
    float *temp2_signal;
    temp2_signal = (float *)calloc(pr * nsamp, sizeof(float));
        for (int j = 0; j < col; j++)
        {
            for (int i = 0; i < row; i++)
            {
                temp2_signal[(th_id * col * row) + j + col * i] = temp_data[(th_id * col * row) + (row * j + i) + (N / 2 - 1)];
            }
        }

    //save the data as it is passed
    for (int i = 0; i < nsamp; i++)
    {
        a_data[th_id * nsamp + i] = temp2_signal[th_id * nsamp + i];
    }
	free(temp1_signal);
	free(temp2_signal);
	free(temp_data);
}
