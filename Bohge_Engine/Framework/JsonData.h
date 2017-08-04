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
#include "Object.hpp"


#include <EASTL/string.h>

namespace BohgeEngine
{
	namespace JsonProperty
	{
		enum JsonDataType
		{
			JDT_STRING = 0,
			JDT_BOOL,
			JDT_INT,
			JDT_FLOAT,
			JDT_OBJECT,
			JDT_OBJECT_PTR,//指向obj的指针
			JDT_ARRAY_STRING,
			JDT_ARRAY_INT,//整数数组
			JDT_ARRAY_FLOAT,//浮点数组
			JDT_ARRAY_OBJECT,//对象数组
		};

		//------------------------------------------------------------------------------
		class BOHGE_FRAMEWORK JsonDataMark
		{
		private:
			const char*					m_Name;
			JsonProperty::JsonDataType	m_eDataType;
		public:
			JsonDataMark( const char* name, JsonProperty::JsonDataType type );
			~JsonDataMark( void ){};
		public:
			BOHGE_FORCEINLINE void SetName( const char* name )
			{
				m_Name = name;
			}
			BOHGE_FORCEINLINE const char* GetName() const
			{
				return m_Name;
			}
			BOHGE_FORCEINLINE JsonProperty::JsonDataType GetDataType() const
			{
				return m_eDataType;
			}
		};
	}
	//------------------------------------------------------------------------------
	struct BOHGE_FRAMEWORK IJsonDataSet : public Object
	{
		RTTI_ABSTRACT_TYPE_DECLEAR( IJsonDataSet );
	public:
		virtual ~IJsonDataSet(void){}
	private:
		virtual void* _GetDataAddress( uint index ) = 0;
		virtual const void* _GetDataAddress( uint index ) const = 0;
	protected:
		virtual void _OnDataLoadded_1(){}
		virtual void _OnDataLoadded_2(){}
		virtual void _OnDataLoadded_3(){}
		virtual void _OnDataLoadded_4(){}
		virtual void _OnDataLoadded_5(){}
		virtual void _OnDataLoadded_6(){}
		virtual void _OnDataLoadded_7(){}
		virtual void _OnDataLoadded_8(){}
		virtual void _OnDataLoadded_9(){}
	public:
		virtual uint GetDataCount() const = 0;//返回数据的
		virtual void OnDataLoadded( uint index ) = 0;
		virtual const JsonProperty::JsonDataMark& GetJsonDataMark( uint index ) const = 0;//返回数据参数
	public:
		template< typename T >
		BOHGE_FORCEINLINE void SetData( uint index, const T& data )
		{
			T* ptr = static_cast<T*>( _GetDataAddress(index) );
			*ptr = data;
		}
		template< typename T >
		BOHGE_FORCEINLINE const T& GetData( uint index ) const
		{
			return *static_cast<const T*>( _GetDataAddress(index) );			
		}
		BOHGE_FORCEINLINE void* GetDataAddress( uint index )
		{
			return _GetDataAddress( index );
		}
		BOHGE_FORCEINLINE const void* GetDataAddress( uint index ) const
		{
			return _GetDataAddress( index );
		}
	};
	//------------------------------------------------------------------------------
	struct BOHGE_FRAMEWORK IJsonArray : public Object
	{
		RTTI_ABSTRACT_TYPE_DECLEAR( IJsonArray );
	private:
		eastl::vector<IJsonDataSet*>	m_SetArray;
	public:
		virtual ~IJsonArray();
	private:
		virtual IJsonDataSet* _Factory() = 0;
	public:
		void Clear();
	public:
		BOHGE_FORCEINLINE void Resize( uint size )
		{
			Clear();
			for ( int i = 0 ; i < size ; ++ i )
			{
				m_SetArray.push_back( _Factory() );
			}
		}
		BOHGE_FORCEINLINE uint Getsize() const
		{
			return m_SetArray.size();
		}
		BOHGE_FORCEINLINE void Insert( int i , const IJsonDataSet& data )
		{
			IJsonDataSet* jd = static_cast<IJsonDataSet*>( data.Duplicate() );
			m_SetArray.insert( m_SetArray.begin() + i, jd );
		}
		BOHGE_FORCEINLINE const IJsonDataSet& At(int i ) const
		{
			return *m_SetArray[i];
		}
		BOHGE_FORCEINLINE const IJsonDataSet& operator[](int i ) const
		{
			return At(i);
		}
		template< typename T >
		BOHGE_FORCEINLINE const T& At( int i ) const
		{
			return static_cast<const T&>( At(i) );
		}
		BOHGE_FORCEINLINE IJsonDataSet& At(int i )
		{
			return *m_SetArray[i];
		}
		BOHGE_FORCEINLINE IJsonDataSet& operator[](int i )
		{
			return At(i);
		}
		template< typename T >
		BOHGE_FORCEINLINE T& At( int i )
		{
			return static_cast<T&>( At(i) );
		}
	};
}

