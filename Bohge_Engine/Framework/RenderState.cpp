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
#include "RenderState.h"
#include "Utility.hpp"


#include <EASTL/vector.h>

using namespace eastl;
namespace BohgeEngine
{
	//---------------------------------------------------------------------------------------------
	RenderState::RenderState(void)
		:m_isDepthTest(false),
		m_isDepthMask(false),
		m_DepthFunction((MaterialProperty::Function)-1),
		m_isCullFace((MaterialProperty::CullFaceMode)-1),
		m_isAlphaEnable(false),
		m_isAlphaTest(false),
		m_Src((MaterialProperty::Blend)-1),
		m_Des((MaterialProperty::Blend)-1),
		m_SrcA((MaterialProperty::Blend)-1),
		m_DesA((MaterialProperty::Blend)-1),
		m_ColorMask_R(false),
		m_ColorMask_G(false),
		m_ColorMask_B(false),
		m_ColorMask_A(false),
		m_isStencil(false),
		m_StencilMask(0),
		m_StencilFunc((MaterialProperty::Function)-1),
		m_StencilFuncRef(0),
		m_StencilFuncMask(0),
		m_StencilFail((MaterialProperty::Operation)-1),
		m_StencilZFail((MaterialProperty::Operation)-1),
		m_StencilZPass((MaterialProperty::Operation)-1),
		m_uHashCode(0),
		m_RenderQueue(0)
	{
	}
	//---------------------------------------------------------------------------------------------
	RenderState::~RenderState(void)
	{
	}
	//---------------------------------------------------------------------------------------------
	void RenderState::CaculateHashCode()
	{
		eastl::vector<byte> bytes;
		bytes.reserve( 25 );
		bytes.push_back( m_RenderQueue );
		bytes.push_back( m_isDepthTest );
		bytes.push_back( m_isDepthMask );
		bytes.push_back( m_DepthFunction );
		bytes.push_back( m_isCullFace );

		bytes.push_back( m_isAlphaEnable );
		bytes.push_back( m_isAlphaTest );
		bytes.push_back( m_Src );
		bytes.push_back( m_Des );
		bytes.push_back( m_SrcA );
		bytes.push_back( m_DesA );

		bytes.push_back( m_ColorMask_R );
		bytes.push_back( m_ColorMask_G );
		bytes.push_back( m_ColorMask_B );
		bytes.push_back( m_ColorMask_A );

		bytes.push_back( m_isStencil );
		bytes.push_back( m_StencilMask );
		bytes.push_back( m_StencilFunc );
		bytes.push_back( m_StencilFuncRef );
		bytes.push_back( m_StencilFuncMask );
		bytes.push_back( m_StencilFail );
		bytes.push_back( m_StencilZFail );
		bytes.push_back( m_StencilZPass );

		m_uHashCode = Utility::HashCode( bytes );
	}
}