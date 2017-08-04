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
#include "Plugindefine.h"

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


//最好不要直接调用这里的函数，通过Math域名调用
BOHGE_EXTERN BOHGE_FRAMEWORK F_Cpf_Cpf		_Neon_dot2;
BOHGE_EXTERN BOHGE_FRAMEWORK F_Cpf_Cpf		_Neon_dot3;
BOHGE_EXTERN BOHGE_FRAMEWORK F_Cpf_Cpf		_Neon_dot4;
BOHGE_EXTERN BOHGE_FRAMEWORK V_Cpf_Cpf_Pf	_Neon_cross3;
BOHGE_EXTERN BOHGE_FRAMEWORK V_Cpf_Pf		_Neon_normalize2;
BOHGE_EXTERN BOHGE_FRAMEWORK V_Cpf_Pf		_Neon_normalize3;
BOHGE_EXTERN BOHGE_FRAMEWORK V_Cpf_Pf		_Neon_normalize4;
BOHGE_EXTERN BOHGE_FRAMEWORK F_F			_Neon_sinf;
BOHGE_EXTERN BOHGE_FRAMEWORK F_F 			_Neon_cosf;
BOHGE_EXTERN BOHGE_FRAMEWORK F_F 			_Neon_tanf;
BOHGE_EXTERN BOHGE_FRAMEWORK F_F 			_Neon_atanf;
BOHGE_EXTERN BOHGE_FRAMEWORK F_F 			_Neon_asinf;
BOHGE_EXTERN BOHGE_FRAMEWORK F_F 			_Neon_acosf;
BOHGE_EXTERN BOHGE_FRAMEWORK F_F 			_Neon_expf;
BOHGE_EXTERN BOHGE_FRAMEWORK F_F 			_Neon_logf;
BOHGE_EXTERN BOHGE_FRAMEWORK F_F 			_Neon_log10f;
BOHGE_EXTERN BOHGE_FRAMEWORK F_F 			_Neon_floorf;
BOHGE_EXTERN BOHGE_FRAMEWORK F_F 			_Neon_ceilf;
BOHGE_EXTERN BOHGE_FRAMEWORK F_F 			_Neon_fabsf;
BOHGE_EXTERN BOHGE_FRAMEWORK F_F 			_Neon_sqrtf;
BOHGE_EXTERN BOHGE_FRAMEWORK F_F 			_Neon_invsqrtf;
BOHGE_EXTERN BOHGE_FRAMEWORK F_F_F			_Neon_powf;
BOHGE_EXTERN BOHGE_FRAMEWORK F_F_F			_Neon_fmodf;
BOHGE_EXTERN BOHGE_FRAMEWORK F_F_I			_Neon_ldexpf;
BOHGE_EXTERN BOHGE_FRAMEWORK F_F_Pi			_Neon_frexpf;
BOHGE_EXTERN BOHGE_FRAMEWORK F_F_Pi			_Neon_modf;
BOHGE_EXTERN BOHGE_FRAMEWORK V_F_Pf			_Neon_sincosf;



