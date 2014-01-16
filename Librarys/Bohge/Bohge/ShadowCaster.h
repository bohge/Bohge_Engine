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



/////////////////////
//		阴影	   //
/////////////////////

#pragma once
#include "Predefine.h"

#include <map>
#include <vector>


namespace BohgeEngine
{
	class Engine;
	class ICamera;
	class Texture;
	class LightNode;
	class RendBuffer;
	class ISceneNode;
	class RenderTarget;
	class VertexBuffer;
	class IndicesBuffer;
	class RenderTargetCube;
	class ShadowCaster
	{
	private:
		typedef std::multimap<float, ISceneNode* > NodeMultimap;//需要绘制阴影的
	public:
		struct CameraItem 
		{
			bool				m_isDirty;
			NodeMultimap		m_CastShadowNode;
			ICamera*			m_pCamera;
			explicit CameraItem( ICamera* came )
				:m_isDirty(true),
				m_pCamera( came )
			{

			}
		};
	private:
		typedef std::vector< CameraItem >		CameraItemArray;
		typedef std::vector< RenderTarget* >	RenderTargetArray;//第一个参数判断是否是不干净的，需要clear
		typedef std::vector< Texture* >			TextureArray;
	protected:
		enum CasterType
		{
			CT_SHADOW_BASE = 1,
			CT_SHADOW_CUBE = 6,
			CT_SHADOW_PSSM = 4,
		};
		enum Constant
		{
			SC_SHADOWMAPSIZE = 512,//深度图的尺寸
			SC_SHADOWRANGE = 160,//深度的有效范围
			SC_SHADOWMOVE = 100,//偏移
		};
	private:
		CasterType		m_eCasterType;
	protected:
		bool					m_isCasterShadow;
		RendBuffer*				m_pRendBuffer;
		VertexBuffer*			m_pVertexBuffer;
		IndicesBuffer*			m_pIndicesBuffer;
		RenderTargetArray		m_TargetPtrArray;
		CameraItemArray			m_CameraItemArray;
		RenderTarget*			m_pGaussHelper;
		TextureArray			m_TextureArray;
	public:
		explicit ShadowCaster( CasterType ct );
		virtual ~ShadowCaster(void);
	private:
		virtual RenderTarget* _GetRendTarget( int index );
	public:
		virtual void Initialization( const LightNode& light ); //根节点将深成的rt纹理押送到纹理数组中
		virtual void Update( const LightNode& light, ICamera& maincamera, bool islightchange );
	public:
		virtual void OnLightChange( const LightNode& light ) = 0;
	public:
		void CastDepth( Engine& engine );//在纹理上渲染深度
	public:
		BOHGE_FORCEINLINE CasterType GetCasterType()
		{
			return m_eCasterType;
		}
		BOHGE_FORCEINLINE const TextureArray& GetDepthTextureArray() const
		{
			return m_TextureArray;
		}
		BOHGE_FORCEINLINE const ICamera& GetCamera( int index ) const
		{
			return *(m_CameraItemArray[index].m_pCamera);
		}
		BOHGE_FORCEINLINE const Texture& GetDepthTexture( int index ) const
		{
			return *m_TextureArray[index];
		}
		BOHGE_FORCEINLINE const ICamera& GetFirstCamera( ) const
		{
			return *(m_CameraItemArray[0].m_pCamera);
		}
		BOHGE_FORCEINLINE  Texture& GetFirstDepthTexture( )
		{
			return *m_TextureArray[0];
		}
		BOHGE_FORCEINLINE bool isCastShadow() const
		{
			return m_isCasterShadow;
		}
	};



	//方向光的caster
	class DirectLightCaster : public ShadowCaster
	{
	public:
		explicit DirectLightCaster( );
		~DirectLightCaster();
	public:
		virtual void Initialization( const LightNode& light );
		virtual void Update( const LightNode& light, ICamera& maincamera, bool islightchange );
		virtual void OnLightChange( const LightNode& light ){};
	};



	//点光的caster
	class PointLightCaster : public ShadowCaster
	{
		RenderTargetCube*	m_RenderTargetCube;
	public:
		explicit PointLightCaster( );
		~PointLightCaster();
	private:
		virtual RenderTarget* _GetRendTarget( int index );
	public:
		virtual void Initialization( const LightNode& light );
		virtual void OnLightChange( const LightNode& light );
	};



	//spot光的caster
	class SpotLightCaster : public ShadowCaster
	{
	public:
		explicit SpotLightCaster( );
		~SpotLightCaster();
	public:
		virtual void Initialization( const LightNode& light );
		virtual void OnLightChange( const LightNode& light );
	};
}