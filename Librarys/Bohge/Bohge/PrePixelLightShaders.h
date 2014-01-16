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
//		逐像素光照的shader		   //
////////////////////////////////////
#pragma once
#include "3DMath.h"
#include "BaseShader.h"



namespace BohgeEngine
{
	//---------------------------------------------------------------------
	//没有变换矩阵的，即直接使用投影矩阵*原始顶点绘制
	class PrePixelLight : public Shader
	{
	private:
		handle			m_hWorldViewProjMatrixLoc;
		handle			m_hLightDirectionLoc;
		handle			m_hCameraPositionLoc;
		handle			m_hAmbientLoc;
		handle			m_hLightLoc;

	protected:
		virtual void _SetParameters();
	public:
		PrePixelLight();
		virtual ~PrePixelLight();
		virtual bool Initialization();
	};




	//---------------------------------------------------------------------
	//有变换矩阵的模型的prepixel light
	class PrePixelLightTransform : public Shader
	{
	private:
		handle			m_hWorldViewProjMatrixLoc;
		handle			m_hTransform;
		handle			m_hNormalTransform;
		handle			m_hLightDirectionLoc;
		handle			m_hCameraPositionLoc;
		handle			m_hAmbientLoc;
		handle			m_hLightLoc;

		handle			m_hColor;
		const vector4f*	m_pColor;

		const Matrix44f*	m_pTransform;
	protected:
		virtual void _SetParameters();
	public:
		PrePixelLightTransform();
		virtual ~PrePixelLightTransform();
		virtual bool Initialization();
	public:
		BOHGE_FORCEINLINE void SetParamTransform(const Matrix44f& input )
		{
			m_pTransform = &input;
		}
		BOHGE_FORCEINLINE void SetParamColor(const vector4f& clr)
		{
			m_pColor = &clr;
		}
	};



	//---------------------------------------------------------------------
	class PrePixelLightInstance : public Shader //Geometry instance的PrePixelLight
	{
	private:
		handle			m_hViewProjMatrixLoc;
		handle			m_hLightDirectionLoc;
		handle			m_hCameraPositionLoc;
		handle			m_hAmbientLoc;
		handle			m_hLightLoc;
	protected:
		virtual void _SetParameters();
	public:
		PrePixelLightInstance();
		virtual bool Initialization();
	};

}