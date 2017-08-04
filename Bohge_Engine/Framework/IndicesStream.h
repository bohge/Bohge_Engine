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
#include "BufferObjectProperty.h"
#include "VariableType.h"
#include "Predefine.h"
#include "Object.hpp"



#include <EASTL/vector.h>

namespace BohgeEngine
{
	class BOHGE_FRAMEWORK IndicesStream : public Object
	{
		RTTI_TYPE_DECLEAR( IndicesStream );
	private:
		enum Constant
		{
			IC_RESERVE_BUFFER_SIZE = 512,
		};
	private:
		eastl::vector<byte>					m_DataVector;
		BufferObjectProperty::IndicesType	m_eType;
		uint								m_ReserveIndicesCount;
		uint								m_IndicesCount;
		uint								m_CopyBytes;
	public:
		IndicesStream(void);
		~IndicesStream(void);
	public:
		void SetIndicesType( BufferObjectProperty::IndicesType type );
		void ReserveBuffer( uint size );
	public:
		BOHGE_FORCEINLINE void PushIndicesData( int data )
		{
			if ( m_IndicesCount >= m_ReserveIndicesCount )
			{
				ReserveBuffer( m_ReserveIndicesCount + IC_RESERVE_BUFFER_SIZE );
			}
			PushIndicesDataFast( data );
		}
		BOHGE_FORCEINLINE void PushIndicesDataFast( int data )
		{
			memcpy( m_DataVector.data() + m_IndicesCount * m_CopyBytes, &data, m_CopyBytes );
			++ m_IndicesCount;
		}
		BOHGE_FORCEINLINE BufferObjectProperty::IndicesType GetIndicesType() const
		{
			return m_eType;
		}
		BOHGE_FORCEINLINE const void* GetBuffer() const
		{
			return m_DataVector.data();
		}
		BOHGE_FORCEINLINE uint GetSize() const
		{
			return m_IndicesCount;
		}
		BOHGE_FORCEINLINE uint GetByteSize() const
		{
			return m_CopyBytes;
		}
		BOHGE_FORCEINLINE void ReleaseBuffer()
		{
			m_eType = BufferObjectProperty::IT_UINT16;
			m_CopyBytes = 0;
			m_IndicesCount = 0;
			m_ReserveIndicesCount = 0;
			m_DataVector.clear();
		}
		BOHGE_FORCEINLINE const eastl::vector<byte>& GetBufferArray() const
		{
			return m_DataVector;
		}
	};
}
