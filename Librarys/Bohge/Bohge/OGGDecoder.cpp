#include "OGGDecoder.h"
#include "IFile.h"
#include "Log.h"



namespace BohgeEngine
{
	//-------------------------------------------------------------------------------------------------------
	OGGDecoder::OGGDecoder(void)
		:Decoder( Decoder::ST_OGG )
	{
	}
	//-------------------------------------------------------------------------------------------------------
	OGGDecoder::~OGGDecoder(void)
	{
	}
	//-------------------------------------------------------------------------------------------------------
	uint OGGDecoder::_DoDecodeAsyn( uint form, uint to )
	{
		//int endian = 0;
		//int bitStream;
		//ov_read(&m_OggFile, _GetInternalBuffer(), DecoderManager::SC_DEFUALT_SOUND_BUFFER_SIZE, endian, 2, 1, &bitStream); 
		DEBUGLOG( "ogg decoding from %d to %d\n", form, to );
		if ( 0 == form )
		{
			ov_time_seek( &m_OggFile, 0.0 );
		}
		long bytes;
		int endian = 0;
		int bitStream;
		int size = to - form;
		char* buffer = _GetInternalBuffer();
		int loaded = 0;
		do
		{
			bytes = ov_read(&m_OggFile, buffer, size - loaded, endian, 2, 1, &bitStream);  
			if ( bytes < 0 )
			{
				break;
			}
			if( size < loaded + bytes )
			{
				bytes = size - loaded;
			}
			loaded += bytes;
			buffer += bytes;
		}  
		while ( loaded < size );
		return loaded;
	}
	//-------------------------------------------------------------------------------------------------------
	size_t ReadHelp(void *ptr, size_t size, size_t nmemb, void *datasource)
	{
		IReadFile* reader = static_cast<IReadFile*>( datasource );
		reader->ReadFile( ptr, size * nmemb );
		return nmemb;
	}
	//-----------------------------------------------------------------------------------
	int SeekHelp(void *datasource, ogg_int64_t offset, int whence)
	{
		IReadFile* reader = static_cast<IReadFile*>( datasource );
		return reader->Seek( offset, whence );
	}
	//-------------------------------------------------------------------------------------------------------
	long TellHelp(void *datasource)
	{
		IReadFile* reader = static_cast<IReadFile*>( datasource );
		return reader->Tell();
	}
	//-------------------------------------------------------------------------------------------------------
	void OGGDecoder::_DoInitialization( int& freq, Format& format, int& ch, int& buffersize, double& time )
	{
		vorbis_info *pInfo;
		ov_callbacks callbacks;
		callbacks.read_func = &ReadHelp;
		callbacks.close_func = NULL;
		callbacks.tell_func = &TellHelp;
		callbacks.seek_func = &SeekHelp;
		int res = ov_open_callbacks( _GetFileSteam(), &m_OggFile, NULL, 0, callbacks );
		pInfo = ov_info(&m_OggFile, -1);
		time = ov_time_total( &m_OggFile, -1 );
		ch = pInfo->channels;
		freq = pInfo->rate;
		//buffersize = ov_pcm_total( &m_OggFile, -1 ) * ch * pInfo->channels;
		//sample rate * channels * bitrate/8 * length.
		buffersize = time * freq * ch * 2;
		if (pInfo->channels == 1)
		{
			format = DF_MONO_16;
		}
		else
		{
			format = DF_STEREO_16;
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void OGGDecoder::_DoReleaseDecoder()
	{
		ov_clear(&m_OggFile);
	}

}