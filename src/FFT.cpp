#include "FFT.h"

// https://rosettacode.org/wiki/Fast_Fourier_transform#C.2B.2B
void fft(CArray& x)
{
	// DFT
	size_t N = x.size(), k = N, n;
	double thetaT = pi / N;
	Complex phiT = Complex(cos(thetaT), -sin(thetaT)), T;
	while (k > 1) {
		n = k;
		k >>= 1;
		phiT = phiT * phiT;
		T = 1.0L;
		for (size_t l = 0; l < k; l++) {
			for (size_t a = l; a < N; a += n) {
				size_t b = a + k;
				Complex t = x[a] - x[b];
				x[a] += x[b];
				x[b] = t * T;
			}
			T *= phiT;
		}
	}
	// Decimate
	size_t m = (size_t)log2(N);
	for (size_t a = 0; a < N; a++) {
		size_t b = a;
		// Reverse bits
		b = (((b & 0xaaaaaaaa) >> 1) | ((b & 0x55555555) << 1));
		b = (((b & 0xcccccccc) >> 2) | ((b & 0x33333333) << 2));
		b = (((b & 0xf0f0f0f0) >> 4) | ((b & 0x0f0f0f0f) << 4));
		b = (((b & 0xff00ff00) >> 8) | ((b & 0x00ff00ff) << 8));
		b = ((b >> 16) | (b << 16)) >> (32 - m);
		if (b > a) {
			Complex t = x[a];
			x[a] = x[b];
			x[b] = t;
		}
	}
}

double hanningWindow(double ind, double size) {
    return 0.5 - 0.5 * cos(2.0 * pi * ind / (size - 1));
}