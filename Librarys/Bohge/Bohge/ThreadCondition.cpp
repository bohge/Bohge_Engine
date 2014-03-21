#include "ThreadCondition.h"


namespace BohgeEngine
{
	//------------------------------------------------------------------------------------------------------
	ThreadCondition::ThreadCondition(void)
	{
		pthread_cond_init( &m_pCondition, NULL );
		pthread_mutex_init( &m_pMutex, NULL );
	}
	//------------------------------------------------------------------------------------------------------
	ThreadCondition::~ThreadCondition(void)
	{
		pthread_cond_destroy( &m_pCondition );
		pthread_mutex_destroy( &m_pMutex );
	}
}