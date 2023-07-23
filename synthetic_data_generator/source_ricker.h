#define pi 3.141592653589793238460
float src(float dt, float freq, float *src_term, int ns)
{
    float t;	
    float t0 = 1 / freq;
    for (int smpl = 0; smpl < ns; smpl++)
    {
        t = smpl * dt;
	src_term[smpl] = 1e15 * (1 - (2 * pi * pi * freq * freq * ((t - t0) * (t - t0)))) * exp(-1 * pi * pi * freq * freq * ((t - t0) * (t - t0)));	//ricker wavelet
    }
}
