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
#include "Framework/ReferenceVertexMetadata.h"
#include "Framework/SquareIndicesMetadata.h"
#include "Framework/RenderObjectMetadata.h"
#include "Framework/OrientationComponent.h"
#include "Framework/PackagedVertexStream.h"
#include "Framework/TransformComponent.h"
#include "Framework/RenderComponent.h"
#include "Framework/TextureMetadata.h"
#include "Framework/ITimeSystem.h"
#include "PositionNoiseAffector.h"
#include "Framework/JsonReader.h"
#include "SpeedAxisAffector.h"
#include "PositionAffector.h"
#include "GeneralAffector.h"
#include "GeneralEmitter.h"
#include "SpeedAffector.h"
#include "DataLoader_1.h"
#include "BornAffector.h"



#include <EASTL/map.h>

using namespace eastl;
using namespace BohgeEngine;
namespace ParticleSystemPlugin
{
	RTTI_TYPE_IMPLEMENT( GeneralEmitter );
	//---------------------------------------------------------------------------------------------------------
	GeneralEmitter::GeneralEmitter(void)
		:m_pOrientation( NEW OrientationComponent ),
		m_pTransform( NEW TransformComponent ),
		m_pRender( NEW RenderComponent ),
		m_pIndicesStream( NEW IndicesStream ),
		m_pVertexStream( NEW VertexStream ),
		m_LimitedIndex(1),
		m_TimeSpan(0.0f),
		m_pDensityVolume(NULL)
	{
		AttachComponent( m_pRender );
		AttachComponent( m_pTransform );
		AttachComponent( m_pOrientation );
	}
	//---------------------------------------------------------------------------------------------------------
	GeneralEmitter::~GeneralEmitter(void)
	{
		ReleaseComponent();
		SAFE_DELETE( m_pRender );
		SAFE_DELETE( m_pTransform );
		SAFE_DELETE( m_pOrientation );
		SAFE_DELETE( m_pIndicesStream );
		SAFE_DELETE( m_pDensityVolume );
		SAFE_DELETE( m_pVertexStream );
		FOR_EACH( IAffectorList, aff, m_AffectorList )
		{
			SAFE_DELETE( *aff );
		}
		FOR_EACH( AffectorVolumePairList, avp, m_AffectorVolumePairList )
		{
			SAFE_DELETE( avp->second );
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void GeneralEmitter::_Load_1( BohgeEngine::JsonReader& reader )
	{
		EmitterLoader_1 data;
		reader.Read( data );
		m_pTransform->SetLocalScale( data.m_Transform.m_Scale.m_Vector3f );
		m_pTransform->SetLocalPosition( data.m_Transform.m_Position.m_Vector3f );
		m_pTransform->SetLocalRotation( data.m_Transform.m_Rotation.m_Quaternionf );

		for ( int i = 0 ; i < data.m_AffectorArray.Getsize(); ++ i )
		{
			Affector_1& aff = data.m_AffectorArray.At<Affector_1>( i );
			switch( aff.m_DataType )
			{
			case Math::DST_VECTOR1: 
				_SetAffectParameter( 
					static_cast<EmitterProperty::EmitterAffector>(aff.m_Affect),
					Math::DST_VECTOR1,
					vector1f( static_cast< JsonFloat* >( aff.m_pMin )->m_Data ),
					vector1f( static_cast< JsonFloat* >( aff.m_pMax )->m_Data ) );
				break;
			case Math::DST_VECTOR3:
				_SetAffectParameter( 
					static_cast<EmitterProperty::EmitterAffector>(aff.m_Affect),
					Math::DST_VECTOR3,
					static_cast< JsonVector3f* >( aff.m_pMin )->m_Vector3f,
					static_cast< JsonVector3f* >( aff.m_pMax )->m_Vector3f );
				break;
			default:ASSERT(false);
			}
		}

		//材质
		m_pRender->PushMetadata(
			RenderObjectMaterialMetadata(
				PathMetadata( data.m_Material.m_Path ),
				data.m_Material.m_Ambient.m_Vector3f,
				data.m_Material.m_Diffuse.m_Vector3f,
				data.m_Material.m_Specular.m_Vector3f,
				data.m_Material.m_Emit.m_Vector3f,
				data.m_Material.m_Opacity_SpeLevel_Shininess.m_Vector3f.m_x,
				data.m_Material.m_Opacity_SpeLevel_Shininess.m_Vector3f.m_y,
				data.m_Material.m_Opacity_SpeLevel_Shininess.m_Vector3f.m_z) );

		//纹理
		for ( int i = 0 ; i < data.m_Material.m_Textures.Getsize(); ++ i )
		{
			const JsonMaterialTextureLoader_1& mt = data.m_Material.m_Textures.At<JsonMaterialTextureLoader_1>( i );
			m_pRender->PushMetadata(
				RenderObjectTextureMetadata(
					MaterialProperty::Material_Attribute(MaterialProperty::ATT_TEXTURE_AMBIENT + mt.m_Slot),
					TextureFileMetadata(
						TextureProperty::TU_STATIC,
						static_cast<TextureProperty::PixelFormat>(mt.m_Texture.m_Format),
						mt.m_Texture.m_Anisotropic,
						mt.m_Texture.m_isMipmap,
						static_cast<TextureProperty::TextureWarp>(mt.m_Texture.m_SWarp),
						static_cast<TextureProperty::TextureWarp>(mt.m_Texture.m_TWarp),
						static_cast<TextureProperty::TextureFilter>(mt.m_Texture.m_MagFilter),
						static_cast<TextureProperty::TextureFilter>(mt.m_Texture.m_MinFilter),
						mt.m_Texture.m_Path) ));
		}

		//初始化影响器 
		_ParseAffector();
		//初始化buffer
		_InitializeStream();
	}
	//---------------------------------------------------------------------------------------------------------
	void GeneralEmitter::_CaculateMaxParticleCount( Math::DataStructType dst, const Object& max )
	{
		switch( dst )
		{
		case Math::DST_VECTOR1: m_LimitedIndex.SetMax( m_LimitedIndex.Max() * static_cast<const vector1f&>( max ).m_x ); break;
		default:ASSERT(false);
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void GeneralEmitter::_SetAffectParameter( EmitterProperty::EmitterAffector aff, Math::DataStructType dst, const Volume& min, const Volume& max )
	{
		ASSERT( min.GetTypeID() == max.GetTypeID() );
		RandVolume* data = NEW RandVolume;
		data->SetRange( dst, min, max );
		switch( aff )
		{
		case EmitterProperty::EA_PARTICLE_DENSITY:
			{
				m_pDensityVolume = data;
				_CaculateMaxParticleCount( dst, max );
			}break;
		case EmitterProperty::EA_PARTICLE_LIFE:
			{
				_CaculateMaxParticleCount( dst, max );
				m_AffectorVolumePairList.push_back( make_pair( aff, data ) );
			}break;
		default:
			{
				m_AffectorVolumePairList.push_back( make_pair( aff, data ) ); 
			}
		}
	}
	//---------------------------------------------------------------------------------------------------------
	Attributes::ShaderAttributes GeneralEmitter::_AttributesMapping( EmitterProperty::EmitterAffector aff )
	{
		switch( aff )
		{
		case EmitterProperty::EA_PARTICLE_LIFE:				return Attributes::ATTRIBUTE_EMITTER_LIFE;
		case EmitterProperty::EA_PARTICLE_SPEED:			return Attributes::ATTRIBUTE_EMITTER_SPEED;
		case EmitterProperty::EA_PARTICLE_ACCELERATION:		return Attributes::ATTRIBUTE_EMITTER_ACCELERATION;
		case EmitterProperty::EA_PARTICLE_SIZE:				return Attributes::ATTRIBUTE_EMITTER_SIZE;
		case EmitterProperty::EA_PARTICLE_SIZE_CURVATURE:	return Attributes::ATTRIBUTE_EMITTER_SIZE_CURVATURE;
		case EmitterProperty::EA_PARTICLE_COLOR:			return Attributes::ATTRIBUTE_EMITTER_COLOR;
		case EmitterProperty::EA_PARTICLE_COLOR_CURVATURE:	return Attributes::ATTRIBUTE_EMITTER_COLOR_CURVATURE;
		case EmitterProperty::EA_PARTICLE_SPIN:				return Attributes::ATTRIBUTE_EMITTER_SPIN;
		case EmitterProperty::EA_PARTICLE_POSITION_NOISE:	return Attributes::ATTRIBUTE_POSITION;
		case EmitterProperty::EA_PARTICLE_USER_0:			return Attributes::ATTRIBUTE_USER_0;
		case EmitterProperty::EA_PARTICLE_USER_1:			return Attributes::ATTRIBUTE_USER_1;
		case EmitterProperty::EA_PARTICLE_USER_2:			return Attributes::ATTRIBUTE_USER_2;
		case EmitterProperty::EA_PARTICLE_USER_3:			return Attributes::ATTRIBUTE_USER_3;
		case EmitterProperty::EA_PARTICLE_USER_4:			return Attributes::ATTRIBUTE_USER_4;
		case EmitterProperty::EA_PARTICLE_USER_5:			return Attributes::ATTRIBUTE_USER_5;
		case EmitterProperty::EA_PARTICLE_USER_6:			return Attributes::ATTRIBUTE_USER_6;
		case EmitterProperty::EA_PARTICLE_USER_7:			return Attributes::ATTRIBUTE_USER_7;
		default:ASSERT(false);
		}
		return Attributes::ATTRIBUTE_USER_0;
	}
	//---------------------------------------------------------------------------------------------------------
	void GeneralEmitter::_SettingVertexStream( EmitterProperty::EmitterAffector aff, BohgeEngine::Math::DataStructType dst )
	{
		switch( dst )
		{
		case Math::DST_VECTOR1:
			m_pVertexStream->SetVertexType<vector1f>(
				_AttributesMapping(aff),
				BufferObjectProperty::DT_FLOAT,
				BufferObjectProperty::DT_HALF_FLOAT );
			break;
		case Math::DST_VECTOR2:
			m_pVertexStream->SetVertexType<vector2f>(
				_AttributesMapping(aff),
				BufferObjectProperty::DT_FLOAT,
				BufferObjectProperty::DT_HALF_FLOAT );
			break;
		case Math::DST_VECTOR3:
			m_pVertexStream->SetVertexType<vector3f>(
				_AttributesMapping(aff),
				BufferObjectProperty::DT_FLOAT,
				BufferObjectProperty::DT_HALF_FLOAT );
			break;
		case Math::DST_VECTOR4:
			m_pVertexStream->SetVertexType<vector4f>(
				_AttributesMapping(aff),
				BufferObjectProperty::DT_FLOAT,
				BufferObjectProperty::DT_HALF_FLOAT );
			break;
		default:ASSERT(false);
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void GeneralEmitter::_ParseAffector()
	{
		typedef	map< EmitterProperty::EmitterAffector, RandVolume* > AffectorVolumeMap;
		AffectorVolumeMap aVM;
		FOR_EACH( AffectorVolumePairList, pair, m_AffectorVolumePairList )
		{
			aVM.insert( make_pair( pair->first, pair->second ) );
		}

		//处理位置
		{
			AffectorVolumeMap::iterator pos = aVM.find( EmitterProperty::EA_PARTICLE_POSITION_NOISE );
			_SettingVertexStream( EmitterProperty::EA_PARTICLE_POSITION_NOISE, Math::DST_VECTOR3 );//都需要添加该顶点流
			IAffector* posAff = NULL;
			if ( aVM.end() == pos )
			{
				PositionAffector* aff = NEW PositionAffector;
				aff->SetTransformComponent( m_pTransform );
				posAff = aff;
			}
			else
			{
				PositionNoiseAffector* aff = NEW PositionNoiseAffector;
				aff->SetTransformComponent( m_pTransform );
				aff->SetRange( pos->second );
				pos->second->SetAttributes( _AttributesMapping( EmitterProperty::EA_PARTICLE_POSITION_NOISE ) );
				posAff = aff;
				aVM.erase( pos );
			}
			m_AffectorList.push_back( posAff );
		}
		//处理速度
		{
			AffectorVolumeMap::iterator speed = aVM.find( EmitterProperty::EA_PARTICLE_SPEED );
			if ( aVM.end() != speed )//如果有速度
			{
				IAffector* speedAff = NULL;
				AffectorVolumeMap::iterator axis = aVM.find( EmitterProperty::EA_PARTICLE_AXIS_NOISE );
				speed->second->SetAttributes( _AttributesMapping( EmitterProperty::EA_PARTICLE_SPEED ) );
				_SettingVertexStream( EmitterProperty::EA_PARTICLE_SPEED, Math::DST_VECTOR3 );//都需要添加该顶点流
				if ( aVM.end() == axis )
				{
					SpeedAffector* aff = NEW SpeedAffector;
					aff->SetOrientationComponent( m_pOrientation );
					aff->SetRange( speed->second );
					speedAff = aff;
				}
				else
				{
					SpeedAxisAffector* aff = NEW SpeedAxisAffector;
					aff->SetOrientationComponent( m_pOrientation );
					aff->SetAxisRange( axis->second );
					aff->SetSpeedRange( speed->second );
					speedAff = aff;
					aVM.erase( axis );
				}
				aVM.erase( speed );
				m_AffectorList.push_back( speedAff );
			}
		}

		//处理通用的
		FOR_EACH( AffectorVolumeMap, volume, aVM )
		{
			_SettingVertexStream( volume->first, volume->second->GetDataStructType() );
			volume->second->SetAttributes( _AttributesMapping( volume->first ) );
			GeneralAffector* aff = NEW GeneralAffector;
			aff->SetRange( volume->second );
			m_AffectorList.push_back( aff );
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void GeneralEmitter::_InitializeStream( )
	{
		{//设置默认影响器
			BornAffector* aff = NEW BornAffector;
			m_AffectorList.push_back( aff );
		}
		{//设置必要数据
			m_pVertexStream->SetVertexType<vector1f>(
				Attributes::ATTRIBUTE_EMITTER_BORN,
				BufferObjectProperty::DT_FLOAT,
				BufferObjectProperty::DT_HALF_FLOAT );

			m_pVertexStream->SetVertexType<vector2f>(
				Attributes::ATTRIBUTE_EMITTER_OFFSET,
				BufferObjectProperty::DT_FLOAT,
				BufferObjectProperty::DT_HALF_FLOAT );

			m_pVertexStream->SetVertexType<vector2f>(
				Attributes::ATTRIBUTE_TEXCOORD0,
				BufferObjectProperty::DT_FLOAT,
				BufferObjectProperty::DT_HALF_FLOAT );
		}
		ASSERT( 4 == GEC_VERTEX_COUNT_EACH_PARTICLE );
		m_LimitedIndex.SetMax( m_LimitedIndex.Max() * GEC_VERTEX_COUNT_EACH_PARTICLE );
		uint count = m_LimitedIndex.Max();
		m_pVertexStream->ReserveBuffer( count );
		for ( int i = 0; i < count; i += GEC_VERTEX_COUNT_EACH_PARTICLE )
		{
			m_pVertexStream->PushVertexDataFast( Attributes::ATTRIBUTE_EMITTER_OFFSET, vector2f( -1,  1 ) );
			m_pVertexStream->PushVertexDataFast( Attributes::ATTRIBUTE_EMITTER_OFFSET, vector2f( -1, -1 ) );
			m_pVertexStream->PushVertexDataFast( Attributes::ATTRIBUTE_EMITTER_OFFSET, vector2f(  1,  1 ) );
			m_pVertexStream->PushVertexDataFast( Attributes::ATTRIBUTE_EMITTER_OFFSET, vector2f(  1, -1 ) );

			m_pVertexStream->PushVertexDataFast( Attributes::ATTRIBUTE_TEXCOORD0, vector2f( 0, 1 ) );
			m_pVertexStream->PushVertexDataFast( Attributes::ATTRIBUTE_TEXCOORD0, vector2f( 0, 0 ) );
			m_pVertexStream->PushVertexDataFast( Attributes::ATTRIBUTE_TEXCOORD0, vector2f( 1, 1 ) );
			m_pVertexStream->PushVertexDataFast( Attributes::ATTRIBUTE_TEXCOORD0, vector2f( 1, 0 ) );
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void GeneralEmitter::CreateResource( IAsyncSource* host )
	{
		if ( NULL == m_pDensityVolume )
		{
			m_pDensityVolume = NEW RandVolume;
			m_pDensityVolume->SetRange( Math::DST_VECTOR1, vector1f(1), vector1f(1) );
		}
		m_pRender->DependenceSource( host );
		m_pRender->PushMetadata(
			RenderObjectMeshMetadate( 
			RenderObjectProperty::RM_TRIANGLES,
			ReferenceVertexMetadata( m_pVertexStream ),
			SquareIndicesMetadata( m_pVertexStream->GetVertexCount() >> 2 ) ) );
		m_pRender->CreateResource();
	}
	//---------------------------------------------------------------------------------------------------------
	uint GeneralEmitter::_CaculateEmitteCount( float span )//计算发射个数
	{
		int emitterCount = static_cast<const vector1f&>(m_pDensityVolume->GetRandomVolume());
		m_TimeSpan += span;
		float fCount = m_TimeSpan * emitterCount;
		//计算残余时间
		float fFloor = Math::Floor( fCount );
		fCount -= fFloor;
		m_TimeSpan = fCount / emitterCount;
		emitterCount = fFloor;
		return emitterCount;
	}
	//---------------------------------------------------------------------------------------------------------
	void GeneralEmitter::UpdataEmitter( float span )
	{
		int emitterCount = _CaculateEmitteCount( span );
		if ( 0 < emitterCount )
		{
			int reflushBegin = m_LimitedIndex;
			for ( int i = 0; i < emitterCount; ++i )
			{
				FOR_EACH(IAffectorList, affect, m_AffectorList)
				{
					Attributes::ShaderAttributes sa = (*affect)->GetAttributes();
					const Volume& volume = (*affect)->GetAffectorVolume();
					m_pVertexStream->ChangeVertexData( sa, m_LimitedIndex + 0, volume );
					m_pVertexStream->ChangeVertexData( sa, m_LimitedIndex + 1, volume );
					m_pVertexStream->ChangeVertexData( sa, m_LimitedIndex + 2, volume );
					m_pVertexStream->ChangeVertexData( sa, m_LimitedIndex + 3, volume );
				}
				m_LimitedIndex += GEC_VERTEX_COUNT_EACH_PARTICLE;
			}
			m_pVertexStream->SetReflushInterval( reflushBegin, m_LimitedIndex );
			m_pRender->ChangeVertexBufferImmediately( *m_pVertexStream );
		}
	}
}