//数组声明
#define JSON_ARRAY_DATA_STRUCT( stname )\
struct stname##_Array : public BohgeEngine::IJsonArray\
		{BohgeEngine::IJsonDataSet* _Factory(){return NEW stname;}};

//数据结构声明
#define JSON_DATA_STRUCT_DECLEAR( stname, count ) \
		typedef void (stname::*fn)(void);\
		static BohgeEngine::JsonProperty::JsonDataMark	m_DataMarkArray[(count)];\
		static stname::fn		m_OnLoadFunc[(count)];\
	private:\
		void*				m_DataAddress[(count)];\
	public:\
		stname##(void);\
	private:\
		void* stname::_GetDataAddress( uint index ){return m_DataAddress[index];}\
		const void* stname::_GetDataAddress( uint index ) const{return m_DataAddress[index];}\
	public:\
		virtual uint GetDataCount() const;\
		const BohgeEngine::JsonProperty::JsonDataMark& stname::GetJsonDataMark( uint index ) const{return m_DataMarkArray[index];}\
		virtual void OnDataLoadded( uint index ){ (this->*m_OnLoadFunc[index])(); }


//数据结构实现
#define JSON_DATA_STRUCT_IMPLEMENT_1( stname, name_1, type_1, data_1 )\
	BohgeEngine::JsonProperty::JsonDataMark stname::m_DataMarkArray[1] = \
	{JsonProperty::JsonDataMark( (name_1), (type_1) )};\
	stname::fn stname::m_OnLoadFunc[1] = \
	{ &stname::_OnDataLoadded_1 };\
	stname::stname( void ){m_DataAddress[0] = data_1;}\
	uint stname::GetDataCount() const{return 1;}


#define JSON_DATA_STRUCT_IMPLEMENT_2( stname, name_1, type_1, data_1, name_2, type_2, data_2 )\
	BohgeEngine::JsonProperty::JsonDataMark stname::m_DataMarkArray[2] = \
	{\
		JsonProperty::JsonDataMark( (name_1), (type_1) ),\
		JsonProperty::JsonDataMark( (name_2), (type_2) )\
	};\
	stname::fn stname::m_OnLoadFunc[2] = \
	{\
		&stname::_OnDataLoadded_1,\
		&stname::_OnDataLoadded_2\
	};\
	stname::stname( void )\
	{\
		m_DataAddress[0] = data_1;\
		m_DataAddress[1] = data_2;\
	}\
	uint stname::GetDataCount() const{return 2;}

#define JSON_DATA_STRUCT_IMPLEMENT_3( stname, name_1, type_1, data_1, name_2, type_2, data_2, name_3, type_3, data_3 )\
	BohgeEngine::JsonProperty::JsonDataMark stname::m_DataMarkArray[3] = \
	{\
		JsonProperty::JsonDataMark( (name_1), (type_1) ),\
		JsonProperty::JsonDataMark( (name_2), (type_2) ),\
		JsonProperty::JsonDataMark( (name_3), (type_3) )\
	};\
	stname::fn stname::m_OnLoadFunc[3] = \
	{\
		&stname::_OnDataLoadded_1,\
		&stname::_OnDataLoadded_2,\
		&stname::_OnDataLoadded_3\
	};\
	stname::stname( void )\
	{\
		m_DataAddress[0] = data_1;\
		m_DataAddress[1] = data_2;\
		m_DataAddress[2] = data_3;\
	}\
	uint stname::GetDataCount() const{return 3;}

#define JSON_DATA_STRUCT_IMPLEMENT_4( stname, name_1, type_1, data_1, name_2, type_2, data_2, name_3, type_3, data_3, name_4, type_4, data_4 )\
	BohgeEngine::JsonProperty::JsonDataMark stname::m_DataMarkArray[4] = \
	{\
		JsonProperty::JsonDataMark( (name_1), (type_1) ),\
		JsonProperty::JsonDataMark( (name_2), (type_2) ),\
		JsonProperty::JsonDataMark( (name_3), (type_3) ),\
		JsonProperty::JsonDataMark( (name_4), (type_4) )\
	};\
	stname::fn stname::m_OnLoadFunc[4] = \
	{\
		&stname::_OnDataLoadded_1,\
		&stname::_OnDataLoadded_2,\
		&stname::_OnDataLoadded_3,\
		&stname::_OnDataLoadded_4\
	};\
	stname::stname( void )\
	{\
		m_DataAddress[0] = data_1;\
		m_DataAddress[1] = data_2;\
		m_DataAddress[2] = data_3;\
		m_DataAddress[3] = data_4;\
	}\
	uint stname::GetDataCount() const{return 4;}

