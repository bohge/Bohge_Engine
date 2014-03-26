#pragma once
#include "SoundManager.h"
#ifdef _OPENSL




struct SLObjectItf_;
typedef const struct SLObjectItf_ * const * SLObjectItf;
struct SLEngineItf_;
typedef const struct SLEngineItf_ * const * SLEngineItf;
struct SLEnvironmentalReverbItf_;
typedef const struct SLEnvironmentalReverbItf_ * const * SLEnvironmentalReverbItf;



namespace BohgeEngine
{
	class SoundManagerSL : public SoundManager
	{
	private:
		SLObjectItf						m_pEngineObject;
		SLEngineItf						m_pEngine;
		SLObjectItf						m_pMixObject;
		SLEnvironmentalReverbItf		m_pEnvironmental;
	public:
		SoundManagerSL(void);
		~SoundManagerSL(void);
	private:
		virtual void _OnCreate( );
		virtual void _OnDestroy( );
	public:
		virtual SoundPlayer* CreatePlayer( int hash, int index, Decoder* res );
	};
}
#endif