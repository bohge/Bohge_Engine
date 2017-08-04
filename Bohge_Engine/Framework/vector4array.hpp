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



//////////////////////////////////////////////
//	3DMath的拆分实现，请不要单独引用该头文件	//
//////////////////////////////////////////////
#ifndef VECTOR4ARRAY
#define VECTOR4ARRAY
#else
#error( "Do not include this file in other files" )
#endif
#include "Object.hpp"



#include <EASTL/vector.h>

namespace BohgeEngine
{
	template<typename T>
	class vector4array : public Object
	{
		RTTI_TYPE_DECLEAR( vector4array );
	private:
		enum Constant
		{
			VAC_DATA_COUNT = 4,
		};
	private:
		typedef eastl::vector< T >	DataArray;
	public:
		typedef T value_type;
	private:
		DataArray		m_Data;
	public:
		vector4array(void){}
		virtual ~vector4array(void){}
	public:
		bool operator == ( const vector4array& rhs )
		{
			return m_Data == rhs.m_Data;
		}
		bool operator != ( const vector4array& rhs )
		{
			return m_Data != rhs.m_Data;
		}
		void Set( int index, const vector4<T>& data )
		{
			memcpy( &m_Data.at( index * VAC_DATA_COUNT ), data.Data(), sizeof(T)*VAC_DATA_COUNT );
		}
		void Set( int index, const Quaternion<T>& data )
		{
			memcpy( &m_Data.at( index * VAC_DATA_COUNT ), data.Data(), sizeof(T)*VAC_DATA_COUNT );
		}
		void Resize( int size )
		{
			m_Data.resize( size * VAC_DATA_COUNT );
		}
		void PushBack( const vector4<T>& data )
		{
			m_Data.push_back( data.m_x );
			m_Data.push_back( data.m_y );
			m_Data.push_back( data.m_z );
			m_Data.push_back( data.m_w );
		}
		uint Size() const
		{
			return m_Data.size() / VAC_DATA_COUNT;
		}
		T* Data()
		{
			return m_Data.data();
		}
		const T* Data() const
		{
			return m_Data.data();
		}
	};
}