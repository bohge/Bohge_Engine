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