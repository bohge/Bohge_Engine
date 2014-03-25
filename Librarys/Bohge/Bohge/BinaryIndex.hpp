#pragma once
#include "Predefine.h"


namespace Utility
{
	class BinaryIndex//只能是0，1的索引
	{
	private:
		int		m_nIndex;
	public:
		BinaryIndex()
			:m_nIndex(0)
		{
		}
		~BinaryIndex()
		{
		}
	public:
		BOHGE_FORCEINLINE const BinaryIndex& operator = ( const BinaryIndex& rhs )
		{
			m_nIndex = rhs.m_nIndex;
			return *this;
		}
		BOHGE_FORCEINLINE const BinaryIndex& operator = ( int rhs )
		{
			m_nIndex = rhs;
			ASSERT( rhs >= 0 && rhs <= 1 );
			return *this;
		}
		BOHGE_FORCEINLINE int operator ! ()
		{
			return ~m_nIndex & 0x00000001;
		}
		BOHGE_FORCEINLINE operator int ()
		{
			return m_nIndex;
		}
	};
}