BOHGE_EXTERN BOHGE_FRAMEWORK void InitializeNeonMath();//初始化数学库
BOHGE_EXTERN BOHGE_FRAMEWORK void enable_runfast();
BOHGE_EXTERN BOHGE_FRAMEWORK float dot2_c( const float v0[2], const float v1[2]);
BOHGE_EXTERN BOHGE_FRAMEWORK float dot2_neon_sfp( const float v0[2], const float v1[2]);
BOHGE_EXTERN BOHGE_FRAMEWORK float dot2_neon_hfp( const float v0[2], const float v1[2]);
BOHGE_EXTERN BOHGE_FRAMEWORK float dot3_c(const float v0[3], const float v1[3]);
BOHGE_EXTERN BOHGE_FRAMEWORK float dot3_neon_sfp(const float v0[3], const float v1[3]);
BOHGE_EXTERN BOHGE_FRAMEWORK float dot3_neon_hfp(const float v0[3], const float v1[3]);
BOHGE_EXTERN BOHGE_FRAMEWORK float dot4_c(const float v0[4], const float v1[4]);
BOHGE_EXTERN BOHGE_FRAMEWORK float dot4_neon_sfp(const float v0[4], const float v1[4]);
BOHGE_EXTERN BOHGE_FRAMEWORK float dot4_neon_hfp(const float v0[4], const float v1[4]);
BOHGE_EXTERN BOHGE_FRAMEWORK void cross3_c( const float v0[3], const float v1[3], float d[3]);
BOHGE_EXTERN BOHGE_FRAMEWORK void cross3_neon_sfp( const float v0[3], const float v1[3], float d[3]);
BOHGE_EXTERN BOHGE_FRAMEWORK void cross3_neon_hfp( const float v0[3], const float v1[3], float d[3]);
BOHGE_EXTERN BOHGE_FRAMEWORK void normalize2_c(const float v[2], float d[2]);
BOHGE_EXTERN BOHGE_FRAMEWORK void normalize2_neon_sfp(const float v[2], float d[2]);
BOHGE_EXTERN BOHGE_FRAMEWORK void normalize2_neon_hfp(const float v[2], float d[2]);
BOHGE_EXTERN BOHGE_FRAMEWORK void normalize3_c(const float v[3], float d[3]);
BOHGE_EXTERN BOHGE_FRAMEWORK void normalize3_neon_sfp(const float v[3], float d[3]);
BOHGE_EXTERN BOHGE_FRAMEWORK void normalize3_neon_hfp(const float v[3], float d[3]);
BOHGE_EXTERN BOHGE_FRAMEWORK void normalize4_c(const float v[4], float d[4]);
BOHGE_EXTERN BOHGE_FRAMEWORK void normalize4_neon_sfp(const float v[4], float d[4]);
BOHGE_EXTERN BOHGE_FRAMEWORK void normalize4_neon_hfp(const float v[4], float d[4]);
BOHGE_EXTERN BOHGE_FRAMEWORK void matmul2_c( const float m0[4], const float m1[4], float d[4]);
BOHGE_EXTERN BOHGE_FRAMEWORK void matmul2_neon_sfp( const float m0[4], const float m1[4], float d[4]);
BOHGE_EXTERN BOHGE_FRAMEWORK void matmul2_neon_hfp( const float m0[4], const float m1[4], float d[4]);
BOHGE_EXTERN BOHGE_FRAMEWORK void matmul3_c( const float m0[9], const float m1[9], float d[9]);
BOHGE_EXTERN BOHGE_FRAMEWORK void matmul3_neon_sfp(const float m0[9], const float m1[9], float d[9]);
BOHGE_EXTERN BOHGE_FRAMEWORK void matmul3_neon_hfp(const float m0[9], const float m1[9], float d[9]);
BOHGE_EXTERN BOHGE_FRAMEWORK void matmul4_c(const float m0[16], const float m1[16], float d[16]);
BOHGE_EXTERN BOHGE_FRAMEWORK void matmul4_neon_sfp(const float m0[16], const float m1[16], float d[16]);
BOHGE_EXTERN BOHGE_FRAMEWORK void matmul4_neon_hfp(const float m0[16], const float m1[16], float d[16]);
BOHGE_EXTERN BOHGE_FRAMEWORK void matvec2_c(const float m[4], const float v[2], float d[2]);
BOHGE_EXTERN BOHGE_FRAMEWORK void matvec2_neon_sfp(const float m[4], const float v[2], float d[2]);
BOHGE_EXTERN BOHGE_FRAMEWORK void matvec2_neon_hfp(const float m[4], const float v[2], float d[2]);
BOHGE_EXTERN BOHGE_FRAMEWORK void matvec3_c(const float m[9], const float v[3], float d[3]);
BOHGE_EXTERN BOHGE_FRAMEWORK void matvec3_neon_sfp(const float m[9], const float v[3], float d[3]);
BOHGE_EXTERN BOHGE_FRAMEWORK void matvec3_neon_hfp(const float m[9], const float v[3], float d[3]);
BOHGE_EXTERN BOHGE_FRAMEWORK void matvec4_c(const float m[16], const float v[4], float d[4]);
BOHGE_EXTERN BOHGE_FRAMEWORK void matvec4_neon_sfp(const float m[16], const float v[4], float d[4]);
BOHGE_EXTERN BOHGE_FRAMEWORK void matvec4_neon_hfp(const float m[16], const float v[4], float d[4]);
BOHGE_EXTERN BOHGE_FRAMEWORK float sinf_c(float x);
BOHGE_EXTERN BOHGE_FRAMEWORK float sinf_neon_hfp(float x);
BOHGE_EXTERN BOHGE_FRAMEWORK float sinf_neon_sfp(float x);
BOHGE_EXTERN BOHGE_FRAMEWORK float cosf_c(float x);
BOHGE_EXTERN BOHGE_FRAMEWORK float cosf_neon_hfp(float x);
BOHGE_EXTERN BOHGE_FRAMEWORK float cosf_neon_sfp(float x);
BOHGE_EXTERN BOHGE_FRAMEWORK void sincosf_c(float x, float r[2]);
BOHGE_EXTERN BOHGE_FRAMEWORK void sincosf_neon_hfp(float x, float r[2]);
BOHGE_EXTERN BOHGE_FRAMEWORK void sincosf_neon_sfp(float x, float r[2]);
BOHGE_EXTERN BOHGE_FRAMEWORK void sinfv_c(float *x, int n, float *r);
BOHGE_EXTERN BOHGE_FRAMEWORK void sinfv_neon(float *x, int n, float *r);
BOHGE_EXTERN BOHGE_FRAMEWORK float tanf_c(float x);
BOHGE_EXTERN BOHGE_FRAMEWORK float tanf_neon_hfp(float x);
BOHGE_EXTERN BOHGE_FRAMEWORK float tanf_neon_sfp(float x);
BOHGE_EXTERN BOHGE_FRAMEWORK float atanf_c(float x);
BOHGE_EXTERN BOHGE_FRAMEWORK float atanf_neon_hfp(float x);
BOHGE_EXTERN BOHGE_FRAMEWORK float atanf_neon_sfp(float x);
BOHGE_EXTERN BOHGE_FRAMEWORK float atan2f_c(float y, float x);
BOHGE_EXTERN BOHGE_FRAMEWORK float atan2f_neon_hfp(float y, float x);
BOHGE_EXTERN BOHGE_FRAMEWORK float atan2f_neon_sfp(float y, float x);
BOHGE_EXTERN BOHGE_FRAMEWORK float asinf_c(float x);
BOHGE_EXTERN BOHGE_FRAMEWORK float asinf_neon_hfp(float x);
BOHGE_EXTERN BOHGE_FRAMEWORK float asinf_neon_sfp(float x);
BOHGE_EXTERN BOHGE_FRAMEWORK float acosf_c(float x);
BOHGE_EXTERN BOHGE_FRAMEWORK float acosf_neon_hfp(float x);
BOHGE_EXTERN BOHGE_FRAMEWORK float acosf_neon_sfp(float x);
BOHGE_EXTERN BOHGE_FRAMEWORK float sinhf_c(float x);
BOHGE_EXTERN BOHGE_FRAMEWORK float sinhf_neon_hfp(float x);
BOHGE_EXTERN BOHGE_FRAMEWORK float sinhf_neon_sfp(float x);
BOHGE_EXTERN BOHGE_FRAMEWORK float coshf_c(float x);
BOHGE_EXTERN BOHGE_FRAMEWORK float coshf_neon_hfp(float x);
BOHGE_EXTERN BOHGE_FRAMEWORK float coshf_neon_sfp(float x);
BOHGE_EXTERN BOHGE_FRAMEWORK float tanhf_c(float x);
BOHGE_EXTERN BOHGE_FRAMEWORK float tanhf_neon_hfp(float x);
BOHGE_EXTERN BOHGE_FRAMEWORK float tanhf_neon_sfp(float x);
BOHGE_EXTERN BOHGE_FRAMEWORK float expf_c(float x);
BOHGE_EXTERN BOHGE_FRAMEWORK float expf_neon_hfp(float x);
BOHGE_EXTERN BOHGE_FRAMEWORK float expf_neon_sfp(float x);
BOHGE_EXTERN BOHGE_FRAMEWORK float logf_c(float x);
BOHGE_EXTERN BOHGE_FRAMEWORK float logf_neon_hfp(float x);
BOHGE_EXTERN BOHGE_FRAMEWORK float logf_neon_sfp(float x);
BOHGE_EXTERN BOHGE_FRAMEWORK float log10f_c(float x);
BOHGE_EXTERN BOHGE_FRAMEWORK float log10f_neon_hfp(float x);
BOHGE_EXTERN BOHGE_FRAMEWORK float log10f_neon_sfp(float x);
BOHGE_EXTERN BOHGE_FRAMEWORK float powf_c(float x, float n);
BOHGE_EXTERN BOHGE_FRAMEWORK float powf_neon_sfp(float x, float n);
BOHGE_EXTERN BOHGE_FRAMEWORK float powf_neon_hfp(float x, float n);
BOHGE_EXTERN BOHGE_FRAMEWORK float floorf_c(float x);
BOHGE_EXTERN BOHGE_FRAMEWORK float floorf_neon_sfp(float x);
BOHGE_EXTERN BOHGE_FRAMEWORK float floorf_neon_hfp(float x);
BOHGE_EXTERN BOHGE_FRAMEWORK float ceilf_c(float x);
BOHGE_EXTERN BOHGE_FRAMEWORK float ceilf_neon_hfp(float x);
BOHGE_EXTERN BOHGE_FRAMEWORK float ceilf_neon_sfp(float x);
BOHGE_EXTERN BOHGE_FRAMEWORK float fabsf_c(float x);
BOHGE_EXTERN BOHGE_FRAMEWORK float fabsf_neon_hfp(float x);
BOHGE_EXTERN BOHGE_FRAMEWORK float fabsf_neon_sfp(float x);
BOHGE_EXTERN BOHGE_FRAMEWORK float ldexpf_c(float m, int e);
BOHGE_EXTERN BOHGE_FRAMEWORK float ldexpf_neon_hfp(float m, int e);
BOHGE_EXTERN BOHGE_FRAMEWORK float ldexpf_neon_sfp(float m, int e);
BOHGE_EXTERN BOHGE_FRAMEWORK float frexpf_c(float x, int *e);
BOHGE_EXTERN BOHGE_FRAMEWORK float frexpf_neon_hfp(float x, int *e);
BOHGE_EXTERN BOHGE_FRAMEWORK float frexpf_neon_sfp(float x, int *e);
BOHGE_EXTERN BOHGE_FRAMEWORK float fmodf_c(float x, float y);
BOHGE_EXTERN BOHGE_FRAMEWORK float fmodf_neon_hfp(float x, float y);
BOHGE_EXTERN BOHGE_FRAMEWORK float fmodf_neon_sfp(float x, float y);
BOHGE_EXTERN BOHGE_FRAMEWORK float modf_c(float x, int *i);
BOHGE_EXTERN BOHGE_FRAMEWORK float modf_neon_hfp(float x, int *i);
BOHGE_EXTERN BOHGE_FRAMEWORK float modf_neon_sfp(float x, int *i);
BOHGE_EXTERN BOHGE_FRAMEWORK float sqrtf_c(float x);
BOHGE_EXTERN BOHGE_FRAMEWORK float sqrtf_neon_hfp(float x);
BOHGE_EXTERN BOHGE_FRAMEWORK float sqrtf_neon_sfp(float x);
BOHGE_EXTERN BOHGE_FRAMEWORK float invsqrtf_c(float x);
BOHGE_EXTERN BOHGE_FRAMEWORK float invsqrtf_neon_hfp(float x);
BOHGE_EXTERN BOHGE_FRAMEWORK float invsqrtf_neon_sfp(float x);
