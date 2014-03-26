#pragma once
#include "SoundManager.h"

#ifdef _OPENAL


typedef struct ALCdevice_struct ALCdevice;
typedef struct ALCcontext_struct ALCcontext;
namespace BohgeEngine
{
	class SoundManagerAL : public SoundManager
	{
	private:
		ALCdevice*		m_pDevice;//设备
		ALCcontext*		m_pContext;//上下文
	public:
		SoundManagerAL(void);
		~SoundManagerAL(void);
	private:
		virtual void _OnCreate( );
		virtual void _OnDestroy( );
	public:
		virtual SoundPlayer* CreatePlayer( int hash, int index, Decoder* res );
	};
}
#endif