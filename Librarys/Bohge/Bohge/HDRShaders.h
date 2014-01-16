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



/////////////////////////////////////
//			高光的一些shader	   //
/////////////////////////////////////

#pragma once
#include "3DMath.h"
#include "BaseShader.h"


namespace BohgeEngine
{

	class Texture;
	//---------------------------------------------------------------------
	class Luminance : public Shader
	{
	protected:
		virtual void _SetParameters();
	public:
		Luminance();
		virtual ~Luminance();
		virtual bool Initialization();
	};





	//---------------------------------------------------------------------
	class Gaussian : public Shader
	{
	private:
		handle				m_hStepSize;//模糊方向
		vector2f			m_vStep;
	protected:
		virtual void _SetParameters();
	public:
		Gaussian();
		virtual ~Gaussian();
		virtual bool Initialization();
	public:
		BOHGE_FORCEINLINE void SetParamStep( const vector2f& step )
		{
			m_vStep = step;
		}
	};



	//---------------------------------------------------------------------
	class HDRGenerateLuminance : public Shader
	{
	private:
		handle				m_hColorTex;
		handle				m_hStepSize;
		const Texture*		m_pColorTexture;
		vector2f			m_StepSize;
	protected:
		virtual void _SetParameters();
	public:
		HDRGenerateLuminance();
		virtual ~HDRGenerateLuminance();
		virtual bool Initialization();
	public:
		BOHGE_FORCEINLINE void SetParamColorTexture(const Texture* input)
		{
			m_pColorTexture = input;
		}
		BOHGE_FORCEINLINE void SetParamStepSize(const vector2f& input)
		{
			m_StepSize = input;
		}
	};

	//---------------------------------------------------------------------
	class HDRAverageLuminance : public Shader
	{
	private:
		handle				m_hColorTex;
		handle				m_hStepSize;
		const Texture*		m_pColorTexture;
		vector2f			m_StepSize;
	protected:
		virtual void _SetParameters();
	public:
		HDRAverageLuminance();
		virtual ~HDRAverageLuminance();
		virtual bool Initialization();
	public:
		BOHGE_FORCEINLINE void SetParamColorTexture(const Texture* input)
		{
			m_pColorTexture = input;
		}
		BOHGE_FORCEINLINE void SetParamStepSize(const vector2f& input)
		{
			m_StepSize = input;
		}
	};

	//---------------------------------------------------------------------
	class HDRAdaptLuminance : public Shader
	{
	private:
		handle				m_hCurrentLuminance;
		handle				m_hPreviousLuminance;
		handle				m_hLambdaElapsedTime;
		handle				m_hExposureLimits;
		const Texture*		m_pCurrentLuminance;
		const Texture*		m_pPreviousLuminance;
		float				m_ExposureLimits;
		float				m_LambdaElapsedTime;
	protected:
		virtual void _SetParameters();
	public:
		HDRAdaptLuminance();
		virtual ~HDRAdaptLuminance();
		virtual bool Initialization();
	public:
		BOHGE_FORCEINLINE void SetParamCurrentLuminance(const Texture* input)
		{
			m_pCurrentLuminance = input;
		}
		BOHGE_FORCEINLINE void SetParamPreviousLuminance(const Texture* input)
		{
			m_pPreviousLuminance = input;
		}
		BOHGE_FORCEINLINE void SetParamLambdaElapsedTime( float input)
		{
			m_LambdaElapsedTime = input;
		}
		BOHGE_FORCEINLINE void SetParamExposureLimits( float input)
		{
			m_ExposureLimits = input;
		}
	};

	//---------------------------------------------------------------------
	class HDRBrightPass : public Shader
	{
	private:
		handle				m_hScratchTexture;
		handle				m_hCurrentLuminance;
		handle				m_hMiddleGray;
		handle				m_hBrightPassWhite;
		handle				m_hBrightPassThreshold;
		handle				m_hBrightPassOffset;
		const Texture*		m_pScratchTexture;
		const Texture*		m_pCurrentLuminance;
		float				m_MiddleGray;
		float				m_BrightPassWhite;
		float				m_BrightPassThreshold;
		float				m_BrightPassOffset;
	protected:
		virtual void _SetParameters();
	public:
		HDRBrightPass();
		virtual ~HDRBrightPass();
		virtual bool Initialization();
	public:
		BOHGE_FORCEINLINE void SetParamCurrentLuminance(const Texture* input)
		{
			m_pCurrentLuminance = input;
		}
		BOHGE_FORCEINLINE void SetParamScratchTexture(const Texture* input)
		{
			m_pScratchTexture = input;
		}
		BOHGE_FORCEINLINE void SetParamMiddleGray( float input)
		{
			m_MiddleGray = input;
		}
		BOHGE_FORCEINLINE void SetParamBrightPassWhite( float input)
		{
			m_BrightPassWhite = input;
		}
		BOHGE_FORCEINLINE void SetParamBrightPassThreshold( float input)
		{
			m_BrightPassThreshold = input;
		}
		BOHGE_FORCEINLINE void SetParamBrightPassOffset( float input)
		{
			m_BrightPassOffset = input;
		}
	};


	//---------------------------------------------------------------------
	class HDRCombine : public Shader
	{
	private:
		handle				m_hSharpTexture;
		handle				m_hCurrentLuminance;
		handle				m_hBloomTexture;
		handle				m_hMiddleGray;
		handle				m_hWhite;
		const Texture*		m_pSharpTexture;
		const Texture*		m_pCurrentLuminance;
		const Texture*		m_pBloomTexture;
		float				m_MiddleGray;
		float				m_White;
	protected:
		virtual void _SetParameters();
	public:
		HDRCombine();
		virtual ~HDRCombine();
		virtual bool Initialization();
	public:
		BOHGE_FORCEINLINE void SetParamCurrentLuminance(const Texture* input)
		{
			m_pCurrentLuminance = input;
		}
		BOHGE_FORCEINLINE void SetParamSharpTexture(const Texture* input)
		{
			m_pSharpTexture = input;
		}
		BOHGE_FORCEINLINE void SetParamBloomTexture(const Texture* input)
		{
			m_pBloomTexture = input;
		}
		BOHGE_FORCEINLINE void SetParamMiddleGray( float input)
		{
			m_MiddleGray = input;
		}
		BOHGE_FORCEINLINE void SetParamWhite( float input)
		{
			m_White = input;
		}
	};

}