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
		m_isStorageBuffer( false ),
		m_isDecodeing( false ),
		m_pFileStream( NULL ),
		m_eDecoderTypes( type ),
		m_pBuffer( NULL ),
		m_nFrequency( 0 ),
		m_nFormat( DF_MONO_8 ),
		m_nChannels( 0 ),
		m_nSourceTotalSize( 0 )
	{
		m_pBuffer = NEW char[ DecoderManager::DC_DEFUALT_SOUND_BUFFER_SIZE ];
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
			SAFE_DELETE_ARRAY( m_pBuffer );
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void Decoder::StorageBuffer()
	{
		if ( !m_isStorageBuffer )
		{
			m_isStorageBuffer = true;
			SAFE_DELETE_ARRAY( m_pBuffer );//删除旧的buffer
			m_pBuffer = NEW char[ m_nSourceTotalSize ];//分配一个完整的buffer
			memset( m_pBuffer, 0, m_nSourceTotalSize );
			_RequestDecode( 0, DecoderManager::DC_DEFUALT_SOUND_BUFFER_SIZE );
		}
	}
	//-------------------------------------------------------------------------------------------------------
	Decoder::BufferChunk Decoder::GetBufferChunk( int pos )
	{	
		uint buffersize = pos + DecoderManager::DC_DEFUALT_SOUND_BUFFER_SIZE > m_nSourceTotalSize ? m_nSourceTotalSize - pos : DecoderManager::DC_DEFUALT_SOUND_BUFFER_SIZE;
		uint next_pos = pos + DecoderManager::DC_DEFUALT_SOUND_BUFFER_SIZE > m_nSourceTotalSize ? 0 : pos + buffersize;
		if ( m_isStorageBuffer )
		{
			return BufferChunk( m_pBuffer + pos, buffersize, next_pos );
		}
		else if ( !m_isDecodeing )//解码中就不反回数据
		{
			return BufferChunk( m_pBuffer, buffersize, next_pos );
		}
		return BufferChunk(NULL,0,pos);
	}
	//-------------------------------------------------------------------------------------------------------
	void Decoder::RequestDecode( uint begin )
	{
		if ( !m_isStorageBuffer )
		{
			_RequestDecode( begin, _GetEndPosition( begin ) );
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void Decoder::_RequestDecode( uint form, uint to )
	{
		m_isDecodeing = true;
		m_FromTo = vector2d( form, to );
		//添加到解码线程队列，进行解码
		DecoderManager::Instance()->PushDecodeJob( this );
	}
	//-------------------------------------------------------------------------------------------------------
	void Decoder::AsyncDoJob()
	{
		_DoDecodeAsyn( m_FromTo.m_x, m_FromTo.m_y );
		m_isDecodeing = false;
	}

}