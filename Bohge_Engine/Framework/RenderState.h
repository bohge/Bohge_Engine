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
#include "ISharedResource.h"
#include "MaterialProperty.h"


namespace BohgeEngine
{
	class RenderState
	{
	private:
		bool							m_isDepthTest;
		bool							m_isDepthMask;
		MaterialProperty::Function		m_DepthFunction;
		MaterialProperty::CullFaceMode	m_isCullFace;

		bool							m_isAlphaEnable;
		bool							m_isAlphaTest;
		MaterialProperty::Blend			m_Src;
		MaterialProperty::Blend			m_Des;
		MaterialProperty::Blend			m_SrcA;
		MaterialProperty::Blend			m_DesA;

		bool							m_ColorMask_R;
		bool							m_ColorMask_G;
		bool							m_ColorMask_B;
		bool							m_ColorMask_A;

		bool							m_isStencil;
		bitmask							m_StencilMask;
		MaterialProperty::Function		m_StencilFunc;
		uint							m_StencilFuncRef;
		bitmask							m_StencilFuncMask;
		MaterialProperty::Operation		m_StencilFail;
		MaterialProperty::Operation		m_StencilZFail;
		MaterialProperty::Operation		m_StencilZPass;

		uint							m_RenderQueue;
		uint							m_uHashCode;
	public:
		RenderState(void);
		~RenderState(void);
	public:
		void CaculateHashCode();
	public:
		BOHGE_FORCEINLINE void SetRenderQueue( uint que )
		{
			m_RenderQueue = que;
		}
		BOHGE_FORCEINLINE void SetDepthTest( bool b )
		{
			m_isDepthTest = b;
		}
		BOHGE_FORCEINLINE void SetDepthMask( bool b )
		{
			m_isDepthMask = b;
		}
		BOHGE_FORCEINLINE void SetAlphaBlend( bool b )
		{
			m_isAlphaEnable = b;
		}
		BOHGE_FORCEINLINE void SetAlphaTest( bool b )
		{
			m_isAlphaTest = b;
		}
		BOHGE_FORCEINLINE void SetDepthFunciton( MaterialProperty::Function func )
		{
			m_DepthFunction = func;
		}
		BOHGE_FORCEINLINE void SetCullFace( MaterialProperty::CullFaceMode cull )
		{
			m_isCullFace = cull;
		}
		BOHGE_FORCEINLINE void SetBlendFunction( 
			MaterialProperty::Blend src,
			MaterialProperty::Blend des,
			MaterialProperty::Blend srcA,
			MaterialProperty::Blend desA )
		{
			m_Src = src;
			m_Des = des;
			m_SrcA = srcA;
			m_DesA = desA;
		}
		BOHGE_FORCEINLINE void SetColorMask( bool r, bool g, bool b, bool a )
		{
			m_ColorMask_R = r;
			m_ColorMask_G = g;
			m_ColorMask_B = b;
			m_ColorMask_A = a;
		}
		BOHGE_FORCEINLINE void SetStencil( bool b )
		{
			m_isStencil = b;
		}
		BOHGE_FORCEINLINE void SetStencilMask(bitmask mask)
		{
			m_StencilMask = mask;
		}
		BOHGE_FORCEINLINE void SetStencilFunction(MaterialProperty::Function func, uint ref, bitmask mask)
		{
			m_StencilFunc = func;
			m_StencilFuncRef = ref;
			m_StencilFuncMask = mask;
		}
		BOHGE_FORCEINLINE void SetStencilOperation(MaterialProperty::Operation fail, MaterialProperty::Operation zfail, MaterialProperty::Operation zpass)
		{
			m_StencilFail = fail;
			m_StencilZFail = zfail;
			m_StencilZPass = zpass;
		}
		BOHGE_FORCEINLINE uint GetHashCode() const
		{
			return m_uHashCode;
		}
		BOHGE_FORCEINLINE uint GetRenderQueue() const
		{
			return m_RenderQueue;
		}
		BOHGE_FORCEINLINE bool isTransparent( int pt ) const
		{
			return m_isAlphaEnable;
		}
		BOHGE_FORCEINLINE bool GetDepthTest() const
		{
			return m_isDepthTest;
		}
		BOHGE_FORCEINLINE bool GetDepthMask() const
		{
			return m_isDepthMask;
		}
		BOHGE_FORCEINLINE MaterialProperty::Function GetDepthFunction() const
		{
			return m_DepthFunction;
		}
		BOHGE_FORCEINLINE MaterialProperty::CullFaceMode GetCullFace() const
		{
			return m_isCullFace;
		}
		BOHGE_FORCEINLINE bool GetAlphaEnable() const
		{
			return m_isAlphaEnable;
		}
		BOHGE_FORCEINLINE bool GetAlphaTest() const
		{
			return m_isAlphaTest;
		}
		BOHGE_FORCEINLINE MaterialProperty::Blend GetBlendSrc() const
		{
			return m_Src;
		}
		BOHGE_FORCEINLINE MaterialProperty::Blend GetBlendDes() const
		{
			return m_Des;
		}
		BOHGE_FORCEINLINE MaterialProperty::Blend GetBlendSrcAlpha() const
		{
			return m_SrcA;
		}
		BOHGE_FORCEINLINE MaterialProperty::Blend GetBlendDesAlpha() const
		{
			return m_DesA;
		}
		BOHGE_FORCEINLINE bool GetColorR() const
		{
			return m_ColorMask_R;
		}
		BOHGE_FORCEINLINE bool GetColorG() const
		{
			return m_ColorMask_G;
		}
		BOHGE_FORCEINLINE bool GetColorB() const
		{
			return m_ColorMask_B;
		}
		BOHGE_FORCEINLINE bool GetColorA() const
		{
			return m_ColorMask_A;
		}
		BOHGE_FORCEINLINE bool GetStencilEnable() const
		{
			return m_isStencil;
		}
		BOHGE_FORCEINLINE bitmask GetStencilMask() const
		{
			return m_StencilMask;
		}
		BOHGE_FORCEINLINE MaterialProperty::Function GetStencilFunc() const
		{
			return m_StencilFunc;
		}
		BOHGE_FORCEINLINE uint GetStencilFuncRef() const
		{
			return m_StencilFuncRef;
		}
		BOHGE_FORCEINLINE bitmask GetStencilFuncMask() const
		{
			return m_StencilFuncMask;
		}
		BOHGE_FORCEINLINE MaterialProperty::Operation GetStencilFail() const
		{
			return m_StencilFail;
		}
		BOHGE_FORCEINLINE MaterialProperty::Operation GetStencilZFail() const
		{
			return m_StencilZFail;
		}
		BOHGE_FORCEINLINE MaterialProperty::Operation GetStencilZPass() const
		{
			return m_StencilZPass;
		}
	};
}
