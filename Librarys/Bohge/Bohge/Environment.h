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
//		环境	   //
/////////////////////

#pragma once
#include "3DMath.h"
#include <vector>


namespace BohgeEngine
{
	class Engine;
	class ICamera;
	class Texture;
	class Material;
	class Texture2D;
    class LightNode;
	class RendBuffer;
	class IPostEffect;
	class SharedBuffer;
	class RenderTarget;
	class RealLightNode;
	class VertexBuffer;
	class IndicesBuffer;
	class Environment //主环境，可产生阴影
	{
		friend class LightNode;
	private:
		enum
		{
			MAX_LIGHTHALO_SIZE	= 20, //初始分配20个灯光的halo
		};
	private:
		typedef std::vector< LightNode* >	LightsArray;
	private:
		int					m_CurrentLightIndex;
		RealLightNode*		m_MainLight;//主灯光
		LightsArray			m_LightsArray; //灯光表，提供真实多光源使用
		vector3f			m_AmbientLight;	//环境光强度
		//阴影深度纹理
		bool				m_isCastShadow;
		//阳光的特效
		bool				m_isEnableVL;
		IPostEffect*		m_VolumeLight;
		vector3f			m_VolumeLightColor;//光线光的颜色
		//雾
		vector3f			m_FogColor;
		vector3f			m_FogOgiParam;
		vector3f			m_FogParam;
		//光环
		Texture2D*			m_pTexture;
		RendBuffer*			m_pRendBuffer;
		VertexBuffer*		m_pVertexBuffer;
		IndicesBuffer*		m_pIndicesBuffer;
		SharedBuffer*		m_pSharedBuffer;
	public:
		explicit Environment( const vector3f& pos, const vector3f& dir, const vector3f& Ambient, const vector3f& color, const vector3f& vlcolor, const vector3f& fc, const vector3f& fp  );
		~Environment();
		void Update( ICamera& maincamera, uint milisecond );
		void Render( Engine& engine );
		void CastDepth( Engine& engine );
		void RecaculateFogParam();
		void SetEnvironment( const vector3f& pos, const vector3f& dir, const vector3f& Ambient, const vector3f& color, const vector3f& vlcolor, const vector3f& fc, const vector3f& fp  );
	private:
		BOHGE_FORCEINLINE void _AddLightNode( LightNode* node ) //为灯光准备的
		{
			m_LightsArray.push_back( node );
		}
		BOHGE_FORCEINLINE void _RemoveLightNode( LightNode* node )
		{
			for ( LightsArray::iterator it = m_LightsArray.begin();
				it != m_LightsArray.end(); 
				it ++ )
			{
				if ( *it == node )
				{
					m_LightsArray.erase( it );
					break;
				}
			}
		}
	public:
		//灯光相关
		BOHGE_FORCEINLINE bool hasLight()
		{
			return !m_LightsArray.empty();
		}
		BOHGE_FORCEINLINE uint GetLightCount()
		{
			return m_LightsArray.size();
		}
		BOHGE_FORCEINLINE void SetCurrentLightIndex( int index )
		{
			m_CurrentLightIndex = index;
		}
		BOHGE_FORCEINLINE LightNode& GetLightByIndex( int index )
		{
			return *m_LightsArray[ index ];
		}
		BOHGE_FORCEINLINE LightNode& GetCurrentLight() //得到当前渲染需要的灯光
		{
			return *m_LightsArray[ m_CurrentLightIndex ];
		}
		//散射效果
		BOHGE_FORCEINLINE void SetVolumeLightActivity( bool b )
		{
			m_isEnableVL = b;
		}
		BOHGE_FORCEINLINE vector3f& GetFogColor()
		{
			return m_FogColor;
		}
		BOHGE_FORCEINLINE vector3f& GetFogOgiParam()
		{
			return m_FogOgiParam;
		}
		BOHGE_FORCEINLINE vector3f& GetFogParam()
		{
			return m_FogParam;
		}
		BOHGE_FORCEINLINE vector3f& GetVolumeLightColor()
		{
			return m_VolumeLightColor;
		}
		BOHGE_FORCEINLINE vector3f& GetAmbient()
		{
			return m_AmbientLight;
		}

		BOHGE_FORCEINLINE bool isCastShadow()
		{
			return m_isCastShadow;
		}
	};
}