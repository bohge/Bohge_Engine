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
#include "JsonReader.h"
#include "IIOSystem.h"
#include "IFile.h"
#include "Log.h"

#include <rapidjson/document.h>     // rapidjson's DOM-style API
#include <EASTL/string.h>



#define M_JSON (*static_cast<rapidjson::Document*>(m_pJsonNode))
using namespace eastl;
namespace BohgeEngine
{
	RTTI_TYPE_IMPLEMENT( JsonReader );
	//------------------------------------------------------------------------------
	JsonReader::JsonReader(void)
		:m_pJsonNode( NEW rapidjson::Document ),
		m_Version(0)
	{
	}
	//------------------------------------------------------------------------------
	JsonReader::~JsonReader(void)
	{
		SAFE_DELETE( m_pJsonNode );
	}
	//------------------------------------------------------------------------------
	bool JsonReader::Open( const eastl::string& path )
	{
		if ( IIOSystem::Instance()->isFileExist( path ) )
		{
			IFile* file = IIOSystem::Instance()->FileFactory( path );
			string data;
			data.resize( file->GetSize() );
			file->ReadFile( &data[0], file->GetSize() );
			file->CloseFile();
			IIOSystem::Instance()->RecycleBin( file );
			return Parse( data );
		}
		return false;
	}
	//------------------------------------------------------------------------------
	bool JsonReader::Parse( const eastl::string& doc )
	{
		//ASSERT( !M_JSON.Parse<0>(doc.c_str()).HasParseError() )
		M_JSON.Parse( doc.c_str() );
		m_Version = M_JSON[ "Version" ].GetUint();
		return true;
	}
	//------------------------------------------------------------------------------
	static void _DoReadData( const rapidjson::Value& data, IJsonDataSet& dataset )
	{
		for ( uint i = 0 ; i < dataset.GetDataCount() ; ++ i )
		{
			const JsonProperty::JsonDataMark& mark = dataset.GetJsonDataMark( i );
			//if ( data.HasMember(mark.GetName()) )
			{
				const rapidjson::Value& nodeValue = data[ mark.GetName() ];
				switch( mark.GetDataType() )
				{
				case JsonProperty::JDT_STRING:dataset.SetData( i, string( nodeValue.GetString() ) );break;
				case JsonProperty::JDT_BOOL:dataset.SetData( i, nodeValue.GetBool() );break;
				case JsonProperty::JDT_INT:dataset.SetData( i, nodeValue.GetInt() );break;
				case JsonProperty::JDT_FLOAT:dataset.SetData( i, static_cast<float>(nodeValue.GetDouble()) );break;
				case JsonProperty::JDT_OBJECT:
					{
						IJsonDataSet* pObject = static_cast<IJsonDataSet*>( dataset.GetDataAddress( i ) );
						_DoReadData( nodeValue, *pObject );
					}break;
				case JsonProperty::JDT_OBJECT_PTR:
					{
						IJsonDataSet** pObject = static_cast<IJsonDataSet**>( dataset.GetDataAddress( i ) );
						_DoReadData( nodeValue, **pObject );
					}break;
				case JsonProperty::JDT_ARRAY_STRING:
					{
						JsonStringArray* pArray = static_cast<JsonStringArray*>( dataset.GetDataAddress( i ) );
						pArray->resize( nodeValue.Size() );
						for ( int j = 0 ; j < nodeValue.Size() ; ++ j )
						{
							(*pArray)[ j ] = nodeValue[j].GetString();
						}
					}break;
				case JsonProperty::JDT_ARRAY_INT:
					{
						JsonIntArray* pArray = static_cast<JsonIntArray*>( dataset.GetDataAddress( i ) );
						pArray->resize( nodeValue.Size() );
						for ( int j = 0 ; j < nodeValue.Size() ; ++ j )
						{
							(*pArray)[ j ] = nodeValue[j].GetInt();
						}
					}break;
				case JsonProperty::JDT_ARRAY_FLOAT:
					{
						JsonFloatArray* pArray = static_cast<JsonFloatArray*>( dataset.GetDataAddress( i ) );
						pArray->resize( nodeValue.Size() );
						for ( int j = 0 ; j < nodeValue.Size() ; ++ j )
						{
							(*pArray)[ j ] = nodeValue[j].GetDouble();
						}
					}break;
				case JsonProperty::JDT_ARRAY_OBJECT:
					{
						IJsonArray* pArrayObject = static_cast<IJsonArray*>( dataset.GetDataAddress( i ) );
						pArrayObject->Resize( nodeValue.Size() );
						for ( int j = 0 ; j < nodeValue.Size(); ++ j )
						{
							const rapidjson::Value& arrayNodeValue = nodeValue[j];
							_DoReadData( arrayNodeValue, pArrayObject->At(j) );
						}
					}break;
				default:ASSERT(false);
				}
				dataset.OnDataLoadded( i );
			}
		}
	}
	void JsonReader::Read( IJsonDataSet& dataset )
	{
		const rapidjson::Value& data = M_JSON[ "Data" ];
		_DoReadData( data, dataset );
	}
}