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
#include "MaterialProperty.h"
#include "PipelineProperty.h"
#include "ISharedResource.h"
#include "ShaderEntity.h"
#include "RenderState.h"



namespace BohgeEngine
{
	class ShaderEntity;
	class RenderState;
	class BOHGE_FRAMEWORK MaterialResource : public ISharedResource
	{
	private:
		static uint								s_StateHash;
		static bool								s_isDepthTest;
		static bool								s_isDepthMask;
		static MaterialProperty::Function		s_DepthFunction;
		static MaterialProperty::CullFaceMode	s_isCullFace;

		static bool								s_isAlphaEnable;
		static bool								s_isAlphaTest;
		static MaterialProperty::Blend			s_Src;
		static MaterialProperty::Blend			s_Des;
		static MaterialProperty::Blend			s_SrcA;
		static MaterialProperty::Blend			s_DesA;

		static bool								s_ColorMask_R;
		static bool								s_ColorMask_G;
		static bool								s_ColorMask_B;
		static bool								s_ColorMask_A;

		static bool								s_isStencil;//其他的stencil不记录
	public:
		BOHGE_FORCEINLINE static bool IsDepthMask()
		{
			return s_isDepthMask;
		}
	private:
		//const MaterialProperty::TextureSlotPropertyMap*	m_rTextureProperty;
		const RenderState*		m_rRenderState[PipelineProperty::PT_PASS_TYPE_COUNT];//按照全部pass去分配，按照实际使用的去new
		ShaderEntity*			m_ShaderEntity[PipelineProperty::PT_PASS_TYPE_COUNT];
	public:
		MaterialResource(void);
		virtual ~MaterialResource(void);
	private:
		virtual void _DoSetDepthTest( bool b ) = 0;
		virtual void _DoSetDepthMask( bool b ) = 0;
		virtual void _DoSetAlphaBlend( bool b ) = 0;
		virtual void _DoSetAlphaTest( bool b ) = 0;
		virtual void _DoSetDepthFunciton( MaterialProperty::Function func ) = 0;
		virtual void _DoSetCullFace( MaterialProperty::CullFaceMode cull ) = 0;
		virtual void _DoSetBlendFunction( 
			MaterialProperty::Blend src,
			MaterialProperty::Blend des,
			MaterialProperty::Blend srcA,
			MaterialProperty::Blend desA ) = 0;
		virtual void _DoSetColorMask( bool r, bool g, bool b, bool a ) = 0;
		virtual void _DoSetStencil( bool b ) = 0;
		virtual void _DoSetStencilMask(bitmask mask) = 0;
		virtual void _DoSetStencilFunction(MaterialProperty::Function func, uint ref, bitmask mask) = 0;
		virtual void _DoSetStencilOperation(
			MaterialProperty::Operation fail,
			MaterialProperty::Operation zfail,
			MaterialProperty::Operation zpass) = 0;
	public:
		virtual void ReleaseResource();
	public:
		void SetRenderState( int pt, const RenderState* st );
		void SetShaderEntity( int pt, ShaderEntity* se );
		void BindRenderState( int pt );
		//void SetTextureProperty( const MaterialProperty::TextureSlotPropertyMap* tm );
	public:
		BOHGE_FORCEINLINE ShaderEntity* GetShaderEntity( int pt )
		{
			return m_ShaderEntity[pt];
		}
		BOHGE_FORCEINLINE uint GetRenderQueue( int pt ) const
		{
			return m_rRenderState[pt]->GetRenderQueue();
		}
		BOHGE_FORCEINLINE bool isTransparent( int pt ) const
		{
			return m_rRenderState[pt]->isTransparent( pt );
		}
		BOHGE_FORCEINLINE uint GetMaterialID( int pt ) const
		{
			return m_ShaderEntity[pt]->GetHashCode() + m_rRenderState[pt]->GetHashCode();
		}
		//BOHGE_FORCEINLINE const MaterialProperty::TextureSlotPropertyMap& GetTextureSlotMap()
		//{
		//	return *m_rTextureProperty;
		//}
	};
}
