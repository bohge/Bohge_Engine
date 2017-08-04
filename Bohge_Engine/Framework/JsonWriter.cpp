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
#include "JsonDefualtDataType.h"
#include "JsonWriter.h"
#include "IIOSystem.h"
#include "IFile.h"
#include "Log.h"

#include <rapidjson/document.h>     // rapidjson's DOM-style API
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>
#include <EASTL/string.h>



#define M_JSON (*static_cast<rapidjson::Document*>(m_pJsonNode))
#define M_VALUE (*static_cast<rapidjson::Value*>(m_pDataValue))
using namespace eastl;
namespace BohgeEngine
{
	RTTI_TYPE_IMPLEMENT( JsonWriter );
	//------------------------------------------------------------------------------
	JsonWriter::JsonWriter(void)
		:m_pJsonNode( NEW rapidjson::Document ),
		m_pDataValue( NEW rapidjson::Value ),
		m_pIFile(NULL)
	{
	}
	//------------------------------------------------------------------------------
	JsonWriter::~JsonWriter(void)
	{
		SAFE_DELETE( m_pJsonNode );
		SAFE_DELETE( m_pDataValue );
		Close();
	}
	//------------------------------------------------------------------------------
	void JsonWriter::Open( uint v, const eastl::string& path )
	{
		M_JSON.SetObject();
		M_JSON.AddMember( "Version", v, M_JSON.GetAllocator() );
		m_pIFile = IIOSystem::Instance()->FileFactory( path );
		m_pIFile->OpenFile( IFile::AT_WRITE );
	}
	//------------------------------------------------------------------------------
	void JsonWriter::Close()
	{
		if ( m_pIFile )
		{
			m_pIFile->CloseFile();
			IIOSystem::Instance()->RecycleBin( m_pIFile );
			m_pIFile = NULL;
		}
	}
	//------------------------------------------------------------------------------
	static void _DoWriteData( rapidjson::Value& host, const IJsonDataSet& dataset, rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>& alloc )
	{
		for ( uint i = 0 ; i < dataset.GetDataCount() ; ++ i )
		{
			const JsonProperty::JsonDataMark& mark = dataset.GetJsonDataMark( i );
			switch( mark.GetDataType() )
			{
			case JsonProperty::JDT_STRING:
				{
					rapidjson::Value::StringRefType str( mark.GetName() );
					rapidjson::Value::StringRefType val( dataset.GetData<eastl::string>( i ).c_str() );
					host.AddMember( str, val, alloc );
				}break;
			case JsonProperty::JDT_BOOL:
				{
					rapidjson::Value::StringRefType str( mark.GetName() );
					host.AddMember( str, dataset.GetData<bool>( i ), alloc );
				}break;
			case JsonProperty::JDT_INT:
				{
					rapidjson::Value::StringRefType str( mark.GetName() );
					host.AddMember( str, dataset.GetData<int>( i ), alloc );
				}break;
			case JsonProperty::JDT_FLOAT:
				{
					rapidjson::Value::StringRefType str( mark.GetName() );
					host.AddMember( str, dataset.GetData<float>( i ), alloc );
				}break;
			case JsonProperty::JDT_OBJECT:
				{
					rapidjson::Value value(rapidjson::kObjectType);
					rapidjson::Value::StringRefType str( mark.GetName() );
					host.AddMember( str, value, alloc );
					const IJsonDataSet* pObject = static_cast<const IJsonDataSet*>( dataset.GetDataAddress( i ) );
					_DoWriteData( value, *pObject, alloc );
				}break;
			case JsonProperty::JDT_OBJECT_PTR:
				{
					rapidjson::Value value(rapidjson::kObjectType);
					rapidjson::Value::StringRefType str( mark.GetName() );
					host.AddMember( str, value, alloc );
					const IJsonDataSet* const* pObject = static_cast<const IJsonDataSet* const*>( dataset.GetDataAddress( i ) );
					_DoWriteData( value, **pObject, alloc );
				}break;
			case JsonProperty::JDT_ARRAY_STRING:
				{
					const JsonStringArray* pArray = static_cast<const JsonStringArray*>( dataset.GetDataAddress( i ) );
					rapidjson::Value valarr(rapidjson::kArrayType);
					rapidjson::Value::StringRefType str( mark.GetName() );
					host.AddMember( str, valarr, alloc );
					for ( int j = 0 ; j < pArray->size() ; ++ j )
					{
						rapidjson::Value::StringRefType val( (*pArray)[ j ].c_str() );
						valarr.PushBack( val, alloc );
					}
				}break;
			case JsonProperty::JDT_ARRAY_INT:
				{
					const JsonIntArray* pArray = static_cast<const JsonIntArray*>( dataset.GetDataAddress( i ) );
					rapidjson::Value valarr(rapidjson::kArrayType);
					rapidjson::Value::StringRefType str( mark.GetName() );
					host.AddMember( str, valarr, alloc );
					for ( int j = 0 ; j < pArray->size() ; ++ j )
					{
						valarr.PushBack( (*pArray)[j], alloc );
					}
				}break;
			case JsonProperty::JDT_ARRAY_FLOAT:
				{
					const JsonFloatArray* pArray = static_cast<const JsonFloatArray*>( dataset.GetDataAddress( i ) );
					rapidjson::Value valarr(rapidjson::kArrayType);
					rapidjson::Value::StringRefType str( mark.GetName() );
					host.AddMember( str, valarr, alloc );
					for ( int j = 0 ; j < pArray->size() ; ++ j )
					{
						valarr.PushBack( (*pArray)[j], alloc );
					}
				}break;
			case JsonProperty::JDT_ARRAY_OBJECT:
				{
					const IJsonArray* pArray = static_cast<const IJsonArray*>( dataset.GetDataAddress( i ) );
					rapidjson::Value valarr(rapidjson::kArrayType);
					rapidjson::Value::StringRefType str( mark.GetName() );
					for ( int j = 0 ; j < pArray->Getsize() ; ++ j )
					{
						rapidjson::Value value(rapidjson::kObjectType);
						const IJsonDataSet& object = pArray->At(j);
						_DoWriteData( value, object, alloc );
						valarr.PushBack( value, alloc );
					}
					host.AddMember( str, valarr, alloc );
				}break;
			default:ASSERT(false);
			}
		}
	}
	void JsonWriter::Write( const IJsonDataSet& dataset )
	{
		M_VALUE.SetObject();
		_DoWriteData( M_VALUE, dataset, M_JSON.GetAllocator() );
		if( M_JSON.HasMember( "Data" ) )
		{
			M_JSON["Data"] = M_VALUE;
		}
		else
		{
			M_JSON.AddMember( "Data", M_VALUE, M_JSON.GetAllocator() );
		}
		rapidjson::StringBuffer buffer;
		rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
		M_JSON.Accept(writer);
		m_pIFile->Seek( 0, SEEK_SET );
		m_pIFile->WriteFile( buffer.GetString(), buffer.GetSize() );
	}

}