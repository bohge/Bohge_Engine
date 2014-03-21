#pragma once
#include "SoundManager.h"

namespace BohgeEngine
{
	class SoundManagerAL : public SoundManager
	{
	public:
		SoundManagerAL(void);
		~SoundManagerAL(void);
	private:
		virtual void _OnCreate( );
		virtual void _OnDestroy( );
	};
}
