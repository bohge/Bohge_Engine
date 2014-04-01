//////////////////////////////////////////////////////////////////////////////////////
//
//						The Bohge Engine License (BEL)
//
//	Copyright (c) 2011-2014 Peng Zhao
//
//	Permission is hereby granted, free of charge, to any person obtaining a copy
//	of this software and associated documentation files (the "Software"), to deal
//	in the Software without restriction, including without limitation the rights
//	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//	copies of the Software, and to permit persons to whom the Software is
//	furnished to do so, subject to the following conditions:
//
//	The above copyright notice and this permission notice shall be included in 
//	all copies or substantial portions of the Software. And the logo of 
//	Bohge Engine shall be displayed full screen for more than 3 seconds 
//	when the software is started. Copyright holders are allowed to develop 
//	game edit based on Bohge Engine, The edit must be released under the MIT 
//	open source license if it is going to be published. In no event shall 
//	copyright holders be prohibited from using any code of Bohge Engine 
//	to develop any other analogous game engines.
//
//	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//
//////////////////////////////////////////////////////////////////////////////////////

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
		IFile* reader = static_cast<IFile*>( datasource );
		reader->ReadFile( ptr, size * nmemb );
		return nmemb;
	}
	//-----------------------------------------------------------------------------------
	int SeekHelp(void *datasource, ogg_int64_t offset, int whence)
	{
		IFile* reader = static_cast<IFile*>( datasource );
		return reader->Seek( offset, whence );
	}
	//-------------------------------------------------------------------------------------------------------
	long TellHelp(void *datasource)
	{
		IFile* reader = static_cast<IFile*>( datasource );
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