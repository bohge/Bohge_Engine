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




#include "NeonMath.h"
#include "Log.h"

F_Cpf_Cpf		_Neon_dot2;
F_Cpf_Cpf		_Neon_dot3;
F_Cpf_Cpf		_Neon_dot4;
V_Cpf_Cpf_Pf	_Neon_cross3;
V_Cpf_Pf		_Neon_normalize2;
V_Cpf_Pf		_Neon_normalize3;
V_Cpf_Pf		_Neon_normalize4;
F_F				_Neon_sinf;
F_F 			_Neon_cosf;
F_F 			_Neon_tanf;
F_F 			_Neon_atanf;
F_F 			_Neon_asinf;
F_F 			_Neon_acosf;
F_F 			_Neon_expf;
F_F 			_Neon_logf;
F_F 			_Neon_log10f;
F_F 			_Neon_floorf;
F_F 			_Neon_ceilf;
F_F 			_Neon_fabsf;
F_F 			_Neon_sqrtf;
F_F 			_Neon_invsqrtf;
F_F_F			_Neon_powf;
F_F_F			_Neon_fmodf;
F_F_I			_Neon_ldexpf;
F_F_Pi			_Neon_frexpf;
F_F_Pi			_Neon_modf;
V_F_Pf			_Neon_sincosf;

void _CMath()
{
	DEBUGLOG("using base c math lib!\n",1);
	_Neon_dot2 = dot2_c;
	_Neon_dot3 = dot3_c;;
	_Neon_dot4 = dot4_c;;
	_Neon_cross3 = cross3_c;
	_Neon_normalize2 = normalize2_c;
	_Neon_normalize3 = normalize3_c;
	_Neon_normalize4 = normalize4_c;
	_Neon_sinf = sinf_c;
	_Neon_cosf = cosf_c;
	_Neon_tanf = tanf_c;
	_Neon_atanf = atanf_c;
	_Neon_asinf = asinf_c;
	_Neon_acosf = acosf_c;
	_Neon_expf = expf_c;
	_Neon_logf = logf_c;
	_Neon_log10f = log10f_c;
	_Neon_floorf = floorf_c;
	_Neon_ceilf = ceilf_c;
	_Neon_fabsf = fabsf_c;
	_Neon_sqrtf = sqrtf_c;
	_Neon_invsqrtf = invsqrtf_c;
	_Neon_powf = powf_c;
	_Neon_fmodf = fmodf_c;
	_Neon_ldexpf = ldexpf_c;
	_Neon_frexpf = frexpf_c;
	_Neon_modf = modf_c;
	_Neon_sincosf = sincosf_c;
}

void NeonSoftMath()
{
	DEBUGLOG("using soft neon math lib!\n",1);
	_Neon_dot2 = dot2_neon_sfp;
	_Neon_dot3 = dot3_neon_sfp;;
	_Neon_dot4 = dot4_neon_sfp;;
	_Neon_cross3 = cross3_neon_sfp;
	_Neon_normalize2 = normalize2_neon_sfp;
	_Neon_normalize3 = normalize3_neon_sfp;
	_Neon_normalize4 = normalize4_neon_sfp;
	_Neon_sinf = sinf_neon_sfp;
	_Neon_cosf = cosf_neon_sfp;
	_Neon_tanf = tanf_neon_sfp;
	_Neon_atanf = atanf_neon_sfp;
	_Neon_asinf = asinf_neon_sfp;
	_Neon_acosf = acosf_neon_sfp;
	_Neon_expf = expf_neon_sfp;
	_Neon_logf = logf_neon_sfp;
	_Neon_log10f = log10f_neon_sfp;
	_Neon_floorf = floorf_neon_sfp;
	_Neon_ceilf = ceilf_neon_sfp;
	_Neon_fabsf = fabsf_neon_sfp;
	_Neon_sqrtf = sqrtf_neon_sfp;
	_Neon_invsqrtf = invsqrtf_neon_sfp;
	_Neon_powf = powf_neon_sfp;
	_Neon_fmodf = fmodf_neon_sfp;
	_Neon_ldexpf = ldexpf_neon_sfp;
	_Neon_frexpf = frexpf_neon_sfp;
	_Neon_modf = modf_neon_sfp;
	_Neon_sincosf = sincosf_neon_sfp;
}

void NeonHardMath()
{
	DEBUGLOG("using hard neon math lib!\n",1);
	_Neon_dot2 = dot2_neon_hfp;
	_Neon_dot3 = dot3_neon_hfp;;
	_Neon_dot4 = dot4_neon_hfp;;
	_Neon_cross3 = cross3_neon_hfp;
	_Neon_normalize2 = normalize2_neon_hfp;
	_Neon_normalize3 = normalize3_neon_hfp;
	_Neon_normalize4 = normalize4_neon_hfp;
	_Neon_sinf = sinf_neon_hfp;
	_Neon_cosf = cosf_neon_hfp;
	_Neon_tanf = tanf_neon_hfp;
	_Neon_atanf = atanf_neon_hfp;
	_Neon_asinf = asinf_neon_hfp;
	_Neon_acosf = acosf_neon_hfp;
	_Neon_expf = expf_neon_hfp;
	_Neon_logf = logf_neon_hfp;
	_Neon_log10f = log10f_neon_hfp;
	_Neon_floorf = floorf_neon_hfp;
	_Neon_ceilf = ceilf_neon_hfp;
	_Neon_fabsf = fabsf_neon_hfp;
	_Neon_sqrtf = sqrtf_neon_hfp;
	_Neon_invsqrtf = invsqrtf_neon_hfp;
	_Neon_powf = powf_neon_hfp;
	_Neon_fmodf = fmodf_neon_hfp;
	_Neon_ldexpf = ldexpf_neon_hfp;
	_Neon_frexpf = frexpf_neon_hfp;
	_Neon_modf = modf_neon_hfp;
	_Neon_sincosf = sincosf_neon_hfp;
}

#ifdef WIN32
void InitializationNeonMath()
{
	_CMath();
}
#elif ANDROID
#ifdef __NEON_MATH__
#include "cpu-features.h"
void InitializationNeonMath()
{
	DEBUGLOG("detectiv CPU family\n");
	if (android_getCpuFamily() == ANDROID_CPU_FAMILY_ARM &&
		(android_getCpuFeatures() & ANDROID_CPU_ARM_FEATURE_NEON) != 0)
	{
		// use NEON-optimized routines
#if __MATH_FPABI == 1
		NeonHardMath();
#else
		NeonSoftMath();
#endif
	}
	else
	{
		// use non-NEON fallback routines instead
		_CMath();
	}
}
#else
void InitializationNeonMath()
{
	_CMath();
}
#endif
#else
#ifdef __NEON_MATH__
void InitializationNeonMath()
{
#if __MATH_FPABI == 1
		NeonHardMath();
#else
		NeonSoftMath();
#endif
}
#else
void InitializationNeonMath()
{
	_CMath();
}
#endif
#endif
