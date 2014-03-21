#pragma once
#include "Predefine.h"


#include <pthread.h>

namespace BohgeEngine
{
	class ThreadCondition
	{
	private:
		pthread_cond_t	m_pCondition;
		pthread_mutex_t	m_pMutex;
	public:
		ThreadCondition(void);
		~ThreadCondition(void);
	public:
		//------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE void Wait()
		{
			pthread_cond_wait( &m_pCondition, &m_pMutex );
		}
		//------------------------------------------------------------------------------------------------------
		BOHGE_FORCEINLINE void Signal()
		{
			pthread_cond_signal( &m_pCondition );
		}
	};
}
