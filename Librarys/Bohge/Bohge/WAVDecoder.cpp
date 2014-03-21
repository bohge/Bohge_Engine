#include "WAVDecoder.h"



namespace BohgeEngine
{
	//-------------------------------------------------------------------------------------------------------
	WAVDecoder::WAVDecoder( IReadFile* stream )
		:SoundResourceDecoder( stream, SoundResourceDecoder::ST_WAV )
	{
	}
	//-------------------------------------------------------------------------------------------------------
	WAVDecoder::~WAVDecoder(void)
	{
	}
	//-------------------------------------------------------------------------------------------------------
	void WAVDecoder::DoDecodeAsyn( uint form, uint to )
	{

	}

}