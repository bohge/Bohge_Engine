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
#include "MaterialResource.h"




namespace BohgeEngine
{
	//---------------------------------------------------------------------------------------------
	bool MaterialResource::s_isAlphaTest = false;
	bool MaterialResource::s_isAlphaEnable = false;
	bool MaterialResource::s_isDepthMask = false;
	bool MaterialResource::s_isDepthTest = false;
	MaterialProperty::Blend MaterialResource::s_DesA = (MaterialProperty::Blend)-1;
	MaterialProperty::Blend MaterialResource::s_SrcA = (MaterialProperty::Blend)-1;
	MaterialProperty::Blend MaterialResource::s_Des = (MaterialProperty::Blend)-1;
	MaterialProperty::Blend MaterialResource::s_Src = (MaterialProperty::Blend)-1;
	MaterialProperty::CullFaceMode MaterialResource::s_isCullFace = (MaterialProperty::CullFaceMode)-1;
	MaterialProperty::Function MaterialResource::s_DepthFunction = (MaterialProperty::Function)-1;
	bool MaterialResource::s_ColorMask_A = false;
	bool MaterialResource::s_ColorMask_B = false;
	bool MaterialResource::s_ColorMask_G = false;
	bool MaterialResource::s_ColorMask_R = false;
	bool MaterialResource::s_isStencil = false;
	uint MaterialResource::s_StateHash = 0;
	//---------------------------------------------------------------------------------------------
	MaterialResource::MaterialResource(void)
	{
		memset( m_rRenderState, NULL, sizeof(m_rRenderState) );
		memset( m_ShaderEntity, NULL, sizeof(m_ShaderEntity) );
	}
	//---------------------------------------------------------------------------------------------
	MaterialResource::~MaterialResource(void)
	{
	}
	//---------------------------------------------------------------------------------------------
	void MaterialResource::ReleaseResource()
	{

	}
	//---------------------------------------------------------------------------------------------
	void MaterialResource::SetRenderState( int pt, const RenderState* st )
	{
		m_rRenderState[pt] = st;
	}
	//---------------------------------------------------------------------------------------------
	void MaterialResource::SetShaderEntity( int pt, ShaderEntity* se )
	{
		m_ShaderEntity[pt] = se;
	}
	//---------------------------------------------------------------------------------------------
	void MaterialResource::BindRenderState( int pt )
	{
		if ( s_StateHash != m_rRenderState[pt]->GetHashCode() )
		{
			s_StateHash = m_rRenderState[pt]->GetHashCode();
			//draw
			if ( s_isDepthTest != m_rRenderState[pt]->GetDepthTest() )
			{
				s_isDepthTest = m_rRenderState[pt]->GetDepthTest();
				_DoSetDepthTest(s_isDepthTest);
				if ( s_isDepthTest 
					&& ( s_DepthFunction != m_rRenderState[pt]->GetDepthFunction() ) )
				{
					s_DepthFunction = m_rRenderState[pt]->GetDepthFunction();
					_DoSetDepthFunciton( s_DepthFunction );
				}
			}
			if ( s_isDepthMask != m_rRenderState[pt]->GetDepthMask() )
			{
				s_isDepthMask = m_rRenderState[pt]->GetDepthMask();
				_DoSetDepthMask(s_isDepthMask);
			}
			if ( s_isCullFace != m_rRenderState[pt]->GetCullFace() )
			{
				s_isCullFace = m_rRenderState[pt]->GetCullFace();
				_DoSetCullFace(s_isCullFace);
			}

			//alpha
			if ( s_isAlphaTest != m_rRenderState[pt]->GetAlphaTest() )
			{
				s_isAlphaTest = m_rRenderState[pt]->GetAlphaTest();
				_DoSetAlphaTest( s_isAlphaTest );
			}
			if ( s_isAlphaEnable != m_rRenderState[pt]->GetAlphaEnable() )
			{
				s_isAlphaEnable = m_rRenderState[pt]->GetAlphaEnable();
				_DoSetAlphaBlend( s_isAlphaEnable );
				if ( s_isAlphaEnable 
					&&
					( s_Src != m_rRenderState[pt]->GetBlendSrc()
					|| s_Des != m_rRenderState[pt]->GetBlendDes()
					|| s_SrcA != m_rRenderState[pt]->GetBlendSrcAlpha()
					|| s_DesA != m_rRenderState[pt]->GetBlendDesAlpha() ) )
				{
					s_Src = m_rRenderState[pt]->GetBlendSrc();
					s_Des = m_rRenderState[pt]->GetBlendDes();
					s_SrcA = m_rRenderState[pt]->GetBlendSrcAlpha();
					s_DesA = m_rRenderState[pt]->GetBlendDesAlpha();
					_DoSetBlendFunction( s_Src, s_Des, s_SrcA, s_DesA );
				}
			}

			//color
			if ( s_ColorMask_R != m_rRenderState[pt]->GetColorR()
				|| s_ColorMask_G != m_rRenderState[pt]->GetColorG()
				|| s_ColorMask_B != m_rRenderState[pt]->GetColorB()
				|| s_ColorMask_A != m_rRenderState[pt]->GetColorA() )
			{
				s_ColorMask_R = m_rRenderState[pt]->GetColorR();
				s_ColorMask_G = m_rRenderState[pt]->GetColorG();
				s_ColorMask_B = m_rRenderState[pt]->GetColorB();
				s_ColorMask_A = m_rRenderState[pt]->GetColorA();
				_DoSetColorMask( s_ColorMask_R, s_ColorMask_G, s_ColorMask_B, s_ColorMask_A );
			}

			//stencil
			if ( s_isStencil != m_rRenderState[pt]->GetStencilEnable() )
			{
				s_isStencil = m_rRenderState[pt]->GetStencilEnable();
				_DoSetStencil( s_isStencil );
				if ( s_isStencil )
				{
					_DoSetStencilMask( m_rRenderState[pt]->GetStencilMask() );
					_DoSetStencilFunction( m_rRenderState[pt]->GetStencilFunc(), m_rRenderState[pt]->GetStencilFuncRef(), m_rRenderState[pt]->GetStencilFuncMask() );
					_DoSetStencilOperation(
						m_rRenderState[pt]->GetStencilFail(),
						m_rRenderState[pt]->GetStencilZFail(),
						m_rRenderState[pt]->GetStencilZPass()
						);
				}
			}
		}
	}
	//---------------------------------------------------------------------------------------------
	//void MaterialResource::SetTextureProperty( const MaterialProperty::TextureSlotPropertyMap* tm )
	//{
	//	m_rTextureProperty = tm;
	//}

}