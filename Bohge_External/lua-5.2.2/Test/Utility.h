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



////////////////////
//	一些工具	 //
///////////////////
#pragma once


#include <string>
#include <vector>
#include <list>

namespace Utility
{
	//------------------------------------------------------------------------------------------------------------
	template<typename T>
	bool isInclude( const std::list<T>& lhs, const T& rhs )
	{
		typedef typename  std::list< T >::const_iterator ListCIT;
		for ( ListCIT it = lhs.begin();
			it != lhs.end();
			++ it  )
		{
			if( (*it) == rhs )
			{
				return true;
			}
		}
		return false;
	}
	//------------------------------------------------------------------------------------------------------------
	static unsigned int HashCode( const char * str )
	{
		unsigned int len = strlen(str);
		unsigned int hash = 0;
		for(size_t i = 0; i < len; ++i) 
			hash = 65599 * hash + str[i];
		return hash ^ (hash >> 16);
	}
	//------------------------------------------------------------------------------------------------------------
	static unsigned int HashCode( const std::string& str )
	{
		unsigned int hash = 0;
		for(size_t i = 0; i < str.size(); ++i) 
			hash = 65599 * hash + str[i];
		return hash ^ (hash >> 16);
	}
}