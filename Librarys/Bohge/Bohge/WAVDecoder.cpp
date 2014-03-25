#include "WAVDecoder.h"



namespace BohgeEngine
{
	//-------------------------------------------------------------------------------------------------------
	WAVDecoder::WAVDecoder(void)
		:Decoder( Decoder::ST_WAV )
	{
	}
	//-------------------------------------------------------------------------------------------------------
	WAVDecoder::~WAVDecoder(void)
	{
	}
	//-------------------------------------------------------------------------------------------------------
	uint WAVDecoder::_DoDecodeAsyn( uint form, uint to )
	{
		return 0;
	}
	//-------------------------------------------------------------------------------------------------------
	void WAVDecoder::_DoInitialization( int& freq, Format& format, int& ch, int& buffersize, double& time )
	{

	}
	//-------------------------------------------------------------------------------------------------------
	void WAVDecoder::_DoReleaseDecoder()
	{

	}

}