#include "MPTDecoder.h"



namespace BohgeEngine
{
	//-------------------------------------------------------------------------------------------------------
	MPTDecoder::MPTDecoder( IReadFile* stream )
		:SoundResourceDecoder( stream, SoundResourceDecoder::ST_MP3 )
	{
	}
	//-------------------------------------------------------------------------------------------------------
	MPTDecoder::~MPTDecoder(void)
	{
	}
	//-------------------------------------------------------------------------------------------------------
	void MPTDecoder::DoDecodeAsyn( uint form, uint to )
	{

	}

}
