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
#pragma once
#include "JsonReadable.h"
#include "JsonReader.h"




namespace BohgeEngine
{
	//------------------------------------------------------------------------------
	JsonReadable::JsonReadable(void)
	{
	}
	//------------------------------------------------------------------------------
	JsonReadable::~JsonReadable(void)
	{
	}
	//------------------------------------------------------------------------------
	void JsonReadable::_Load_1( JsonReader& reader )
	{
		ASSERT(false);
	}
	//------------------------------------------------------------------------------
	void JsonReadable::_Load_2( JsonReader& reader )
	{
		ASSERT(false);
	}
	//------------------------------------------------------------------------------
	void JsonReadable::_Load_3( JsonReader& reader )
	{
		ASSERT(false);
	}
	//------------------------------------------------------------------------------
	void JsonReadable::_Load_4( JsonReader& reader )
	{
		ASSERT(false);
	}
	//------------------------------------------------------------------------------
	void JsonReadable::_Load_5( JsonReader& reader )
	{
		ASSERT(false);
	}
	//------------------------------------------------------------------------------
	void JsonReadable::_Load_6( JsonReader& reader )
	{
		ASSERT(false);
	}
	//------------------------------------------------------------------------------
	void JsonReadable::_Load_7( JsonReader& reader )
	{
		ASSERT(false);
	}
	//------------------------------------------------------------------------------
	void JsonReadable::_Load_8( JsonReader& reader )
	{
		ASSERT(false);
	}
	//------------------------------------------------------------------------------
	void JsonReadable::_Load_9( JsonReader& reader )
	{
		ASSERT(false);
	}
	//------------------------------------------------------------------------------
	void JsonReadable::Load( const eastl::string& path )
	{
		JsonReader json;
		json.Open( path );
		switch( json.GetVersion() )
		{
		case 1:_Load_1( json ); break;
		case 2:_Load_2( json ); break;
		case 3:_Load_3( json ); break;
		case 4:_Load_4( json ); break;
		case 5:_Load_5( json ); break;
		case 6:_Load_6( json ); break;
		case 7:_Load_7( json ); break;
		case 8:_Load_8( json ); break;
		case 9:_Load_9( json ); break;
		default:ASSERT(false);
		}
	}

}