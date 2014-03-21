#pragma once
#include "soundresourcedecoder.h"


namespace BohgeEngine
{
	class MPTDecoder : public SoundResourceDecoder
	{
	public:
		MPTDecoder( IReadFile* stream );
		~MPTDecoder(void);
	public:
		virtual void DoDecodeAsyn( uint form, uint to );//“Ï≤ΩΩ‚¬Î
	};
}