#define JSON_DATA_STRUCT_IMPLEMENT_5( stname, name_1, type_1, data_1, name_2, type_2, data_2, name_3, type_3, data_3, name_4, type_4, data_4, name_5, type_5, data_5 )\
	BohgeEngine::JsonProperty::JsonDataMark stname::m_DataMarkArray[5] = \
	{\
		JsonProperty::JsonDataMark( (name_1), (type_1) ),\
		JsonProperty::JsonDataMark( (name_2), (type_2) ),\
		JsonProperty::JsonDataMark( (name_3), (type_3) ),\
		JsonProperty::JsonDataMark( (name_4), (type_4) ),\
		JsonProperty::JsonDataMark( (name_5), (type_5) )\
	};\
	stname::fn stname::m_OnLoadFunc[5] = \
	{\
		&stname::_OnDataLoadded_1,\
		&stname::_OnDataLoadded_2,\
		&stname::_OnDataLoadded_3,\
		&stname::_OnDataLoadded_4,\
		&stname::_OnDataLoadded_5\
	};\
	stname::stname( void )\
	{\
		m_DataAddress[0] = data_1;\
		m_DataAddress[1] = data_2;\
		m_DataAddress[2] = data_3;\
		m_DataAddress[3] = data_4;\
		m_DataAddress[4] = data_5;\
	}\
	uint stname::GetDataCount() const{return 5;}

#define JSON_DATA_STRUCT_IMPLEMENT_6( stname, name_1, type_1, data_1, name_2, type_2, data_2, name_3, type_3, data_3, name_4, type_4, data_4, name_5, type_5, data_5, name_6, type_6, data_6 )\
	BohgeEngine::JsonProperty::JsonDataMark stname::m_DataMarkArray[6] = \
	{\
		JsonProperty::JsonDataMark( (name_1), (type_1) ),\
		JsonProperty::JsonDataMark( (name_2), (type_2) ),\
		JsonProperty::JsonDataMark( (name_3), (type_3) ),\
		JsonProperty::JsonDataMark( (name_4), (type_4) ),\
		JsonProperty::JsonDataMark( (name_5), (type_5) ),\
		JsonProperty::JsonDataMark( (name_6), (type_6) )\
	};\
	stname::fn stname::m_OnLoadFunc[6] = \
	{\
		&stname::_OnDataLoadded_1,\
		&stname::_OnDataLoadded_2,\
		&stname::_OnDataLoadded_3,\
		&stname::_OnDataLoadded_4,\
		&stname::_OnDataLoadded_5,\
		&stname::_OnDataLoadded_6\
	};\
	stname::stname( void )\
	{\
		m_DataAddress[0] = data_1;\
		m_DataAddress[1] = data_2;\
		m_DataAddress[2] = data_3;\
		m_DataAddress[3] = data_4;\
		m_DataAddress[4] = data_5;\
		m_DataAddress[5] = data_6;\
	}\
	uint stname::GetDataCount() const{return 6;}

#define JSON_DATA_STRUCT_IMPLEMENT_7( stname, name_1, type_1, data_1, name_2, type_2, data_2, name_3, type_3, data_3, name_4, type_4, data_4, name_5, type_5, data_5, name_6, type_6, data_6, name_7, type_7, data_7 )\
	BohgeEngine::JsonProperty::JsonDataMark stname::m_DataMarkArray[7] = \
	{\
	JsonProperty::JsonDataMark( (name_1), (type_1) ),\
	JsonProperty::JsonDataMark( (name_2), (type_2) ),\
	JsonProperty::JsonDataMark( (name_3), (type_3) ),\
	JsonProperty::JsonDataMark( (name_4), (type_4) ),\
	JsonProperty::JsonDataMark( (name_5), (type_5) ),\
	JsonProperty::JsonDataMark( (name_6), (type_6) ),\
	JsonProperty::JsonDataMark( (name_7), (type_7) )\
	};\
	stname::fn stname::m_OnLoadFunc[7] = \
	{\
	&stname::_OnDataLoadded_1,\
	&stname::_OnDataLoadded_2,\
	&stname::_OnDataLoadded_3,\
	&stname::_OnDataLoadded_4,\
	&stname::_OnDataLoadded_5,\
	&stname::_OnDataLoadded_6,\
	&stname::_OnDataLoadded_7\
	};\
	stname::stname( void )\
	{\
	m_DataAddress[0] = data_1;\
	m_DataAddress[1] = data_2;\
	m_DataAddress[2] = data_3;\
	m_DataAddress[3] = data_4;\
	m_DataAddress[4] = data_5;\
	m_DataAddress[5] = data_6;\
	m_DataAddress[6] = data_7;\
	}\
	uint stname::GetDataCount() const{return 7;}


