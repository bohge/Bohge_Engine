#pragma once
#include "Predefine.h"



namespace BohgeEngine
{
	class IAsynJob
	{
	private:
		unsigned int		m_nPriority;//”≈œ»º∂
	public:
		IAsynJob():m_nPriority(0){}
		virtual ~IAsynJob(void){}
	public:
		virtual void AsyncDoJob() = 0;
	public:
		BOHGE_FORCEINLINE void SetPriority( unsigned int p )
		{
			m_nPriority = p;
		}
		BOHGE_FORCEINLINE unsigned int GetPriority() const
		{
			return m_nPriority;
		}
	};
}
