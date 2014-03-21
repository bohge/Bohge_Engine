#pragma once
#include "soundresourcedecoder.h"



namespace BohgeEngine
{
	class WAVDecoder : public SoundResourceDecoder
	{
	public:
		WAVDecoder( IReadFile* stream );
		~WAVDecoder(void);
	public:
		virtual void DoDecodeAsyn( uint form, uint to );//“Ï≤ΩΩ‚¬Î
	};
}
