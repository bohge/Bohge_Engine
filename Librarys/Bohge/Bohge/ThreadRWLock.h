#pragma once
#include "Predefine.h"

#include <pthread.h>

namespace BohgeEngine
{
	class ThreadRWLock
	{
	private:
		pthread_rwlock_t	m_pRWLock;
	public:
		ThreadRWLock(void);
		~ThreadRWLock(void);
	public:
		//------------------------------------------------------------------------------------------------------
		inline void ReadLock()//共享方式的读取锁
		{
			pthread_rwlock_rdlock( &m_pRWLock );
		}
		//------------------------------------------------------------------------------------------------------
		inline void WriteLock()//独占的写入锁
		{
			pthread_rwlock_wrlock( &m_pRWLock );
		}
		//------------------------------------------------------------------------------------------------------
		inline void Unlock()//解锁
		{
			pthread_rwlock_unlock( &m_pRWLock );
		}
	};
}
