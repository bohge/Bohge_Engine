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
//		阴影相关的shader		   //
////////////////////////////////////
#pragma once
#include "3DMath.h"
#include "BaseShader.h"


namespace BohgeEngine
{
	//---------------------------------------------------------------------
	class ShadowVolume : public Shader
	{
	private:
		handle              m_hLightDirectionLoc;
		handle              m_hNormalTransformLoc;
		handle              m_hModelViewProjLoc;
		handle              m_hViewProjLoc;

		const Matrix44f*	m_pTransform;
	protected:
		virtual void _SetParameters();
	public:
		ShadowVolume();
		virtual ~ShadowVolume();
		virtual bool Initialization();
	public:
		BOHGE_FORCEINLINE void SetParamTransform(const Matrix44f& input )
		{
			m_pTransform = &input;
		}
	};


	//---------------------------------------------------------------------
	class ShadowVolumeInstance : public Shader
	{
	private:
		handle              m_hLightDirectionLoc;
		handle              m_hViewProjLoc;

	protected:
		virtual void _SetParameters();
	public:
		ShadowVolumeInstance();
		virtual bool Initialization();
	};



	//---------------------------------------------------------------------
	class GaussianDepth : public Shader//四个数值的模糊，把深度拆包，效果好，没有波纹线条，适用于编辑器输出
	{
	private:
		handle				m_hStepSize;//模糊方向
		vector2f			m_vStep;
	protected:
		virtual void _SetParameters();
	public:
		GaussianDepth();
		virtual ~GaussianDepth();
		virtual bool Initialization();
	public:
		BOHGE_FORCEINLINE void SetParamStep( const vector2f& step )
		{
			m_vStep = step;
		}
	};



	//---------------------------------------------------------------------
	class GaussianDepthLow : public Shader//四个数值的模糊,这个不把深度拆包，但是会产生波纹线
	{
	private:
		handle				m_hStepSize;//模糊方向
		vector2f			m_vStep;
	protected:
		virtual void _SetParameters();
	public:
		GaussianDepthLow();
		virtual ~GaussianDepthLow();
		virtual bool Initialization();
	public:
		BOHGE_FORCEINLINE void SetParamStep( const vector2f& step )
		{
			m_vStep = step;
		}
	};
}