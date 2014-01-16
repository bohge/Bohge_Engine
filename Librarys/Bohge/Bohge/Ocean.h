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



#pragma once
#include "3DMath.h"
#include "fft.h"
#include "complex.h"


namespace BohgeEngine
{
	struct vertex_ocean {
		float   x,   y,   z; // vertex
		float  nx,  ny,  nz; // normal
		float   a,   b,   c; // htilde0
		float  _a,  _b,  _c; // htilde0mk conjugate
		float  ox,  oy,  oz; // original position
	};




	struct complex_vector_normal {	// structure used with discrete fourier transform
		complex h;		// wave height
		vector2f D;		// displacement
		vector3f n;		// normal
	};




	class Ocean {
	private:
		bool geometry;				// flag to render geometry or surface

		float g;				// gravity constant
		int N, Nplus1;				// dimension -- N should be a power of 2
		float A;				// phillips spectrum parameter -- affects heights of waves
		vector2f w;				// wind parameter
		float length;				// length parameter
		complex *h_tilde,			// for fast fourier transform
			*h_tilde_slopex, *h_tilde_slopez,
			*h_tilde_dx, *h_tilde_dz;
		cFFT *fft;				// fast fourier transform

		vertex_ocean *vertices;			// vertices for vertex buffer object
		unsigned int *indices;			// indicies for vertex buffer object
		unsigned int indices_count;		// number of indices to render
		uint vbo_vertices, vbo_indices;	// vertex buffer objects

		uint glProgram, glShaderV, glShaderF;	// shaders
		int vertex, normal, texture, light_position, projection, view, model;	// attributes and uniforms
	public:
		BOHGE_FORCEINLINE vertex_ocean* Vertexs()
		{
			return vertices;
		}
	public:
		Ocean(const int N, const float A, const vector2f w, const float length, bool geometry);
		~Ocean();

		float dispersion(int n_prime, int m_prime);		// deep water
		float phillips(int n_prime, int m_prime);		// phillips spectrum
		complex hTilde_0(int n_prime, int m_prime);
		complex hTilde(float t, int n_prime, int m_prime);
		complex_vector_normal h_D_and_n(vector2f x, float t);
		void evaluateWavesFFT(float t);
	};

	Ocean::Ocean(const int N, const float A, const vector2f w, const float length, const bool geometry) :
	g(9.81), geometry(geometry), N(N), Nplus1(N+1), A(A), w(w), length(length),
		vertices(0), indices(0), h_tilde(0), h_tilde_slopex(0), h_tilde_slopez(0), h_tilde_dx(0), h_tilde_dz(0), fft(0)
	{
		h_tilde        = new complex[N*N];
		h_tilde_slopex = new complex[N*N];
		h_tilde_slopez = new complex[N*N];
		h_tilde_dx     = new complex[N*N];
		h_tilde_dz     = new complex[N*N];
		fft            = new cFFT(N);
		vertices       = new vertex_ocean[Nplus1*Nplus1];
		indices        = new unsigned int[Nplus1*Nplus1*10];

		int index;

		complex htilde0, htilde0mk_conj;
		for (int m_prime = 0; m_prime < Nplus1; m_prime++) {
			for (int n_prime = 0; n_prime < Nplus1; n_prime++) {
				index = m_prime * Nplus1 + n_prime;

				htilde0        = hTilde_0( n_prime,  m_prime);
				htilde0mk_conj = hTilde_0(-n_prime, -m_prime).conj();

				vertices[index].a  = htilde0.a;
				vertices[index].b  = htilde0.b;
				vertices[index]._a = htilde0mk_conj.a;
				vertices[index]._b = htilde0mk_conj.b;

				vertices[index].ox = vertices[index].x =  (n_prime - N / 2.0f) * length / N;
				vertices[index].oy = vertices[index].y =  0.0f;
				vertices[index].oz = vertices[index].z =  (m_prime - N / 2.0f) * length / N;

				vertices[index].nx = 0.0f;
				vertices[index].ny = 1.0f;
				vertices[index].nz = 0.0f;
			}
		}

		indices_count = 0;
		for (int m_prime = 0; m_prime < N; m_prime++) {
			for (int n_prime = 0; n_prime < N; n_prime++) {
				index = m_prime * Nplus1 + n_prime;

				if (geometry) {
					indices[indices_count++] = index;				// lines
					indices[indices_count++] = index + 1;
					indices[indices_count++] = index;
					indices[indices_count++] = index + Nplus1;
					indices[indices_count++] = index;
					indices[indices_count++] = index + Nplus1 + 1;
					if (n_prime == N - 1) {
						indices[indices_count++] = index + 1;
						indices[indices_count++] = index + Nplus1 + 1;
					}
					if (m_prime == N - 1) {
						indices[indices_count++] = index + Nplus1;
						indices[indices_count++] = index + Nplus1 + 1;
					}
				} else {
					indices[indices_count++] = index;				// two triangles
					indices[indices_count++] = index + Nplus1;
					indices[indices_count++] = index + Nplus1 + 1;
					indices[indices_count++] = index;
					indices[indices_count++] = index + Nplus1 + 1;
					indices[indices_count++] = index + 1;
				}
			}
		}
	}

