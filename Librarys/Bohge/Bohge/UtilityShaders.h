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



///////////////////////////////
//		基本工具shader		 //
///////////////////////////////
#pragma once
#include "3DMath.h"
#include "BaseShader.h"


namespace BohgeEngine
{
	class Texture;
	//---------------------------------------------------------------------
	class Constant : public Shader
	{
	private:
		handle				m_hModelViewProjLoc;
		handle				m_hShadowColor;

		const Matrix44f*	m_pTransform;
	protected:
		virtual void _SetParameters();
	public:
		Constant();
		virtual ~Constant();
		virtual bool Initialization();
	public:
		BOHGE_FORCEINLINE void SetParamTransform(const Matrix44f& input )
		{
			m_pTransform = &input;
		}
	};


	//---------------------------------------------------------------------
	class Combine : public Shader//这个shader所做的事颜色想加混合，而alpha按照相乘
	{
	private:
		handle				m_hTexOrginialHandle;
		handle				m_hTexSecenHandle;
		handle				m_hWeightOrginialHandle;//权重
		handle				m_hWeightSecenHandle;
		const Texture*		m_pTexOrginial;
		const Texture*		m_pTexSecen;
		float				m_fWeightOrginial;
		float				m_fWeightSecen;
	protected:
		virtual void _SetParameters();
	public:
		Combine();
		virtual ~Combine();
		virtual bool Initialization();
	public:
		BOHGE_FORCEINLINE void SetParamOrginial(const Texture* input)
		{
			m_pTexOrginial = input;
		}
		BOHGE_FORCEINLINE void SetParamSecen(const Texture* input)
		{
			m_pTexSecen = input;
		}
		BOHGE_FORCEINLINE void SetParamOrginialWeight(const float input)
		{
			m_fWeightOrginial = input;
		}
		BOHGE_FORCEINLINE void SetParamSecenWeight(const float input)
		{
			m_fWeightSecen = input;
		}
	};





	//---------------------------------------------------------------------
	class PipelineCombine : public Shader	//去除原始图像中背景处的alpha值得
	{
	private:
		handle				m_hTexOrginialHandle;
		handle				m_hTexSecenHandle;
		const Texture*		m_pTexOrginial;
		const Texture*		m_pTexSecen;
	protected:
		virtual void _SetParameters();
	public:
		PipelineCombine();
		virtual ~PipelineCombine();
		virtual bool Initialization();
	public:
		BOHGE_FORCEINLINE void SetParamOrginial(const Texture* input)
		{
			m_pTexOrginial = input;
		}
		BOHGE_FORCEINLINE void SetParamSecen(const Texture* input)
		{
			m_pTexSecen = input;
		}
	};



	//---------------------------------------------------------------------
	class QuadShader : public Shader
	{

	protected:
		virtual void _SetParameters();
	public:
		QuadShader();
		virtual ~QuadShader();
		virtual bool Initialization();
	};


	//---------------------------------------------------------------------
	class CopyDepthToRBGAShader : public MaterialShader//编辑器使用的shader
	{
	protected:
		virtual void _SetParameters();
	public:
		CopyDepthToRBGAShader();
		virtual bool Initialization();
	};





	//---------------------------------------------------------------------
	class Blur2x2Shader : public Shader
	{
	private:
		handle				m_hColorTex;
		handle				m_hStepSize;
		handle				m_hGaussWeight;
		const Texture*		m_pColorTexture;
		vector2f			m_StepSize;
		std::vector<float>	m_GaussWeight;
	protected:
		virtual void _SetParameters();
	public:
		Blur2x2Shader();
		virtual ~Blur2x2Shader();
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
		BOHGE_FORCEINLINE void SetParamGaussWeight(const std::vector<float>& input)
		{
			m_GaussWeight = input;
		}
	};
}