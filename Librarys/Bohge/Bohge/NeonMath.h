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



//////////////////////////////////////////////
//	3DMath的拆分实现，请不要单独引用该头文件	//
//////////////////////////////////////////////
#pragma once

//打开NEON_MATH宏来使用快速的neon数学库，但是这个数学库计算结果为近似数值，
//因此不建议在使用高精度计算和较大数据时使用这个数学库
//矩阵运算由于行主序和列主序的不同，暂时不用了，使用其他的库

typedef float (* F_Cpf_Cpf)(const float* v0, const float* v1);
typedef void (* V_Cpf_Cpf_Pf)(const float* v0, const float* v1, float* d );
typedef void (* V_Cpf_Pf)(const float* v0, float* v1);
typedef float (* F_F)(float x);
typedef void (* V_F_Pf)(float x, float* d);
typedef float (* F_F_F)(float x, float d);
typedef float (* F_F_I)(float x, int d);
typedef float (* F_F_Pi)(float x, int* d);

#ifdef __cplusplus
#define EXTERN_FUNC extern "C"
#else
#define EXTERN_FUNC extern
#endif

//最好不要直接调用这里的函数，通过Math域名调用
EXTERN_FUNC F_Cpf_Cpf		_Neon_dot2;
EXTERN_FUNC F_Cpf_Cpf		_Neon_dot3;
EXTERN_FUNC F_Cpf_Cpf		_Neon_dot4;
EXTERN_FUNC V_Cpf_Cpf_Pf	_Neon_cross3;
EXTERN_FUNC V_Cpf_Pf		_Neon_normalize2;
EXTERN_FUNC V_Cpf_Pf		_Neon_normalize3;
EXTERN_FUNC V_Cpf_Pf		_Neon_normalize4;
EXTERN_FUNC F_F				_Neon_sinf;
EXTERN_FUNC F_F 			_Neon_cosf;
EXTERN_FUNC F_F 			_Neon_tanf;
EXTERN_FUNC F_F 			_Neon_atanf;
EXTERN_FUNC F_F 			_Neon_asinf;
EXTERN_FUNC F_F 			_Neon_acosf;
EXTERN_FUNC F_F 			_Neon_expf;
EXTERN_FUNC F_F 			_Neon_logf;
EXTERN_FUNC F_F 			_Neon_log10f;
EXTERN_FUNC F_F 			_Neon_floorf;
EXTERN_FUNC F_F 			_Neon_ceilf;
EXTERN_FUNC F_F 			_Neon_fabsf;
EXTERN_FUNC F_F 			_Neon_sqrtf;
EXTERN_FUNC F_F 			_Neon_invsqrtf;
EXTERN_FUNC F_F_F			_Neon_powf;
EXTERN_FUNC F_F_F			_Neon_fmodf;
EXTERN_FUNC F_F_I			_Neon_ldexpf;
EXTERN_FUNC F_F_Pi			_Neon_frexpf;
EXTERN_FUNC F_F_Pi			_Neon_modf;
EXTERN_FUNC V_F_Pf			_Neon_sincosf;



