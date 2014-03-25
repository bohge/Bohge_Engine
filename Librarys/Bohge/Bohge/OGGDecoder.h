#pragma once
#include "Decoder.h"


#include <vorbis/vorbisfile.h> 

namespace BohgeEngine
{
	class OGGDecoder : public Decoder
	{
	private:
		OggVorbis_File		m_OggFile;
	public:
		OGGDecoder(void);
		~OGGDecoder(void);
	private:
		virtual void _DoDecodeAsyn( uint form, uint to );//异步解码
		virtual void _DoInitialization( int& freq, Format& format, int& ch, int& buffersize, double& time );
		virtual void _DoReleaseDecoder();//释放资源
	};
}
