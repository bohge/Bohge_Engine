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



/////////////////////////////
//			材质		   //
/////////////////////////////
#pragma once
#include "Engine.h"
#include "Device.h"
#include "3DMath.h"
#include "ISceneNode.h"
#include "ShaderManager.h"
#include "Pipeline.h"
#include <string>
#include <map>


namespace BohgeEngine
{
	class Serializer;
	class Texture;
	class MaterialShader;
	class IAttributeNode;
	class TextureState;
	class Material
	{
	public:
		enum Material_Attribute	//材质包含的子项,模型输出的时候导出
		{
			ATT_AMBIENT = 2001,//环境光
			ATT_DIFFUSE,//满反射
			ATT_SPECULAR,//反射
			ATT_EMIT,
			ATT_OPCITY,//是否透明
			ATT_SPECULAR_LEVEL,//反射强度
			ATT_SHININESS,
			ATT_TEXTURES = 2501,//材质贴图开始输出
				ATT_TEX_UNKONW,//未知,缩进的意思是这个属于ATT_TEXTURES的字属性
				ATT_TEX_AMBIENT,//环境贴图
				ATT_TEX_DIFFUSE,//纹理属性
				ATT_TEX_SPECULAR,//反射颜色
				ATT_TEX_SPLEVEL,//发射等级
				ATT_TEX_GLOSSINESS,//发光
				ATT_TEX_ILLUMINATION,//自发光
				ATT_TEX_OPACITY,//透明贴图
				ATT_TEX_FITER,//过滤贴图
				ATT_TEX_BUMP,//法线贴图
				ATT_TEX_REFLECTION,//发射贴图
				ATT_TEX_REFRACTION,//折射贴图
				ATT_TEX_DISPLACEMENT,//视察贴图

				ATT_TEX_TERRAIN_WEIGHT,//地形的贴图
				ATT_TEX_TERRAIN_DETAL,
				ATT_TEX_TERRAIN_BASE,
				ATT_TEX_TERRAIN_R,
				ATT_TEX_TERRAIN_G,
				ATT_TEX_TERRAIN_B,

				ATT_TEX_USER0,//用户自定义的
				ATT_TEX_USER1,//用户自定义的
				ATT_TEX_USER2,//用户自定义的
				ATT_TEX_USER3,//用户自定义的
				ATT_TEX_USER4,//用户自定义的
				ATT_TEX_USER5,//用户自定义的
				ATT_TEX_USER6,//用户自定义的
				ATT_TEX_USER7,//用户自定义的

				//新增的必须放在后面
				ATT_TEX_LIGHT_MAP,//light map

			ATT_TEX_END,//最后一个

