#pragma once
#include "soundresourcedecoder.h"


namespace BohgeEngine
{
	class OGGDecoder : public SoundResourceDecoder
	{
	public:
		OGGDecoder( IReadFile* stream );
		~OGGDecoder(void);
	public:
		virtual void DoDecodeAsyn( uint form, uint to );//“Ï≤ΩΩ‚¬Î
	};
}
