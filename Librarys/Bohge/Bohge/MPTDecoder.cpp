#include "MPTDecoder.h"



namespace BohgeEngine
{
	//-------------------------------------------------------------------------------------------------------
	MPTDecoder::MPTDecoder(void)
		:Decoder( Decoder::ST_MP3 )
	{
	}
	//-------------------------------------------------------------------------------------------------------
	MPTDecoder::~MPTDecoder(void)
	{
	}
	//-------------------------------------------------------------------------------------------------------
	uint MPTDecoder::_DoDecodeAsyn( uint form, uint to )
	{
		return 0;
	}
	//-------------------------------------------------------------------------------------------------------
	void MPTDecoder::_DoInitialization( int& freq, Format& format, int& ch, int& buffersize, double& time )
	{

	}
	//-------------------------------------------------------------------------------------------------------
	void MPTDecoder::_DoReleaseDecoder()
	{

	}

}