			ATTRIBUTE_END = 3000, //材质属性结束
		};
	private:
		struct InternalMaterial
		{
			Device::DrawState	m_DrawState;
			Device::AlphaState	m_AlphaState;
			int					m_ShaderIndexs;
			InternalMaterial():m_ShaderIndexs(-1){}
		};
	private:
		typedef std::map< int, TextureState* >		TextureStateMap;
		typedef std::map<Material_Attribute ,std::pair< bool, Texture2D*> > TextureSlots; //bool是指是否是由材质加载的，还是外部给予的
	private:
		IAttributeNode*		m_pNode;
		//MaterialShader*	m_pShader;
	private:
		std::string			m_Name;
	private:
		vector3f			m_Ambient;
		vector3f			m_Diffuse;
		vector3f			m_Specular;
		vector3f			m_Emit;
		float				m_Opacity;
		float				m_SpeLevel;
		float				m_Shininess;
		TextureSlots		m_TextureMap;
		TextureStateMap		m_DefaultTextureState;
		//绘制的材质
		InternalMaterial	m_MaterialState[Pipeline::PT_COUNT];
	public:
		Material();
		~Material();
		Material( const Material& rhs )
			:m_pNode(NULL)
		{
			_InitializationInternalMaterial();
			_Docopy( rhs );
		}
	private:
		void _InitializationInternalMaterial();
		void _Docopy( const Material& rhs );
		void _InsertTexture( Material_Attribute texAtt, bool ismake, Texture2D* tex );
	public:
		//生成一张贴图材质buffer，可以修改它
		Texture2D* LoadTexture(Material_Attribute texAtt, Device::PixelFormat pf, int pixelsize, const vector2d& size, const byte* buffer);
		Texture2D* LoadTexture(Material_Attribute texAtt, Device::PixelFormat pf, const std::string& path);
		Texture2D* LoadTexture(Material_Attribute texAtt, const std::string& path);
		Texture2D* LoadTexture(Material_Attribute texAtt, Texture2D& tex );
		const void* Attribute(Material_Attribute att) const;
		bool operator == ( const Material& rhs ) const;
		bool isSameTextureAndState( const Material& rhs ) const;//是否使用相同的纹理,和渲染state
		void SetDefaultTextureState( Material_Attribute texAtt, const TextureState& ts );
	public:
		BOHGE_FORCEINLINE void SetShader( const Pipeline::PassType& pt, int name )
		{
			m_MaterialState[pt].m_ShaderIndexs = name;
			//m_pShader = engine::Instance().GetShaderManage()->GetMaterialShader( name );
		}
		BOHGE_FORCEINLINE Material& operator = ( const Material& rhs )
		{
			_InitializationInternalMaterial();
			_Docopy( rhs );
			return *this;
		}
		BOHGE_FORCEINLINE void Using( const Pipeline::PassType& pt ) const//使用这个材质作为当前渲染材质
		{
			Engine& engine = Engine::Instance();
			engine.GetDevice()->SetDrawState( m_MaterialState[pt].m_DrawState );
			engine.GetDevice()->EnableAlphaBlend( m_MaterialState[pt].m_AlphaState );
			//ASSERT( NULL != m_pNode );
			ASSERT( -1 != m_MaterialState[pt].m_ShaderIndexs );
			MaterialShader* pShader = engine.GetShaderManager()->GetMaterialShader( m_MaterialState[pt].m_ShaderIndexs );
			pShader->SetParamNode(this);
			pShader->UsingShader();
			//m_pNode = NULL;
		}
		BOHGE_FORCEINLINE const void* Attribute(IAttributeNode::Node_Attribute att) const
		{
			return m_pNode->Attribute( att );
		}
		BOHGE_FORCEINLINE const vector3f& Ambient() const
		{
			return m_Ambient;
		}
		BOHGE_FORCEINLINE const vector3f& Diffuse() const
		{
			return m_Diffuse;
		}
		BOHGE_FORCEINLINE const vector3f& Specular() const
		{
			return m_Specular;
		}
		BOHGE_FORCEINLINE const vector3f& Emit() const
		{
			return m_Emit;
		}
		BOHGE_FORCEINLINE const float& Opacity() const
		{
			return m_Opacity;
		}
		BOHGE_FORCEINLINE const float& SpeLevel() const
		{
			return m_SpeLevel;
		}
		BOHGE_FORCEINLINE const float& Shininess() const
		{
			return m_Shininess;
		}
		BOHGE_FORCEINLINE vector3f& Ambient()
		{
			return m_Ambient;
		}
		BOHGE_FORCEINLINE vector3f& Diffuse()
		{
			return m_Diffuse;
		}
		BOHGE_FORCEINLINE vector3f& Specular()
		{
			return m_Specular;
		}
		BOHGE_FORCEINLINE vector3f& Emit()
		{
			return m_Emit;
		}
		BOHGE_FORCEINLINE float& Opacity()
		{
			return m_Opacity;
		}
		BOHGE_FORCEINLINE float& SpeLevel()
		{
			return m_SpeLevel;
		}
		BOHGE_FORCEINLINE float& Shininess()
		{
			return m_Shininess;
		}
		BOHGE_FORCEINLINE MaterialShader& GetShader( const Pipeline::PassType& pt ) const
		{
			ASSERT( -1 != m_MaterialState[pt].m_ShaderIndexs );
			return *Engine::Instance().GetShaderManager()->GetMaterialShader( m_MaterialState[pt].m_ShaderIndexs );
		}
		BOHGE_FORCEINLINE void SetNode( IAttributeNode* node )
		{
			m_pNode = node;
		}
		BOHGE_FORCEINLINE int GetTextureCount() const
		{
			return m_TextureMap.size();
		}
		BOHGE_FORCEINLINE int ShaderIndex( const Pipeline::PassType& pt ) const
		{
			return m_MaterialState[pt].m_ShaderIndexs;
		}
		BOHGE_FORCEINLINE Device::DrawState& GetDrawState( const Pipeline::PassType& pt )
		{
			return m_MaterialState[pt].m_DrawState;
		}
		BOHGE_FORCEINLINE Device::AlphaState& GetAlpahState( const Pipeline::PassType& pt )
		{ 
			return m_MaterialState[pt].m_AlphaState;
		}
		BOHGE_FORCEINLINE const std::string& GetName() const
		{
			return m_Name;
		}
		BOHGE_FORCEINLINE void SetName( const std::string& name )
		{
			m_Name = name;
		}
	};

}