#define JSON_DATA_STRUCT_IMPLEMENT_8( stname, name_1, type_1, data_1, name_2, type_2, data_2, name_3, type_3, data_3, name_4, type_4, data_4, name_5, type_5, data_5, name_6, type_6, data_6, name_7, type_7, data_7, name_8, type_8, data_8 )\
	BohgeEngine::JsonProperty::JsonDataMark stname::m_DataMarkArray[8] = \
	{\
	JsonProperty::JsonDataMark( (name_1), (type_1) ),\
	JsonProperty::JsonDataMark( (name_2), (type_2) ),\
	JsonProperty::JsonDataMark( (name_3), (type_3) ),\
	JsonProperty::JsonDataMark( (name_4), (type_4) ),\
	JsonProperty::JsonDataMark( (name_5), (type_5) ),\
	JsonProperty::JsonDataMark( (name_6), (type_6) ),\
	JsonProperty::JsonDataMark( (name_7), (type_7) ),\
	JsonProperty::JsonDataMark( (name_8), (type_8) )\
	};\
	stname::fn stname::m_OnLoadFunc[8] = \
	{\
	&stname::_OnDataLoadded_1,\
	&stname::_OnDataLoadded_2,\
	&stname::_OnDataLoadded_3,\
	&stname::_OnDataLoadded_4,\
	&stname::_OnDataLoadded_5,\
	&stname::_OnDataLoadded_6,\
	&stname::_OnDataLoadded_7,\
	&stname::_OnDataLoadded_8\
	};\
	stname::stname( void )\
	{\
	m_DataAddress[0] = data_1;\
	m_DataAddress[1] = data_2;\
	m_DataAddress[2] = data_3;\
	m_DataAddress[3] = data_4;\
	m_DataAddress[4] = data_5;\
	m_DataAddress[5] = data_6;\
	m_DataAddress[6] = data_7;\
	m_DataAddress[7] = data_8;\
	}\
	uint stname::GetDataCount() const{return 8;}

#define JSON_DATA_STRUCT_IMPLEMENT_9( stname, name_1, type_1, data_1, name_2, type_2, data_2, name_3, type_3, data_3, name_4, type_4, data_4, name_5, type_5, data_5, name_6, type_6, data_6, name_7, type_7, data_7, name_8, type_8, data_8, name_9, type_9, data_9 )\
	BohgeEngine::JsonProperty::JsonDataMark stname::m_DataMarkArray[9] = \
	{\
	JsonProperty::JsonDataMark( (name_1), (type_1) ),\
	JsonProperty::JsonDataMark( (name_2), (type_2) ),\
	JsonProperty::JsonDataMark( (name_3), (type_3) ),\
	JsonProperty::JsonDataMark( (name_4), (type_4) ),\
	JsonProperty::JsonDataMark( (name_5), (type_5) ),\
	JsonProperty::JsonDataMark( (name_6), (type_6) ),\
	JsonProperty::JsonDataMark( (name_7), (type_7) ),\
	JsonProperty::JsonDataMark( (name_8), (type_8) ),\
	JsonProperty::JsonDataMark( (name_9), (type_9) )\
	};\
	stname::fn stname::m_OnLoadFunc[9] = \
	{\
	&stname::_OnDataLoadded_1,\
	&stname::_OnDataLoadded_2,\
	&stname::_OnDataLoadded_3,\
	&stname::_OnDataLoadded_4,\
	&stname::_OnDataLoadded_5,\
	&stname::_OnDataLoadded_6,\
	&stname::_OnDataLoadded_7,\
	&stname::_OnDataLoadded_8,\
	&stname::_OnDataLoadded_9\
	};\
	stname::stname( void )\
	{\
	m_DataAddress[0] = data_1;\
	m_DataAddress[1] = data_2;\
	m_DataAddress[2] = data_3;\
	m_DataAddress[3] = data_4;\
	m_DataAddress[4] = data_5;\
	m_DataAddress[5] = data_6;\
	m_DataAddress[6] = data_7;\
	m_DataAddress[7] = data_8;\
	m_DataAddress[8] = data_9;\
	}\
	uint stname::GetDataCount() const{return 9;}