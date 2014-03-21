#include "ThreadMutex.h"


namespace BohgeEngine
{
	//------------------------------------------------------------------------------------------------------
	ThreadMutex::ThreadMutex(void)
	{
		pthread_mutex_init( &m_pMutex, NULL );
	}
	//------------------------------------------------------------------------------------------------------
	ThreadMutex::~ThreadMutex(void)
	{
		pthread_mutex_destroy( &m_pMutex );
	}
}
