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



#include "WaterTile.h"
#include "Material.h"
#include "Serializer.h"
#include "SceneDataIndex.h"
#include "Texture.h"
#include "Serializer.h"
#include "SceneDataIndex.h"
#include "IOSystem.h"


using namespace std;

namespace BohgeEngine
{
	static int g_AutoIndex = 0;


	//-------------------------------------------------------------------------------------------------------
	WaterTile::WaterTile(void)
		:m_Index(g_AutoIndex++)
	{
		m_WaterMaterial = NEW Material;
		m_WaterParameter = NEW parameterhandler;
	}
	//-------------------------------------------------------------------------------------------------------
	WaterTile::~WaterTile(void)
	{
		SAFE_DELETE( m_WaterMaterial );
		SAFE_DELETE( m_WaterParameter );
	}
	//-------------------------------------------------------------------------------------------------------
	void WaterTile::Update( uint milisecon )
	{

	}
	//-------------------------------------------------------------------------------------------------------
	void WaterTile::Render( Engine& engine )
	{

	}
	//-------------------------------------------------------------------------------------------------------
	void WaterTile::Load( const std::string& path, const std::string& TexPath )
	{
		Serializer loader( path + WaterData );
		bool isContinue = true;
		while( isContinue )
		{
			int index;
			loader>>index;
			switch( index )
			{
			case SceneDataIndex::Water_Materal:
				{
					for ( bool isDone = false; !isDone; )
					{
						int att = 0;
						loader>> att;
						switch( att )
						{
						case static_cast<int>(Material::ATT_AMBIENT) : loader>>m_WaterMaterial->Ambient().m_x >>m_WaterMaterial->Ambient().m_y >>m_WaterMaterial->Ambient().m_z; break;
						case static_cast<int>(Material::ATT_DIFFUSE) : loader>>m_WaterMaterial->Diffuse().m_x >>m_WaterMaterial->Diffuse().m_y >>m_WaterMaterial->Diffuse().m_z; break;
						case static_cast<int>(Material::ATT_SPECULAR) : loader>>m_WaterMaterial->Specular().m_x >>m_WaterMaterial->Specular().m_y >>m_WaterMaterial->Specular().m_z; break;
						case static_cast<int>(Material::ATT_EMIT) : loader>>m_WaterMaterial->Emit().m_x >>m_WaterMaterial->Emit().m_y >>m_WaterMaterial->Emit().m_z; break;
						case static_cast<int>(Material::ATT_OPCITY) : loader>>m_WaterMaterial->Opacity(); break;
						case static_cast<int>(Material::ATT_SPECULAR_LEVEL) : loader>>m_WaterMaterial->SpeLevel(); break;
						case static_cast<int>(Material::ATT_SHININESS) : loader>>m_WaterMaterial->Shininess(); break;
						case static_cast<int>(Material::ATT_TEXTURES) :
							{
								int texCount = 0;
								loader>> texCount;
								for ( int j = 0; j < texCount; j ++)
								{
									std::string name;
									int ID;
									loader>>ID>>name;
									if( IOINSTANCE.isExist(TexPath + WaterTextureFolder + name) )
									{
										Texture2D* temp = m_WaterMaterial->LoadTexture( static_cast<Material::Material_Attribute>(ID), TexPath + WaterTextureFolder + name );
										if ( Material::ATT_TEX_DIFFUSE == static_cast<Material::Material_Attribute>(ID) )
										{
											temp->SetWarp( Device::TW_CLAMP_TO_EDGE, Device::TW_CLAMP_TO_EDGE );
										}
										else
										{
											temp->SetWarp( Device::TW_REPEAT, Device::TW_REPEAT );
											if ( Material::ATT_TEX_BUMP == static_cast<Material::Material_Attribute>(ID) )
											{
												temp->SetAnisotropic( 2 );
											}
										}
									}
								}
							}break;
						case static_cast<int>(Material::ATTRIBUTE_END):
							{
								m_WaterMaterial->SetShader( Pipeline::PT_LIGHTING, ShaderManager::WaterShader );
								m_WaterMaterial->SetShader( Pipeline::PT_NORMAL, ShaderManager::WaterShaderNormal );
								m_WaterMaterial->GetAlpahState( Pipeline::PT_LIGHTING ).m_isAlphaEnable = false;
								m_WaterMaterial->GetAlpahState( Pipeline::PT_NORMAL ).m_isAlphaEnable = false;
								isDone = true; break;
							}
						}
					}
				}break;
			case SceneDataIndex::Water_Param:
				{
					loader>>m_WaterParameter->params[p_fStrength].fData;
					loader>>m_WaterParameter->params[p_fNoiseheight].fData;
					loader>>m_WaterParameter->params[p_bRefrStrength].fData;
					loader>>m_WaterParameter->params[p_fFalloff].fData;
					loader>>m_WaterParameter->params[p_fScale].fData;
					loader>>m_WaterParameter->params[p_iOctaves].iData;
					loader>>m_WaterParameter->params[p_fAnimspeed].fData;
					loader>>m_WaterParameter->params[p_fTimemulti].fData;
					loader>>m_WaterParameter->params[p_fElevation].fData;
					loader>>m_WaterParameter->params[p_fWaterBaseHeight].fData;
					loader>>m_WaterParameter->params[p_fCanSeeDepth].fData;
					loader>>m_WaterParameter->params[p_fNoiseTile].fData;
					loader>>m_WaterParameter->params[p_fColorChangeDepthInv].fData;
					loader>>m_WaterParameter->params[p_fNoiseStrength].fData;
					loader>>m_WaterParameter->params[p_fNoiseWaveSpeed].fData;
					loader>>m_WaterParameter->params[p_fBumpScale].fData;
					loader>>m_WaterParameter->params[p_fBumpFarPer].fData;
					loader>>m_WaterParameter->params[p_fBumpStrength].fData;
					loader>>m_WaterParameter->params[p_fReserved1].fData;
					loader>>m_WaterParameter->params[p_fReserved2].fData;
					loader>>m_WaterParameter->params[p_fReserved3].fData;
					loader>>m_WaterParameter->params[p_fReserved4].fData;
					loader>>m_WaterParameter->params[p_fReserved5].fData;
					loader>>m_WaterParameter->params[p_fReserved6].fData;
					loader>>m_WaterParameter->params[p_fReserved7].fData;
					loader>>m_WaterParameter->params[p_fReserved8].fData;
					loader>>m_WaterParameter->params[p_fReserved9].fData;
					loader>>m_WaterParameter->params[p_fReserved10].fData;
					loader>>m_WaterParameter->params[p_fReserved11].fData;
					loader>>m_WaterParameter->params[p_fReserved12].fData;
					loader>>m_WaterParameter->params[p_fReserved13].fData;
					loader>>m_WaterParameter->params[p_fReserved14].fData;
					loader>>m_WaterParameter->params[p_fReserved15].fData;
					loader>>m_WaterParameter->params[p_fReserved16].fData;
				}break;
			case SceneDataIndex::Water_Bind:
				{
					vector3f vmin, vmax;
					loader>>vmax>>vmin;
					m_BindBox = aabbox3df( vmin, vmax );

					m_WorldRange.Contains( vector2f(vmax.m_x, vmax.m_z) );
					m_WorldRange.Contains( vector2f(vmin.m_x, vmin.m_z) );
				}break;
			case SceneDataIndex::Save_Done: isContinue = false; break;
			}
		}
	}
}