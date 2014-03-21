#include "Threading.h"



namespace BohgeEngine
{
	//------------------------------------------------------------------------------------------------------
	Threading::Threading(void)
		:m_isRunning(false),
		m_ePriority(TP_NORMAL)
	{
	}
	//------------------------------------------------------------------------------------------------------
	Threading::~Threading(void)
	{
	}
	//------------------------------------------------------------------------------------------------------
	void* Threading::ThreadProc( void* param )
	{
		Threading* ptr = static_cast<Threading*>( param );
		return ptr->DoWork();
	}
	//------------------------------------------------------------------------------------------------------
	void Threading::Start()
	{
		ASSERT( false == m_isRunning );
		m_isRunning = true;
		_OnBeforeStart();
		pthread_create( &m_pThreadID, NULL, ThreadProc, (void*)this );
		_OnAfterStart();
	}
	//------------------------------------------------------------------------------------------------------
	void Threading::Stop()
	{
		ASSERT( true == m_isRunning );
		if( !_StopWork( ) )
		{
			void* ptr = 0;
			pthread_join( m_pThreadID, &ptr );
			pthread_exit( ptr );
		}
		m_isRunning = false;
	}
	//------------------------------------------------------------------------------------------------------
	void Threading::SetPriority( ThreadPriority tp )
	{
		m_ePriority = tp;
	}
}