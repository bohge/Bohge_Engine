#include "OGGDecoder.h"



namespace BohgeEngine
{
	//-------------------------------------------------------------------------------------------------------
	OGGDecoder::OGGDecoder( IReadFile* stream )
		:SoundResourceDecoder( stream, SoundResourceDecoder::ST_OGG )
	{
	}
	//-------------------------------------------------------------------------------------------------------
	OGGDecoder::~OGGDecoder(void)
	{
	}

	void OGGDecoder::DoDecodeAsyn( uint form, uint to )
	{

	}

}