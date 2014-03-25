#include "Decoder.h"
#include "DecoderManager.h"
#include "MPTDecoder.h"
#include "OGGDecoder.h"
#include "WAVDecoder.h"
#include "Log.h"
#include "UsualFile.h"

#include <string>


using namespace std;

namespace BohgeEngine
{
	//-------------------------------------------------------------------------------------------------------
	Decoder* Decoder::DecoderFactory( const std::string& path )
	{
		//确定声音类型
		Decoder* decoder = NULL;
		if ( string::npos != path.find( ".mp3" )
			|| string::npos != path.find( ".MP3" ) )
		{
			decoder = NEW MPTDecoder();
		}
		else if ( string::npos != path.find( ".ogg" )
			|| string::npos != path.find( ".OGG" ) )
		{
			decoder = NEW OGGDecoder();
		}
		else if ( string::npos != path.find( ".wav" )
			|| string::npos != path.find( ".WAV" ) )
		{
			decoder = NEW WAVDecoder();
		}
		else
		{
			DEBUGLOG( "Unkonw sound type %s\n", path.c_str() );
		}
		decoder->LoadResource( path );
		return decoder;
	}
	//-------------------------------------------------------------------------------------------------------
	void Decoder::DestoryDecoder( Decoder* decoder )
	{
		SAFE_DELETE( decoder );
	}
	//-------------------------------------------------------------------------------------------------------
	Decoder::Decoder( SoundType type )
		:m_isActived( true ),
		m_pFileStream( NULL ),
		m_eDecoderTypes( type ),
		m_nFrequency( 0 ),
		m_nFormat( DF_MONO_8 ),
		m_nChannels( 0 ),
		m_nSourceTotalSize( 0 ),
		m_nLoadedBufferIndex( 0 )
	{
	}
	//-------------------------------------------------------------------------------------------------------
	Decoder::~Decoder(void)
	{
		ReleaseDecoder();
	}
	//-------------------------------------------------------------------------------------------------------
	void Decoder::LoadResource( const std::string& path )
	{
		m_FilePath = path;
		m_pFileStream = NEW ReadUsualFile( path );
		m_pFileStream->OpenFile();
		_DoInitialization(m_nFrequency, m_nFormat, m_nChannels, m_nSourceTotalSize, m_dTime);
		m_nSourceTotalSize -= ( m_nSourceTotalSize % (m_nChannels*2) );//确保整数倍
		int vecSize = Math::Ceil( static_cast<float>(m_nSourceTotalSize) / DecoderManager::DC_DEFUALT_SOUND_BUFFER_SIZE );
		m_BufferVector.resize( vecSize );
		for ( int i = 0 ; i < vecSize ; i ++ )
		{
			m_BufferVector[i] = NEW char[DecoderManager::DC_DEFUALT_SOUND_BUFFER_SIZE];
			memset( m_BufferVector[i], 0, DecoderManager::DC_DEFUALT_SOUND_BUFFER_SIZE );
		}
		_RequestDecode();
		//sample rate * channels * bitrate/8 * length.
		//switch( m_nChannels )
		//{
		//case 1:
		//	{
		//		m_nSourceTotalSize = m_dTime * m_nFrequency * 2;
		//		m_nSourceTotalSize -= (m_nSourceTotalSize % 2);
		//	}break;
		//case 2:
		//	{
		//		m_nSourceTotalSize = m_dTime * m_nFrequency * 4;
		//		m_nSourceTotalSize -= (m_nSourceTotalSize % 4);
		//	}break;
		//case 4:
		//	{
		//		m_nSourceTotalSize = m_dTime * m_nFrequency * 8;
		//		m_nSourceTotalSize -= (m_nSourceTotalSize % 8);
		//	}break;
		//case 6:
		//	{
		//		m_nSourceTotalSize = m_dTime * m_nFrequency * 12;
		//		m_nSourceTotalSize -= (m_nSourceTotalSize % 12);
		//	}break;
		//default: ASSERT(false);
		//}
	}
	//-------------------------------------------------------------------------------------------------------
	void Decoder::ReleaseDecoder()
	{
		if ( m_isActived )
		{
			_DoReleaseDecoder();
			m_pFileStream->CloseFile();
			m_isActived = false;
			SAFE_DELETE( m_pFileStream );
			for ( int i = 0 ; i < m_BufferVector.size() ; i ++ )
			{
				SAFE_DELETE_ARRAY( m_BufferVector[i] );
			}
			m_BufferVector.clear();
		}
	}
	//-------------------------------------------------------------------------------------------------------
	Decoder::BufferChunk Decoder::GetBufferChunk( int index )
	{	
		int pos = index * DecoderManager::DC_DEFUALT_SOUND_BUFFER_SIZE;
		bool isdone = pos + DecoderManager::DC_DEFUALT_SOUND_BUFFER_SIZE >= m_nSourceTotalSize;
		uint buffersize = isdone ? m_nSourceTotalSize - pos : DecoderManager::DC_DEFUALT_SOUND_BUFFER_SIZE;
		uint next_index = isdone ? 0 : index + 1;
		return BufferChunk( m_BufferVector[index], buffersize, next_index, isdone );
	}
	//-------------------------------------------------------------------------------------------------------
	void Decoder::_RequestDecode()
	{
		//添加到解码线程队列，进行解码
		SetPriority( m_nLoadedBufferIndex );//优先级就是读到第几个buffer
		DecoderManager::Instance()->PushDecodeJob( this );
	}
	//-------------------------------------------------------------------------------------------------------
	void Decoder::AsyncDoJob()
	{
		uint from = m_nLoadedBufferIndex * DecoderManager::DC_DEFUALT_SOUND_BUFFER_SIZE;
		uint to = _GetEndPosition(from);
		_DoDecodeAsyn( from, to );
		m_nLoadedBufferIndex ++;
		if ( m_nLoadedBufferIndex < m_BufferVector.size() )
		{
			_RequestDecode();
		}
	}

}