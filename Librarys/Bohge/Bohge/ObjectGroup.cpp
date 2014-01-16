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



/////////////////////////
//		实体团体	   //
/////////////////////////
#include "ObjectGroup.h"
#include "Model.h"
#include "Emitter.h"
#include "Light.h"
#include "ObjectRoot.h"
#include "Serializer.h"
#include "ObjectDataIndex.h"
#include "Light.h"
#include "RealLights.h"
#include "FakeLights.h"

using namespace std;
namespace BohgeEngine
{
	//--------------------------------------------------------------------------------------------------------------------------------------
	ObjectGroup::ObjectGroup( )
		:LocatableObject(this)
	{
		SetRender( false );
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	ObjectGroup::~ObjectGroup( )
	{
		for ( ObjectVector::iterator it = m_ObjectVector.begin();
			it != m_ObjectVector.end();
			it ++ )
		{
			SAFE_DELETE( *it );
		}
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	void ObjectGroup::_LoadObjectVersion_1( Serializer& loader, const std::string& path )
	{
		bool isNeedVirtualRoot;
		loader>>isNeedVirtualRoot;//是否需要徐根
		int saveCount;//加载的个数
		loader>>saveCount;
		m_ObjectVector.reserve( isNeedVirtualRoot ? saveCount + 1: saveCount );
		if ( isNeedVirtualRoot )//当需要虚根的时候
		{
			ObjectRoot* root = NEW ObjectRoot;
			AttachNode( root );//附加到自己这个根节点上
			vector3f pos;
			vector3f scale;
			Quaternionf rot;
			loader>>pos>>rot>>scale;
			root->SetLocalPosition( pos );
			root->SetLocalRotation( rot );
			root->SetLocalScale( scale );
			m_ObjectVector.push_back( NEW ObjectDataInfo( ObjectGlobelDefined::OT_ROOT, root ) );
		}
		{//包围盒
			vector3f min,max;
			loader>>min>>max;
			m_BindBox.Contains( min );
			m_BindBox.Contains( max );
			m_TransedBinBox = m_BindBox;
		}
		for ( int i = 0; i < saveCount;	i ++ )
		{
			string subpath;
			int entitytype;
			vector2d index;
			loader
				>>entitytype
				>>index
				>>subpath;
			IAttributeNode* father = index.m_x >= 0 ? m_ObjectVector[index.m_x]->GetObjectPtr()->GetNodeByIndex(index.m_y) : NULL;
			IAttributeNode* object;
			switch ( entitytype )
			{
			case ObjectGlobelDefined::OT_MODEL: object = _LoadModel( path + PLANTFORMDELIMITER + subpath ); break;
			case ObjectGlobelDefined::OT_EMITTR: object = _LoadEmitter( path + PLANTFORMDELIMITER + subpath ); break;
			case ObjectGlobelDefined::OT_LIGHT: object = _LoadLight( path + PLANTFORMDELIMITER + subpath ); break;
			}
			if ( index.m_x < 0 )//根节点
			{
				AttachNode( object );//附加到自己这个根节点上
			}
			if ( NULL != father )
			{
				father->AttachNode( object );
			}
			m_ObjectVector.push_back( NEW ObjectDataInfo( (ObjectGlobelDefined::Type)entitytype, object ) );
		}
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	void ObjectGroup::LoadObject( const std::string& path )
	{
		Serializer loader( path + PLANTFORMDELIMITER + ObjectFile );
		int ver;
		loader>>ver;
		m_Name = path;
		switch( ver )
		{
		case ObjectVersion::ObjectVersion1:_LoadObjectVersion_1( loader, path );break;
		}
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	IAttributeNode* ObjectGroup::_LoadModel( const std::string& path )
	{
		LocatableModel* model = NEW LocatableModel;
		string meshname;
		vector3f pos;
		Quaternionf rot;
		vector3f sca;
		Serializer loader( path );
		loader>>meshname
			>>pos
			>>rot
			>>sca;
		string meshpath = path.substr( 0, path.find_last_of("/\\")+1 );
		model->LoadModel( meshpath + meshname );
		model->SetLocalPosition(pos);
		model->SetLocalRotation(rot);
		model->SetLocalScale(sca);
		return model;
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	IAttributeNode* ObjectGroup::_LoadEmitter( const std::string& path )
	{
		Serializer loader( path );
		vector3f		Position;//粒子的发射位置
		vector3f		NoiseRange;//粒子的抖动噪点
		Quaternionf		Rotation;//粒子的发生放向
		vector3f		Acceleration;//粒子的加速度
		vector2f		Speed;//粒子的速度
		float			EndSpeed;//粒子结束速度
		vector3f		Angle;//粒子的角度
		Color			ColorBegin1;//粒子的颜色
		Color			ColorBegin2;
		Color			ColorEnd1;
		Color			ColorEnd2;
		vector2f		ColorPow;//粒子颜色曲率
		vector2d		ParticelLife;//粒子生命时间
		vector2f		AlphaPow;//粒子alpha的曲率
		vector2f		SizeBegin;//粒子尺寸
		vector2f		SizeEnd;
		vector2f		SizePow;//粒子尺寸曲率
		vector2d		CountPerSec;//每秒喷射
		int				LifeTime;//发射器生命周期
		std::string		TextureName;
		loader
			>>Position//粒子的发射位置
			>>NoiseRange//粒子的抖动噪点
			>>Rotation//粒子的旋转
			>>Acceleration//粒子的加速度
			>>Speed//粒子的速度
			>>EndSpeed//粒子结束速度
			>>Angle//粒子的角度
			>>ColorBegin1//粒子的颜色
			>>ColorBegin2
			>>ColorEnd1
			>>ColorEnd2
			>>ColorPow//粒子颜色曲率
			>>ParticelLife//粒子生命时间
			>>AlphaPow//粒子alpha的曲率
			>>SizeBegin//粒子尺寸
			>>SizeEnd
			>>SizePow//粒子尺寸曲率
			>>CountPerSec//每秒喷射
			>>LifeTime//发射器生命周期
			>>TextureName;
		string texpath = path.substr( 0, path.find_last_of("/\\")+1 );
		Emitter* emit = NEW Emitter(
			Position,//粒子的发射位置
			NoiseRange,//粒子的抖动噪点
			FORWARD,
			Acceleration,//粒子的加速度
			Speed,//粒子的速度
			EndSpeed,//粒子结束速度
			Angle,//粒子的角度
			ColorBegin1,//粒子的颜色
			ColorBegin2,
			ColorEnd1,
			ColorEnd2,
			ColorPow,//粒子颜色曲率
			ParticelLife,//粒子生命时间
			AlphaPow,//粒子alpha的曲率
			SizeBegin,//粒子尺寸
			SizeEnd,
			SizePow,//粒子尺寸曲率
			CountPerSec,//每秒喷射
			LifeTime,//发射器生命周期
			texpath + TextureName
			);
		emit->SetLocalRotation( Rotation );
		{//包围盒
			vector3f min,max;
			loader>>min>>max;
			emit->WorldBindBox().Contains( min );
			emit->WorldBindBox().Contains( max );			
		}
		return emit;
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	IAttributeNode* ObjectGroup::_LoadLight( const std::string& path )
	{
		int lt,ls;
		vector3f pos;
		Quaternionf rot;
		vector3f sca;
		vector3f dir;
		float range;
		vector3f clr;
		vector2f helo;
		vector4f atten;
		vector2f angle;
		Serializer loader( path );
		loader>>lt
			>>ls
			>>pos
			>>rot
			>>sca
			>>dir
			>>range
			>>clr
			>>helo
			>>atten
			>>angle;
		LightNode* node;
		switch( lt )
		{
		case LightNode::LT_POINT:
			{
				if( LightNode::LS_REAL == ls )
					node = NEW RealPointLight;
				else
					node = NEW FakePointLight;
			}break;
		case LightNode::LT_DIRECTIONAL:
			{
				node = LightNode::LS_REAL == ls ? NEW RealDirectionLight : NULL;
			}break;
		case LightNode::LT_SPOT:
			{
				node = LightNode::LS_REAL == ls ? NEW RealSpotLight : NULL;
			}break;
		}
		node->SetLocalPosition( pos );
		node->SetLocalRotation( rot );
		node->SetRange(range);
		node->SetColor(clr);
		node->SetAttenuation(atten);
		node->SetHaloSize(helo);
		node->SetLightAngle(angle);
		return node;
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	void ObjectGroup::Update( uint milisecon )
	{
		bool islocatablechange = isChange();
		LocatableObject::Update(milisecon);
		if ( islocatablechange 
			|| isFatherChange() )
		{
			Matrix44f* trans = (Matrix44f*)(this->Attribute( IAttributeNode::ATT_WORLDTRANSFORM ));
			m_TransedBinBox =  NULL == trans ? m_BindBox : m_BindBox * *trans;
		}
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	bool ObjectGroup::isInview( const Frustumf& cFru )
	{
		return IntersectedType::IT_NO != cFru.Intersect( m_TransedBinBox );
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	const void* ObjectGroup::Attribute( const Node_Attribute& name ) const
	{
		return LocatableAttribute( name );
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	void ObjectGroup::LoadLightMap( int modindex, int matindex, const std::string& path )
	{
		switch( m_ObjectVector[modindex]->GetType() )
		{
		case ObjectGlobelDefined::OT_MODEL:
			{
				Model* mod = static_cast< Model* >( m_ObjectVector[modindex]->GetObjectPtr() );
				mod->LoadLightMap( matindex, path );
			}break;
		default:
			ASSERT( false && "undefined ObjectGlobelDefined!" );
		}
	}

}