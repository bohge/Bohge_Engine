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
//		环境相关的shader	 //
///////////////////////////////
#pragma once
#include "3DMath.h"
#include "BaseShader.h"

namespace BohgeEngine
{

	//---------------------------------------------------------------------
	class SkyBoxShader : public Shader
	{
	private:
		handle					m_hTransform;
		const Matrix44f*		m_pTransform;
		handle					m_hAlpha;
		float					m_Alpha;
	protected:
		virtual void _SetParameters();
	public:
		SkyBoxShader();
		virtual ~SkyBoxShader();
		virtual bool Initialization();
		BOHGE_FORCEINLINE void SetParamTransform(const Matrix44f* input)
		{
			m_pTransform = input;
		}
		BOHGE_FORCEINLINE void SetParamAlpha( float a )
		{
			m_Alpha = a;
		}
	};


	class Texture;
	class HaloShader : public Shader
	{
	private:
		handle				m_hProjLoc;
		handle				m_hViewProjLoc;
		handle				m_hRotation;
		handle				m_hCamerTransform;
		handle				m_hLightTexture;
		handle				m_hDepthTexture;
		handle				m_hColor;
		vector3f			m_Color;
		const Texture*		m_pLightTexture;
		const Texture*		m_pDepthTexture;
	public:
		HaloShader();
	protected:
		virtual void _SetParameters();
	public:
		virtual bool Initialization();
	public:
		BOHGE_FORCEINLINE void SetLightTexture( const Texture* t )
		{
			m_pLightTexture = t;
		}
		BOHGE_FORCEINLINE void SetDepthTexture( const Texture* t )
		{
			m_pDepthTexture = t;
		}
		BOHGE_FORCEINLINE void SetColor( const vector3f& clr )
		{
			m_Color = clr;
		}
	};


	//---------------------------------------------------------------------
	class FakeLightShader : public Shader
	{
	private:
		handle				m_hColor;
		handle				m_hProjLoc;
		handle				m_hRotation;
		handle				m_hCamerTransform;
		vector3f			m_Color;
	protected:
		virtual void _SetParameters();
	public:
		virtual bool Initialization();
	public:
		BOHGE_FORCEINLINE void SetColor( const vector3f& clr )
		{
			m_Color = clr;
		}
	};
}