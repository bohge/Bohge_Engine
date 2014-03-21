#pragma once
#include "Predefine.h"

#include <pthread.h>

namespace BohgeEngine
{
	class ThreadMutex
	{
	private:
		pthread_mutex_t		m_pMutex;
	public:
		ThreadMutex(void);
		~ThreadMutex(void);
	public:
		//------------------------------------------------------------------------------------------------------
		inline void Lock()//¼ÓËø
		{
			pthread_mutex_lock( &m_pMutex );
		}
		//------------------------------------------------------------------------------------------------------
		inline void Unlock()//½âËø
		{
			pthread_mutex_unlock( &m_pMutex );
		}
	};
}
