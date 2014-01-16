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
//		Object块			//
/////////////////////////
#include "ObjectTile.h"
#include "SceneDataIndex.h"
#include "ObjectFactory.h"
#include "ObjectGroup.h"
#include "Serializer.h"
#include "Light.h"
#include "LightMaped.h"
#include "RealLights.h"
#include "FakeLights.h"
#include "Model.h"
#include "Material.h"
#include "Utility.h"




using namespace std;

namespace BohgeEngine
{
	//-------------------------------------------------------------------------------------------------------
	ObjectTile::ObjectTile(void)
	{
	}
	//-------------------------------------------------------------------------------------------------------
	ObjectTile::~ObjectTile(void)
	{
		for ( ObjectMap::iterator it = m_ObjectMap.begin();
			it != m_ObjectMap.end();
			it ++ )
		{
			SAFE_DELETE( it->second );
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void ObjectTile::Load( const std::string& path, const std::string& Tilename )
	{
		string localpath = path + Tilename;
		Serializer loader( localpath );
		int size;
		loader>>size;
		for ( int i = 0 ; i < size ; i ++ )
		{
			int type;
			string savepath;
			LocatableObject* lo;
			loader>>type>>savepath;
			Utility::ReplaceAllString(savepath, "\\", "/" );
			switch(type)
			{
			case ObjectFactory::OT_LIGHT:lo = _LoadLight( path, savepath );break;
			case ObjectFactory::OT_OBJECTGROUP:
			case ObjectFactory::OT_LOCATABLEMODEL: lo = _LoadObject( path, savepath );break;
			}
			m_ObjectMap.insert( make_pair( Utility::HashCode(savepath), lo) );
		}
	}
	//-------------------------------------------------------------------------------------------------------
	LocatableObject* ObjectTile::_LoadLight( const std::string& path, const std::string& filename )
	{
		Serializer loader(  path + filename );
		int lt,ls;
		vector3f pos;
		Quaternionf rot;
		vector3f sca;
		float range;
		vector3f clr;
		vector2f helo;
		vector4f atten;
		vector2f angle;
		loader>>lt
			>>ls
			>>pos
			>>rot
			>>sca
			>>range
			>>clr
			>>helo
			>>atten
			>>angle;

		LightNode* light;
		switch( lt )
		{
		case LightNode::LT_POINT: light = LightNode::LS_LIGHTMAPED == ls ? static_cast<LightNode*>(NEW LightMapedPoint) : static_cast<LightNode*>(NEW FakePointLight); break;
		case LightNode::LT_DIRECTIONAL: light = LightNode::LS_LIGHTMAPED == ls ? NEW LightMapedDirection : NULL; break;
		case LightNode::LT_SPOT: light = LightNode::LS_LIGHTMAPED == ls ? NEW LightMapedSpot : NULL ; break;
		}
		ASSERT( NULL != light );
		light->SetLocalPosition( pos );
		light->SetLocalRotation( rot );
		light->SetRange(range);
		light->SetColor(clr);
		light->SetAttenuation(atten);
		light->SetHaloSize(helo);
		light->SetLightAngle(angle);
		return light;
	}
	//-------------------------------------------------------------------------------------------------------
	LocatableObject* ObjectTile::_LoadObject( const std::string& path, const std::string& filename )
	{
		Serializer loader(  path + filename );
		string meshname;
		vector3f pos;
		Quaternionf rot;
		vector3f sca;
		int inttype;
		loader>>meshname
			>>inttype
			>>pos
			>>rot
			>>sca;
		
		ObjectFactory::ObjectType type = (ObjectFactory::ObjectType)inttype;
		string respath = path + ObjectEntityFolder + meshname;
		LocatableObject* obj = ObjectFactory::CreatObject( type, respath );
		obj->SetLocalPosition( pos );
		obj->SetLocalRotation( rot );
		obj->SetLocalScale( sca );

		//加载light map;
		int meshcount;
		loader>>meshcount;
		for ( int i = 0; i < meshcount; i ++ )
		{
			int index, texcount;//index是在group中的位置
			loader>>index>>texcount;
			for ( int j = 0 ; j < texcount ; j ++ )
			{
				int materialindex;
				string lightpath;
				loader>>materialindex>>lightpath;
				Utility::ReplaceAllString( lightpath, "\\", "/" );
				switch( type )
				{
				case ObjectFactory::OT_LOCATABLEMODEL:
					{
						LocatableModel* model = static_cast<LocatableModel*>(obj);
						model->LoadLightMap( materialindex, path + lightpath );
					}break;
				case ObjectFactory::OT_OBJECTGROUP:
					{
						ObjectGroup* group = static_cast<ObjectGroup*>(obj);
						group->LoadLightMap( index, materialindex, path + lightpath );
					}break;
				}
			}
		}
		return obj;
	}
	//-------------------------------------------------------------------------------------------------------
	const LocatableObject* ObjectTile::GetLoadedObject( const std::string& Tilename )
	{
		ObjectMap::const_iterator it = m_ObjectMap.find( Utility::HashCode( Tilename ) );
		if ( m_ObjectMap.end() != it )
		{
			return it->second;
		}
		return NULL;
	}

}