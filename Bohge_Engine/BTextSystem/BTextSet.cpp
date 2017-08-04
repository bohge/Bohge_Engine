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
#include "Framework/StringConvert.hpp"
#include "Framework/JsonReader.h"
#include "DataLoader_1.h"
#include "FontEntity.h"
#include "BTextSet.h"


const wchar_t* NOSTRING = L"Error ID!";

using namespace eastl;
using namespace BohgeEngine;
namespace BTextSystemPlugin
{
	//---------------------------------------------------------------------------------------------------------
	BTextSet::BTextSet(void)
		:m_pFontEntity(NEW FontEntity),
		m_LastID(1000000)
	{
	}
	//---------------------------------------------------------------------------------------------------------
	BTextSet::~BTextSet(void)
	{
		SAFE_DELETE( m_pFontEntity );
		FOR_EACH( TextMap, text, m_TextMap )
		{
			SAFE_DELETE( text->second );
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void BTextSet::_Load_1( JsonReader& reader )
	{
		TextSet_1 data;
		reader.Read( data );
		m_pFontEntity->PushMetadata( PathMetadata( data.m_Font ) );
		for ( int i = 0 ; i < data.m_Texts.Getsize(); ++ i )
		{
			Text_1& text = data.m_Texts.At<Text_1>( i );
			wstring* wstr = NEW wstring;
			Utility::UTF8ToUTF16( text.m_Text, *wstr );
			m_TextMap.insert( make_pair( text.m_ID, wstr ) );
			ASSERT( m_LastID > text.m_ID );
		}
	}
	//---------------------------------------------------------------------------------------------------------
	int BTextSet::InsertString( const eastl::string& str )
	{
		wstring* wstr = NEW wstring;
		Utility::UTF8ToUTF16( str, *wstr );
		m_TextMap.insert( make_pair( m_LastID, wstr ) );
		return m_LastID ++;
	}
	//---------------------------------------------------------------------------------------------------------
	void BTextSet::GetText( int id, const BohgeEngine::vector2d& fontsize, BohgeEngine::TextString& ts )
	{
		TextMap::iterator it = m_TextMap.find( id );
		if ( m_TextMap.end() == it  )
		{
			m_pFontEntity->LoadText( fontsize, NOSTRING, ts );
		}
		else
		{
			m_pFontEntity->LoadText( fontsize, *it->second, ts );
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void BTextSet::CreateResource( BohgeEngine::IAsyncSource* source )
	{
		m_pFontEntity->DependenceSource( source );
		m_pFontEntity->CreateResource();
	}
	//---------------------------------------------------------------------------------------------------------
	void BTextSet::ReleaseText( int id, const BohgeEngine::vector2d& fontsize )
	{
		TextMap::iterator it = m_TextMap.find( id );
		if ( m_TextMap.end() == it  )
		{
			m_pFontEntity->UnLoadText( fontsize, NOSTRING );
		}
		else
		{
			m_pFontEntity->UnLoadText( fontsize, *it->second );
		}
	}

}