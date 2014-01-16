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
#include "Material.h"
#include "Serializer.h"
#include "Texture.h"
#include "TextureState.h"



using namespace std;


namespace BohgeEngine
{
	//--------------------------------------------------------------------------------------------------------------------------------------
	Material::Material()
		:m_pNode(NULL),
		m_Opacity(0),
		m_SpeLevel(0),
		m_Shininess(0)
	{
		_InitializationInternalMaterial();
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	Material::~Material(void)
	{
		FOR_EACH( TextureSlots, it, m_TextureMap )
		{
			if( it->second.first )
			{
				SAFE_DELETE( it->second.second );
			}
		}
		FOR_EACH( TextureStateMap, it, m_DefaultTextureState )
		{
			SAFE_DELETE( it->second );
		}
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	void Material::_InsertTexture( Material_Attribute texAtt, bool ismake, Texture2D* tex )
	{
		TextureStateMap::iterator it = m_DefaultTextureState.find( texAtt );
		if ( m_DefaultTextureState.end() != it )
		{
			tex->SetWarp( it->second->SWarp(), it->second->TWarp() );
			tex->SetFilter( it->second->MagFilter(), it->second->MinFilter() );
		}
		m_TextureMap.insert( std::make_pair( texAtt, std::make_pair(ismake, tex)) );
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	Texture2D* Material::LoadTexture( Material_Attribute texAtt, Device::PixelFormat pf, const std::string& path )
	{
		TextureSlots::iterator it = m_TextureMap.find( texAtt );
		if ( it != m_TextureMap.end() ) //如果在材质中找到相应的数据
		{
			if( it->second.second->GetName() == path )//如果有，而且一样，就直接跳过，不读去了
			{
				return it->second.second;
			}
			if( it->second.first ) //如果是自己生成的
			{
				it->second.second->LoadTexture( pf, path );
				return it->second.second;
			}
			else
			{
				m_TextureMap.erase( it );//不是则删除他的引用，自己造一个
			}
		}
		Texture2D* tex = NEW Texture2D();
		tex->LoadTexture( pf, path );
		//m_TextureMap.insert( std::make_pair( texAtt, std::make_pair(true, tex)) );
		_InsertTexture( texAtt, true, tex );
		return tex;
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	Texture2D* Material::LoadTexture(Material_Attribute texAtt, const std::string& path)
	{
		Device::PixelFormat pf;
		switch( texAtt )
		{
		case ATT_TEX_AMBIENT: pf = Device::PF_R8G8B8; break;
		case ATT_TEX_DIFFUSE: pf = Device::PF_R8G8B8; break;
		case ATT_TEX_SPECULAR: pf = Device::PF_R8G8B8; break;
		case ATT_TEX_SPLEVEL: pf = Device::PF_L8; break;
		case ATT_TEX_GLOSSINESS: pf = Device::PF_L8; break;
		case ATT_TEX_ILLUMINATION: pf = Device::PF_R8G8B8; break;
		case ATT_TEX_OPACITY: pf = Device::PF_A8; break;
		case ATT_TEX_FITER: pf = Device::PF_R8G8B8; break;
		case ATT_TEX_BUMP: pf = Device::PF_R8G8B8; break;
		case ATT_TEX_REFLECTION: pf = Device::PF_R8G8B8; break;
		case ATT_TEX_REFRACTION: pf = Device::PF_R8G8B8; break;
		case ATT_TEX_DISPLACEMENT: pf = Device::PF_R8G8B8; break;
		case ATT_TEX_TERRAIN_WEIGHT: pf = Device::PF_R8G8B8A8; break;
		case ATT_TEX_TERRAIN_DETAL: pf = Device::PF_R8G8B8; break;
		case ATT_TEX_TERRAIN_BASE: pf = Device::PF_R8G8B8A8; break;
		case ATT_TEX_TERRAIN_R: pf = Device::PF_R8G8B8A8; break;
		case ATT_TEX_TERRAIN_G: pf = Device::PF_R8G8B8A8; break;
		case ATT_TEX_TERRAIN_B: pf = Device::PF_R8G8B8A8; break;
		case ATT_TEX_LIGHT_MAP: pf = Device::PF_R8G8B8; break;
		default: ASSERT( false );break;
		}
		return LoadTexture(texAtt, pf, path);
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	Texture2D* Material::LoadTexture( Material_Attribute texAtt, Device::PixelFormat pf, int pixelsize, const vector2d& size, const byte* buffer )
	{
		TextureSlots::iterator it = m_TextureMap.find( texAtt );
		if ( it != m_TextureMap.end() ) //如果在材质中找到相应的数据
		{
			if( it->second.first ) //如果是自己生成的
			{
				it->second.second->MakeTexture( pf, pixelsize, size, buffer );
				return it->second.second;
			}
			else
			{
				m_TextureMap.erase( it );//不是则删除他的引用，自己造一个
			}
		}
		Texture2D* tex = NEW Texture2D();
		tex->MakeTexture( pf, pixelsize, size, buffer );
		//m_TextureMap.insert( std::make_pair( texAtt, std::make_pair(true, tex)) );
		_InsertTexture( texAtt, true, tex );
		return tex;
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	Texture2D* Material::LoadTexture( Material_Attribute texAtt, Texture2D& tex )
	{
		TextureSlots::iterator it = m_TextureMap.find( texAtt );
		if ( it != m_TextureMap.end() ) //如果在材质中找到相应的数据
		{
			if( it->second.first ) //如果是自己生成的
			{
				SAFE_DELETE( it->second.second );
			}
		}
		//m_TextureMap.insert( std::make_pair( texAtt, std::make_pair(false, &tex)) );
		_InsertTexture( texAtt, false, &tex );
		return &tex;
	}

	//--------------------------------------------------------------------------------------------------------------------------------------
	const void* Material::Attribute(Material::Material_Attribute att) const
	{
		TextureSlots::const_iterator it = m_TextureMap.find( att );
		if ( it != m_TextureMap.end() ) //如果在材质中找到相应的数据
		{
			return it->second.second;
		}
		return NULL;
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	void Material::_Docopy( const Material& rhs )
	{
		m_pNode;//这个不能变
		for ( int i = 0; i < Pipeline::PT_COUNT; i ++ )
		{
			m_MaterialState[i].m_DrawState = rhs.m_MaterialState[i].m_DrawState;
			m_MaterialState[i].m_AlphaState = rhs.m_MaterialState[i].m_AlphaState;
			m_MaterialState[i].m_ShaderIndexs = rhs.m_MaterialState[i].m_ShaderIndexs;
		}
		m_Ambient = rhs.m_Ambient;
		m_Diffuse = rhs.m_Diffuse;
		m_Specular = rhs.m_Specular;
		m_Emit = rhs.m_Emit;
		m_Opacity = rhs.m_Opacity;
		m_SpeLevel = rhs.m_SpeLevel;
		m_Shininess = rhs.m_Shininess;

		FOR_EACH_CONST( TextureSlots, it, rhs.m_TextureMap )
		{
			if( it->second.second->isLoadFormFile() )
			{
				this->LoadTexture( it->first, it->second.second->GetPixelFormat(), it->second.second->GetName() );
			}
		}
		FOR_EACH_CONST( TextureStateMap, it, rhs.m_DefaultTextureState )
		{
			m_DefaultTextureState.insert( make_pair( it->first, NEW TextureState( *it->second ) ) );
		}
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	bool Material::operator==( const Material& rhs ) const
	{
		//if( m_pShader == rhs.m_pShader )//shader 是地址，可以直接比较
		if( 0 == memcmp( m_MaterialState, rhs.m_MaterialState, sizeof(m_MaterialState) ) )
		{
			if( Math::isEqual( m_Opacity, rhs.m_Opacity ) )
			{
				if ( Math::isEqual( m_SpeLevel, rhs.m_SpeLevel ) )
				{
					if ( Math::isEqual( m_Shininess, rhs.m_Shininess ) )
					{
						if ( m_Ambient == rhs.m_Ambient )
						{
							if ( m_Diffuse == rhs.m_Diffuse )
							{
								if ( m_Specular == rhs.m_Specular )
								{
									if ( m_Emit == rhs.m_Emit )
									{
										if( m_TextureMap.size() == rhs.m_TextureMap.size() )
										{
											for ( TextureSlots::const_iterator it = m_TextureMap.begin();
												it != m_TextureMap.end();
												it ++ )
											{
												TextureSlots::const_iterator finded = rhs.m_TextureMap.find( it->first );
												if( finded == rhs.m_TextureMap.end() )
												{
													return false;
												}
												else
												{
													if( finded->second.second->GetName() != it->second.second->GetName() )
													{
														return false;
													}
												}
											}
											return true;
										}
									}
								}
							}
						}
					}
				}
			}
		}
		return false;
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	bool Material::isSameTextureAndState( const Material& rhs ) const
	{
		if( m_TextureMap.size() == rhs.m_TextureMap.size() )
		{
			for ( int i = 0; i < Pipeline::PT_COUNT; i ++ )
			{
				if ( 
					0 != memcmp( &m_MaterialState[i].m_DrawState, &rhs.m_MaterialState[i].m_DrawState, sizeof( m_MaterialState[i].m_DrawState ) )
					|| 0 != memcmp( &m_MaterialState[i].m_AlphaState, &rhs.m_MaterialState[i].m_AlphaState, sizeof( m_MaterialState[i].m_AlphaState ) )
					)
				{
					return false;
				}
			}
			for ( TextureSlots::const_iterator it = m_TextureMap.begin();
				it != m_TextureMap.end();
				it ++ )
			{
				TextureSlots::const_iterator finded = rhs.m_TextureMap.find( it->first );
				if( finded == rhs.m_TextureMap.end() )
				{
					return false;
				}
				else
				{
					if( finded->second.second->GetName() != it->second.second->GetName() )
					{
						return false;
					}
				}
			}
			return true;
		}
		return false;
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	void Material::_InitializationInternalMaterial()
	{
		m_MaterialState[Pipeline::PT_LIGHTING].m_AlphaState.m_isAlphaEnable = false;
		m_MaterialState[Pipeline::PT_NORMAL].m_AlphaState.m_isAlphaEnable = false;
		m_MaterialState[Pipeline::PT_DEPTH].m_AlphaState.m_isAlphaEnable = false;
		ASSERT( Pipeline::PT_COUNT == 3 );
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	void Material::SetDefaultTextureState( Material_Attribute texAtt, const TextureState& ts )
	{
		m_DefaultTextureState.insert( make_pair( texAtt, NEW TextureState( ts ) ) );
	}
}