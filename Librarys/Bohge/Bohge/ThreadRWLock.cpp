#include "ThreadRWLock.h"


namespace BohgeEngine
{
	//------------------------------------------------------------------------------------------------------
	ThreadRWLock::ThreadRWLock(void)
	{
		pthread_rwlock_init( &m_pRWLock, NULL );
	}
	//------------------------------------------------------------------------------------------------------
	ThreadRWLock::~ThreadRWLock(void)
	{
		pthread_rwlock_destroy( &m_pRWLock );
	}
}