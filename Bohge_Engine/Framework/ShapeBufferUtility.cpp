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
#include "ShapeBufferUtility.h"


//0-------2
//|	    / |
//|	  /   |
//| /	  |
//1-------3
namespace BohgeEngine
{
	RTTI_TYPE_IMPLEMENT( QuadIndicesMetadata );
	//---------------------------------------------------------------------------------------------------------
	IndicesStream* QuadIndicesMetadata::_GetStream()
	{
		m_Stream.SetIndicesType( BufferObjectProperty::IT_UINT16 );
		m_Stream.ReserveBuffer( 6 );
		m_Stream.PushIndicesDataFast( 0 );
		m_Stream.PushIndicesDataFast( 1 );
		m_Stream.PushIndicesDataFast( 2 );

		m_Stream.PushIndicesDataFast( 2 );
		m_Stream.PushIndicesDataFast( 1 );
		m_Stream.PushIndicesDataFast( 3 );
		return &m_Stream;
	}
	//---------------------------------------------------------------------------------------------------------
	void QuadIndicesMetadata::GetIdentifier( eastl::vector<byte>& bytes ) const/*在str中推入表示 ?*/
	{
		eastl::string name = this->GetTypeName();
		bytes.insert( bytes.end(), name.begin(), name.end() );
	}





	RTTI_TYPE_IMPLEMENT( QuadVertexMetadata );
	//---------------------------------------------------------------------------------------------------------
	VertexStream* QuadVertexMetadata::_GetStream()
	{
		m_Stream.SetVertexType<vector4f>( Attributes::ATTRIBUTE_POSITION, BufferObjectProperty::DT_FLOAT, BufferObjectProperty::DT_FLOAT );
		m_Stream.ReserveBuffer( 4 );

		m_Stream.PushVertexDataFast( Attributes::ATTRIBUTE_POSITION, vector4f(-1,1,0,1)  );
		m_Stream.PushVertexDataFast( Attributes::ATTRIBUTE_POSITION, vector4f(-1,-1,0,1)  );
		m_Stream.PushVertexDataFast( Attributes::ATTRIBUTE_POSITION, vector4f(1,1,0,1)  );
		m_Stream.PushVertexDataFast( Attributes::ATTRIBUTE_POSITION, vector4f(1,-1,0,1)  );
		return &m_Stream;
	}
	//---------------------------------------------------------------------------------------------------------
	void QuadVertexMetadata::GetIdentifier( eastl::vector<byte>& bytes ) const
	{
		eastl::string name = this->GetTypeName();
		bytes.insert( bytes.end(), name.begin(), name.end() );
	}

}