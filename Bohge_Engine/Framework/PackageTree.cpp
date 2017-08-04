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
#include "PackageTree.h"
#include "PackageNode.h"



using namespace eastl;
namespace BohgeEngine
{
	RTTI_TYPE_IMPLEMENT( PackageTree );
	//------------------------------------------------------------------------------------------------------
	PackageTree::PackageTree(void)
		:m_pRootNode(NULL),
		m_Index(0)
	{
	}
	//------------------------------------------------------------------------------------------------------
	PackageTree::~PackageTree(void)
	{
		SAFE_DELETE( m_pRootNode );
	}
	//------------------------------------------------------------------------------------------------------
	void PackageTree::Initialize( const vector2d& size )
	{
		m_Rect = Aabrectd( vector2d(0,0), size );
		m_pRootNode = NEW PackageNode(this);
		m_pRootNode->Initialize( m_Rect, NULL );
	}
	//------------------------------------------------------------------------------------------------------
	TextureAtlasData PackageTree::InsertPackage( const vector2d& size )
	{
		for ( FreeSlotMap::iterator begin = m_FreeSlotMap.lower_bound( _GetArea(size) );
			begin != m_FreeSlotMap.end();
			++begin )
		{
			if ( begin->second->isCapable( size ) )
			{
				uint index = m_Index++;
				m_NodeMap.insert( make_pair( index, begin->second ) );//必须先插入，否则会被删除
				const vector2d& data = begin->second->InsertPackage( size );//这里面代码写的不太好，出现上面问题
				return TextureAtlasData( data, size, index );
			}
		}
		ASSERT(false);
		return TextureAtlasData( vector2d(), vector2d(), 0 );
	}
	//------------------------------------------------------------------------------------------------------
	void PackageTree::InsertFreeSlot( const vector2d& size, PackageNode* node )
	{
		m_FreeSlotMap.insert( make_pair( _GetArea(size), node ) );
	}
	//------------------------------------------------------------------------------------------------------
	void PackageTree::RemoveFreeSlot( const vector2d& size, PackageNode* node )
	{
		pair< FreeSlotMap::iterator, FreeSlotMap::iterator> range = m_FreeSlotMap.equal_range( _GetArea(size) );
		for ( FreeSlotMap::iterator slot = range.first;
			slot != range.second;
			++slot )
		{
			if ( slot->second == node )
			{
				m_FreeSlotMap.erase( slot );
				break;
			}
		}
	}
	//------------------------------------------------------------------------------------------------------
	void PackageTree::ErasePackage( handle h )
	{
		NodeMap::iterator it = m_NodeMap.find( h );
		ASSERT( m_NodeMap.end() != it );
		it->second->ReleaseNode();
		m_NodeMap.erase( it );
	}
}