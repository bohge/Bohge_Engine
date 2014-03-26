#include "WAVDecoder.h"
#include "IFile.h"
#include "Log.h"


namespace BohgeEngine
{
	struct WAVHead
	{
		char	riff[4];
		int		ChunkDataSize;
		char	wave[4];
		char	fmt[4];
		int		size;
		short	format;
		short	chanel;
		int		ferq;
		int		byteRate;
		short	offset;
		short	bitsPerSample;
	};
	struct WAVData
	{
		char	flag[4];
		int		size;
	};
	//-------------------------------------------------------------------------------------------------------
	WAVDecoder::WAVDecoder(void)
		:Decoder( Decoder::ST_WAV ),
		m_DataPosition(0)
	{
	}
	//-------------------------------------------------------------------------------------------------------
	WAVDecoder::~WAVDecoder(void)
	{
	}
	//-------------------------------------------------------------------------------------------------------
	uint WAVDecoder::_DoDecodeAsyn( uint form, uint to )
	{
		uint readsize = to - form;
		char* buffer = _GetInternalBuffer();
		DEBUGLOG( "wav decoding from %d to %d\n", form, to );
		if ( 0 == form )
		{
			_GetFileSteam()->Seek( m_DataPosition, SEEK_SET );
		}
		return _GetFileSteam()->ReadFile( buffer, readsize );
	}
	//-------------------------------------------------------------------------------------------------------
	void WAVDecoder::_DoInitialization( int& freq, Format& format, int& ch, int& buffersize, double& time )
	{	
		WAVHead head;
		WAVData datainfo;
		_GetFileSteam()->ReadFile( &head, sizeof(WAVHead) );
		if( 16 !=  head.size ) //需要跳帧
		{
			short extra = 0;
			_GetFileSteam()->ReadFile(&extra, sizeof(short) );
			if( 0 < extra )
			{
				byte* skip = NEW byte[extra];
				_GetFileSteam()->ReadFile(skip, extra );
				SAFE_DELETE_ARRAY(skip);
			}
			byte fmt;
			bool isContinue = true;
			do 
			{
				_GetFileSteam()->ReadFile(&fmt, sizeof(fmt) );
				if( fmt == 'd' )
				{
					_GetFileSteam()->ReadFile(&fmt, sizeof(fmt) );
					if( fmt == 'a' )
					{
						_GetFileSteam()->ReadFile(&fmt, sizeof(fmt) );
						if( fmt == 't' )
						{
							_GetFileSteam()->ReadFile(&fmt, sizeof(fmt) );
							if( fmt == 'a' )
							{
								isContinue = false;
							}
						}
					}
				}
			} while (isContinue);
			datainfo.flag[0] = 'd';
			datainfo.flag[1] = 'a';
			datainfo.flag[2] = 't';
			datainfo.flag[3] = 'a';
			_GetFileSteam()->ReadFile(&datainfo.size, sizeof(datainfo.size) );
		}
		else
		{
			_GetFileSteam()->ReadFile(&datainfo, sizeof(WAVData) );
			std::string dataflag;
			dataflag.resize(4);
			dataflag[0] = datainfo.flag[0];
			dataflag[1] = datainfo.flag[1];
			dataflag[2] = datainfo.flag[2];
			dataflag[3] = datainfo.flag[3];
			if ( dataflag != "data" )
			{
				ASSERT(false);//未知的文件类型
			}
		}
		if( 8 == head.bitsPerSample )
		{
			if (head.chanel == 1)
				format = DF_MONO_8;
			else
				format = DF_STEREO_8;
		}
		else
		{
			if (head.chanel == 1)
				format = DF_MONO_16;
			else
				format = DF_STEREO_16;
		}
		ch = head.chanel;
		freq = head.ferq;
		buffersize = datainfo.size;
		//sample rate * channels * bitrate/8 * length.
		//=>time = size / (sample rate * channels * bitrate/8)
		time = static_cast<float>(buffersize) /  ( freq * ch * head.bitsPerSample / 8 );
		m_DataPosition = _GetFileSteam()->Tell();
	}
	//-------------------------------------------------------------------------------------------------------
	void WAVDecoder::_DoReleaseDecoder()
	{

	}

}