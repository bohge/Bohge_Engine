#pragma once
#include "Predefine.h"
#include "VariableType.h"



namespace BohgeEngine
{
	class IJob
	{
	public:
		enum JobType
		{
			JT_ASYNCHRONOUS = 0,
			JT_SYNCHRONOUS,
		};
	private:
		JobType		m_eAsynJob;
		uint		m_nPriority;//”≈œ»º∂
	public:
		IJob():m_nPriority(0),m_eAsynJob(JT_ASYNCHRONOUS){}
		virtual ~IJob(void){}
	public:
		virtual void DoJob() = 0;
	public:
		BOHGE_FORCEINLINE void SetJobType( JobType jt )
		{
			m_eAsynJob = jt;
		}
		BOHGE_FORCEINLINE JobType GetJobType()
		{
			return m_eAsynJob;
		}
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