EXTERN_FUNC	void InitializationNeonMath();//初始化数学库
EXTERN_FUNC	void enable_runfast();
EXTERN_FUNC	float dot2_c( const float v0[2], const float v1[2]);
EXTERN_FUNC	float dot2_neon_sfp( const float v0[2], const float v1[2]);
EXTERN_FUNC float dot2_neon_hfp( const float v0[2], const float v1[2]);
EXTERN_FUNC float dot3_c(const float v0[3], const float v1[3]);
EXTERN_FUNC float dot3_neon_sfp(const float v0[3], const float v1[3]);
EXTERN_FUNC float dot3_neon_hfp(const float v0[3], const float v1[3]);
EXTERN_FUNC float dot4_c(const float v0[4], const float v1[4]);
EXTERN_FUNC float dot4_neon_sfp(const float v0[4], const float v1[4]);
EXTERN_FUNC float dot4_neon_hfp(const float v0[4], const float v1[4]);
EXTERN_FUNC void cross3_c( const float v0[3], const float v1[3], float d[3]);
EXTERN_FUNC void cross3_neon_sfp( const float v0[3], const float v1[3], float d[3]);
EXTERN_FUNC void cross3_neon_hfp( const float v0[3], const float v1[3], float d[3]);
EXTERN_FUNC void normalize2_c(const float v[2], float d[2]);
EXTERN_FUNC void normalize2_neon_sfp(const float v[2], float d[2]);
EXTERN_FUNC void normalize2_neon_hfp(const float v[2], float d[2]);
EXTERN_FUNC void normalize3_c(const float v[3], float d[3]);
EXTERN_FUNC void normalize3_neon_sfp(const float v[3], float d[3]);
EXTERN_FUNC void normalize3_neon_hfp(const float v[3], float d[3]);
EXTERN_FUNC void normalize4_c(const float v[4], float d[4]);
EXTERN_FUNC void normalize4_neon_sfp(const float v[4], float d[4]);
EXTERN_FUNC void normalize4_neon_hfp(const float v[4], float d[4]);
EXTERN_FUNC void matmul2_c( const float m0[4], const float m1[4], float d[4]);
EXTERN_FUNC void matmul2_neon_sfp( const float m0[4], const float m1[4], float d[4]);
EXTERN_FUNC void matmul2_neon_hfp( const float m0[4], const float m1[4], float d[4]);
EXTERN_FUNC void matmul3_c( const float m0[9], const float m1[9], float d[9]);
EXTERN_FUNC void matmul3_neon_sfp(const float m0[9], const float m1[9], float d[9]);
EXTERN_FUNC void matmul3_neon_hfp(const float m0[9], const float m1[9], float d[9]);
EXTERN_FUNC void matmul4_c(const float m0[16], const float m1[16], float d[16]);
EXTERN_FUNC void matmul4_neon_sfp(const float m0[16], const float m1[16], float d[16]);
EXTERN_FUNC void matmul4_neon_hfp(const float m0[16], const float m1[16], float d[16]);
EXTERN_FUNC void matvec2_c(const float m[4], const float v[2], float d[2]);
EXTERN_FUNC void matvec2_neon_sfp(const float m[4], const float v[2], float d[2]);
EXTERN_FUNC void matvec2_neon_hfp(const float m[4], const float v[2], float d[2]);
EXTERN_FUNC void matvec3_c(const float m[9], const float v[3], float d[3]);
EXTERN_FUNC void matvec3_neon_sfp(const float m[9], const float v[3], float d[3]);
EXTERN_FUNC void matvec3_neon_hfp(const float m[9], const float v[3], float d[3]);
EXTERN_FUNC void matvec4_c(const float m[16], const float v[4], float d[4]);
EXTERN_FUNC void matvec4_neon_sfp(const float m[16], const float v[4], float d[4]);
EXTERN_FUNC void matvec4_neon_hfp(const float m[16], const float v[4], float d[4]);
EXTERN_FUNC float sinf_c(float x);
EXTERN_FUNC float sinf_neon_hfp(float x);
EXTERN_FUNC float sinf_neon_sfp(float x);
EXTERN_FUNC float cosf_c(float x);
EXTERN_FUNC float cosf_neon_hfp(float x);
EXTERN_FUNC float cosf_neon_sfp(float x);
EXTERN_FUNC void sincosf_c(float x, float r[2]);
EXTERN_FUNC void sincosf_neon_hfp(float x, float r[2]);
EXTERN_FUNC void sincosf_neon_sfp(float x, float r[2]);
EXTERN_FUNC void sinfv_c(float *x, int n, float *r);
EXTERN_FUNC void sinfv_neon(float *x, int n, float *r);
EXTERN_FUNC float tanf_c(float x);
EXTERN_FUNC float tanf_neon_hfp(float x);
EXTERN_FUNC float tanf_neon_sfp(float x);
EXTERN_FUNC float atanf_c(float x);
EXTERN_FUNC float atanf_neon_hfp(float x);
EXTERN_FUNC float atanf_neon_sfp(float x);
EXTERN_FUNC float atan2f_c(float y, float x);
EXTERN_FUNC float atan2f_neon_hfp(float y, float x);
EXTERN_FUNC float atan2f_neon_sfp(float y, float x);
EXTERN_FUNC float asinf_c(float x);
EXTERN_FUNC float asinf_neon_hfp(float x);
EXTERN_FUNC float asinf_neon_sfp(float x);
EXTERN_FUNC float acosf_c(float x);
EXTERN_FUNC float acosf_neon_hfp(float x);
EXTERN_FUNC float acosf_neon_sfp(float x);
EXTERN_FUNC float sinhf_c(float x);
EXTERN_FUNC float sinhf_neon_hfp(float x);
EXTERN_FUNC float sinhf_neon_sfp(float x);
EXTERN_FUNC float coshf_c(float x);
EXTERN_FUNC float coshf_neon_hfp(float x);
EXTERN_FUNC float coshf_neon_sfp(float x);
EXTERN_FUNC float tanhf_c(float x);
EXTERN_FUNC float tanhf_neon_hfp(float x);
EXTERN_FUNC float tanhf_neon_sfp(float x);
EXTERN_FUNC float expf_c(float x);
EXTERN_FUNC float expf_neon_hfp(float x);
EXTERN_FUNC float expf_neon_sfp(float x);
EXTERN_FUNC float logf_c(float x);
EXTERN_FUNC float logf_neon_hfp(float x);
EXTERN_FUNC float logf_neon_sfp(float x);
EXTERN_FUNC float log10f_c(float x);
EXTERN_FUNC float log10f_neon_hfp(float x);
EXTERN_FUNC float log10f_neon_sfp(float x);
EXTERN_FUNC float powf_c(float x, float n);
EXTERN_FUNC float powf_neon_sfp(float x, float n);
EXTERN_FUNC float powf_neon_hfp(float x, float n);
EXTERN_FUNC float floorf_c(float x);
EXTERN_FUNC float floorf_neon_sfp(float x);
EXTERN_FUNC float floorf_neon_hfp(float x);
EXTERN_FUNC float ceilf_c(float x);
EXTERN_FUNC float ceilf_neon_hfp(float x);
EXTERN_FUNC float ceilf_neon_sfp(float x);
EXTERN_FUNC float fabsf_c(float x);
EXTERN_FUNC float fabsf_neon_hfp(float x);
EXTERN_FUNC float fabsf_neon_sfp(float x);
EXTERN_FUNC float ldexpf_c(float m, int e);
EXTERN_FUNC float ldexpf_neon_hfp(float m, int e);
EXTERN_FUNC float ldexpf_neon_sfp(float m, int e);
EXTERN_FUNC float frexpf_c(float x, int *e);
EXTERN_FUNC float frexpf_neon_hfp(float x, int *e);
EXTERN_FUNC float frexpf_neon_sfp(float x, int *e);
EXTERN_FUNC float fmodf_c(float x, float y);
EXTERN_FUNC float fmodf_neon_hfp(float x, float y);
EXTERN_FUNC float fmodf_neon_sfp(float x, float y);
EXTERN_FUNC float modf_c(float x, int *i);
EXTERN_FUNC float modf_neon_hfp(float x, int *i);
EXTERN_FUNC float modf_neon_sfp(float x, int *i);
EXTERN_FUNC float sqrtf_c(float x);
EXTERN_FUNC float sqrtf_neon_hfp(float x);
EXTERN_FUNC float sqrtf_neon_sfp(float x);
EXTERN_FUNC float invsqrtf_c(float x);
EXTERN_FUNC float invsqrtf_neon_hfp(float x);
EXTERN_FUNC float invsqrtf_neon_sfp(float x);