	Ocean::~Ocean() {
		if (h_tilde)		delete [] h_tilde;
		if (h_tilde_slopex)	delete [] h_tilde_slopex;
		if (h_tilde_slopez)	delete [] h_tilde_slopez;
		if (h_tilde_dx)		delete [] h_tilde_dx;
		if (h_tilde_dz)		delete [] h_tilde_dz;
		if (fft)		delete fft;
		if (vertices)		delete [] vertices;
		if (indices)		delete [] indices;
	}

	float Ocean::dispersion(int n_prime, int m_prime) {
		float w_0 = 2.0f * PI / 200.0f;
		float kx = PI * (2 * n_prime - N) / length;
		float kz = PI * (2 * m_prime - N) / length;
		return floor(Math::Sqrt(g * Math::Sqrt(kx * kx + kz * kz)) / w_0) * w_0;
	}

	float Ocean::phillips(int n_prime, int m_prime) {
		vector2f k(PI * (2 * n_prime - N) / length,
			PI * (2 * m_prime - N) / length);
		float k_length  = k.Length();
		if (k_length < 0.000001) return 0.0;

		float k_length2 = k_length  * k_length;
		float k_length4 = k_length2 * k_length2;

		float k_dot_w   = k.Normalize().Dot( w.Normalize() );
		float k_dot_w2  = k_dot_w * k_dot_w * k_dot_w * k_dot_w * k_dot_w * k_dot_w;

		float w_length  = w.Length();
		float L         = w_length * w_length / g;
		float L2        = L * L;

		float damping   = 0.001;
		float l2        = L2 * damping * damping;

		return A * exp(-1.0f / (k_length2 * L2)) / k_length4 * k_dot_w2 * exp(-k_length2 * l2);
	}

	complex gaussianRandomVariable() {
		float x1, x2, w;
		do {
			x1 = 2.f * (float)rand()/RAND_MAX - 1.f;
			x2 = 2.f * (float)rand()/RAND_MAX - 1.f;
			w = x1 * x1 + x2 * x2;
		} while ( w >= 1.f );
		w = Math::Sqrt((-2.f * log(w)) / w);
		return complex(x1 * w, x2 * w);
	}
	complex Ocean::hTilde_0(int n_prime, int m_prime) {
		complex r = gaussianRandomVariable();
		return r * Math::Sqrt(phillips(n_prime, m_prime) / 2.0f);
	}

	complex Ocean::hTilde(float t, int n_prime, int m_prime) {
		int index = m_prime * Nplus1 + n_prime;

		complex htilde0(vertices[index].a,  vertices[index].b);
		complex htilde0mkconj(vertices[index]._a, vertices[index]._b);

		float omegat = dispersion(n_prime, m_prime) * t;

		//float cos_ = cos(omegat);
		//float sin_ = sin(omegat);
		float sc[2];
		Math::SinCos( omegat, sc );

		complex c0( sc[1],  sc[0] );
		complex c1( sc[1], -sc[0] );

		complex res = htilde0 * c0 + htilde0mkconj * c1;

		return htilde0 * c0 + htilde0mkconj*c1;
	}

	complex_vector_normal Ocean::h_D_and_n(vector2f x, float t) {
		complex h(0.0f, 0.0f);
		vector2f D(0.0f, 0.0f);
		vector3f n(0.0f, 0.0f, 0.0f);

		complex c, res, htilde_c;
		vector2f k;
		float kx, kz, k_length, k_dot_x;
		float sc[2];
		for (int m_prime = 0; m_prime < N; m_prime++) {
			kz = 2.0f * PI * (m_prime - N / 2.0f) / length;
			for (int n_prime = 0; n_prime < N; n_prime++) {
				kx = 2.0f * PI * (n_prime - N / 2.0f) / length;
				k = vector2f(kx, kz);

				k_length = k.Length();
				k_dot_x = k.Dot( x );

				//c = complex(cos(k_dot_x), sin(k_dot_x));
				Math::SinCos( k_dot_x, sc );
				c = complex( sc[1], sc[0] );
				htilde_c = hTilde(t, n_prime, m_prime) * c;

				h = h + htilde_c;

				n = n + vector3f(-kx * htilde_c.b, 0.0f, -kz * htilde_c.b);

				if (k_length < 0.000001) continue;
				D = D + vector2f(kx / k_length * htilde_c.b, kz / k_length * htilde_c.b);
			}
		}

		n = (vector3f(0.0f, 1.0f, 0.0f) - n);
		n.NormalizeSelf();

		complex_vector_normal cvn;
		cvn.h = h;
		cvn.D = D;
		cvn.n = n;
		return cvn;
	}

