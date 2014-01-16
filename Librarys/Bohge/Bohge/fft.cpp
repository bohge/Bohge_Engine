//////////////////////////////////////////////////////////////////////////////////////
//
//						The Bohge Engine License (BEL)
//
//	Copyright (c) 2011-2014 Peng Zhao
//
//	Permission is hereby granted, free of charge, to any person obtaining a copy
//	of this software and associated documentation files (the "Software"), to deal
//	in the Software without restriction, including without limitation the rights
//	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//	copies of the Software, and to permit persons to whom the Software is
//	furnished to do so, subject to the following conditions:
//
//	The above copyright notice and this permission notice shall be included in 
//	all copies or substantial portions of the Software. And the logo of 
//	Bohge Engine shall be displayed full screen for more than 3 seconds 
//	when the software is started. Copyright holders are allowed to develop 
//	game edit based on Bohge Engine, The edit must be released under the MIT 
//	open source license if it is going to be published. In no event shall 
//	copyright holders be prohibited from using any code of Bohge Engine 
//	to develop any other analogous game engines.
//
//	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//
//////////////////////////////////////////////////////////////////////////////////////



#include "fft.h"
#include "3DMath.h"

namespace BohgeEngine
{
	cFFT::cFFT(unsigned int N) : N(N), reversed(0), T(0), pi2(2 * PI) {
		c[0] = c[1] = 0;

		log_2_N = Math::Log((float)N)/Math::Log(2.0);

		reversed = new unsigned int[N];		// prep bit reversals
		for (int i = 0; i < N; i++) reversed[i] = reverse(i);

		int pow2 = 1;
		T = new complex*[log_2_N];		// prep T
		for (int i = 0; i < log_2_N; i++) {
			T[i] = new complex[pow2];
			for (int j = 0; j < pow2; j++) T[i][j] = t(j, pow2 * 2);
			pow2 *= 2;
		}

		c[0] = new complex[N];
		c[1] = new complex[N];
		which = 0;
	}

	cFFT::~cFFT() {
		if (c[0]) delete [] c[0];
		if (c[1]) delete [] c[1];
		if (T) {
			for (int i = 0; i < log_2_N; i++) if (T[i]) delete [] T[i];
			delete [] T;
		}
		if (reversed) delete [] reversed;
	}

	unsigned int cFFT::reverse(unsigned int i) {
		unsigned int res = 0;
		for (int j = 0; j < log_2_N; j++) {
			res = (res << 1) + (i & 1);
			i >>= 1;
		}
		return res;
	}

	complex cFFT::t(unsigned int x, unsigned int N) {
		return complex(Math::Cos(pi2 * x / N), Math::Sin(pi2 * x / N));
	}

	void cFFT::fft(complex* input, complex* output, int stride, int offset) {
		for (int i = 0; i < N; i++) c[which][i] = input[reversed[i] * stride + offset];

		int loops       = N>>1;
		int size        = 1<<1;
		int size_over_2 = 1;
		int w_          = 0;
		for (int i = 1; i <= log_2_N; i++) {
			which ^= 1;
			for (int j = 0; j < loops; j++) {
				for (int k = 0; k < size_over_2; k++) {
					c[which][size * j + k] =  c[which^1][size * j + k] +
						c[which^1][size * j + size_over_2 + k] * T[w_][k];
				}

				for (int k = size_over_2; k < size; k++) {
					c[which][size * j + k] =  c[which^1][size * j - size_over_2 + k] -
						c[which^1][size * j + k] * T[w_][k - size_over_2];
				}
			}
			loops       >>= 1;
			size        <<= 1;
			size_over_2 <<= 1;
			w_++;
		}

		for (int i = 0; i < N; i++) output[i * stride + offset] = c[which][i];
	}
}