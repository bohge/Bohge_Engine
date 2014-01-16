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
//		ObjectÖ÷¹Ü		//
/////////////////////////
#include "ObjectManager.h"
#include "ObjectTile.h"
#include "Serializer.h"
#include "Utility.h"

using namespace std;

namespace BohgeEngine
{

	//-------------------------------------------------------------------------------------------------------
	ObjectManager::ObjectManager( const ObjectProperty& ppy )
		:m_Property(ppy)
	{
	}
	//-------------------------------------------------------------------------------------------------------
	ObjectManager::~ObjectManager()
	{
		this->Unload();
	}
	//-------------------------------------------------------------------------------------------------------
	void ObjectManager::Load( const std::string& path )
	{
		string ObjectPath = path + ObjectFolder;
		Serializer loader( ObjectPath+ObjectInfoList );

		int size;
		loader>>size;
		for ( int i = 0; i < size; i ++ )
		{
			vector2d offset;
			string tilename;
			loader>>offset>>tilename;
			Utility::ReplaceAllString(tilename, "\\", "/" );
			ObjectTile* temp = new ObjectTile( );
			temp->Load( ObjectPath, tilename );
			m_ObjectTiles.push_back( temp );
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void ObjectManager::Unload()
	{
		for ( ObjectTiles::iterator it = m_ObjectTiles.begin();
			it != m_ObjectTiles.end();
			it ++ )
		{
			SAFE_DELETE( *it );
		}
	}
	//-------------------------------------------------------------------------------------------------------
	const LocatableObject* ObjectManager::GetLoadedObject( const std::string& Tilename )
	{
		for ( ObjectTiles::iterator it = m_ObjectTiles.begin();
			it != m_ObjectTiles.end();
			it ++ )
		{
			const LocatableObject* lo = (*it)->GetLoadedObject( Tilename );
			if ( NULL != lo )
			{
				return lo;
			}
		}
		return NULL;
	}

}