	void Ocean::evaluateWavesFFT(float t) {
		float kx, kz, len, lambda = -1.0f;
		int index, index1;

		for (int m_prime = 0; m_prime < N; m_prime++) {
			kz = PI * (2.0f * m_prime - N) / length;
			for (int n_prime = 0; n_prime < N; n_prime++) {
				kx = PI*(2 * n_prime - N) / length;
				len = Math::Sqrt(kx * kx + kz * kz);
				index = m_prime * N + n_prime;

				h_tilde[index] = hTilde(t, n_prime, m_prime);
				h_tilde_slopex[index] = h_tilde[index] * complex(0, kx);
				h_tilde_slopez[index] = h_tilde[index] * complex(0, kz);
				if (len < 0.000001f) {
					h_tilde_dx[index]     = complex(0.0f, 0.0f);
					h_tilde_dz[index]     = complex(0.0f, 0.0f);
				} else {
					h_tilde_dx[index]     = h_tilde[index] * complex(0, -kx/len);
					h_tilde_dz[index]     = h_tilde[index] * complex(0, -kz/len);
				}
			}
		}

		for (int m_prime = 0; m_prime < N; m_prime++) {
			fft->fft(h_tilde, h_tilde, 1, m_prime * N);
			fft->fft(h_tilde_slopex, h_tilde_slopex, 1, m_prime * N);
			fft->fft(h_tilde_slopez, h_tilde_slopez, 1, m_prime * N);
			fft->fft(h_tilde_dx, h_tilde_dx, 1, m_prime * N);
			fft->fft(h_tilde_dz, h_tilde_dz, 1, m_prime * N);
		}
		for (int n_prime = 0; n_prime < N; n_prime++) {
			fft->fft(h_tilde, h_tilde, N, n_prime);
			fft->fft(h_tilde_slopex, h_tilde_slopex, N, n_prime);
			fft->fft(h_tilde_slopez, h_tilde_slopez, N, n_prime);
			fft->fft(h_tilde_dx, h_tilde_dx, N, n_prime);
			fft->fft(h_tilde_dz, h_tilde_dz, N, n_prime);
		}

		int sign;
		float signs[] = { 1.0f, -1.0f };
		vector3f n;
		for (int m_prime = 0; m_prime < N; m_prime++) {
			for (int n_prime = 0; n_prime < N; n_prime++) {
				index  = m_prime * N + n_prime;		// index into h_tilde..
				index1 = m_prime * Nplus1 + n_prime;	// index into vertices

				sign = signs[(n_prime + m_prime) & 1];

				h_tilde[index]     = h_tilde[index] * sign;

				// height
				vertices[index1].y = h_tilde[index].a;

				// displacement
				h_tilde_dx[index] = h_tilde_dx[index] * sign;
				h_tilde_dz[index] = h_tilde_dz[index] * sign;
				vertices[index1].x = vertices[index1].ox + h_tilde_dx[index].a * lambda;
				vertices[index1].z = vertices[index1].oz + h_tilde_dz[index].a * lambda;

				// normal
				h_tilde_slopex[index] = h_tilde_slopex[index] * sign;
				h_tilde_slopez[index] = h_tilde_slopez[index] * sign;
				n = vector3f(0.0f - h_tilde_slopex[index].a, 1.0f, 0.0f - h_tilde_slopez[index].a);
				n.NormalizeSelf();
				vertices[index1].nx =  n.m_x;
				vertices[index1].ny =  n.m_y;
				vertices[index1].nz =  n.m_z;

				// for tiling
				if (n_prime == 0 && m_prime == 0) {
					vertices[index1 + N + Nplus1 * N].y = h_tilde[index].a;

					vertices[index1 + N + Nplus1 * N].x = vertices[index1 + N + Nplus1 * N].ox + h_tilde_dx[index].a * lambda;
					vertices[index1 + N + Nplus1 * N].z = vertices[index1 + N + Nplus1 * N].oz + h_tilde_dz[index].a * lambda;

					vertices[index1 + N + Nplus1 * N].nx =  n.m_x;
					vertices[index1 + N + Nplus1 * N].ny =  n.m_y;
					vertices[index1 + N + Nplus1 * N].nz =  n.m_z;
				}
				if (n_prime == 0) {
					vertices[index1 + N].y = h_tilde[index].a;

					vertices[index1 + N].x = vertices[index1 + N].ox + h_tilde_dx[index].a * lambda;
					vertices[index1 + N].z = vertices[index1 + N].oz + h_tilde_dz[index].a * lambda;

					vertices[index1 + N].nx =  n.m_x;
					vertices[index1 + N].ny =  n.m_y;
					vertices[index1 + N].nz =  n.m_z;
				}
				if (m_prime == 0) {
					vertices[index1 + Nplus1 * N].y = h_tilde[index].a;

					vertices[index1 + Nplus1 * N].x = vertices[index1 + Nplus1 * N].ox + h_tilde_dx[index].a * lambda;
					vertices[index1 + Nplus1 * N].z = vertices[index1 + Nplus1 * N].oz + h_tilde_dz[index].a * lambda;

					vertices[index1 + Nplus1 * N].nx =  n.m_x;
					vertices[index1 + Nplus1 * N].ny =  n.m_y;
					vertices[index1 + Nplus1 * N].nz =  n.m_z;
				}
			}
		}
	}

}