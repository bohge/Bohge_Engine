#include "SoundResourceDecoder.h"
#include "SoundResourceManager.h"
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
	SoundResourceDecoder* SoundResourceDecoder::SoundResourceDecoderFactory( const std::string& path )
	{
		//确定声音类型
		ReadUsualFile* filestream = NEW ReadUsualFile( path );
		if ( string::npos != path.find( ".mp3" )
			|| string::npos != path.find( ".MP3" ) )
		{
			return NEW MPTDecoder( filestream );
		}
		else if ( string::npos != path.find( ".ogg" )
			|| string::npos != path.find( ".OGG" ) )
		{
			return NEW OGGDecoder( filestream );
		}
		else if ( string::npos != path.find( ".wav" )
			|| string::npos != path.find( ".WAV" ) )
		{
			return NEW WAVDecoder( filestream );
		}
		else
		{
			DEBUGLOG( "Unkonw sound type %s\n", path.c_str() );
		}
		return NULL;
	}
	//-------------------------------------------------------------------------------------------------------
	SoundResourceDecoder::SoundResourceDecoder( IReadFile* stream, SoundType type )
		:m_isActived( true ),
		m_isStorageBuffer( false ),
		m_isDecodeing( false ),
		m_pFileStream( stream ),
		m_eDecoderTypes( type ),
		m_pBuffer( NULL ),
		m_nFrequency( 0 ),
		m_nFormat( 0 ),
		m_nChannels( 0 ),
		m_nBufferSize( 0 )
	{
		m_pBuffer = NEW byte[ SoundResourceManager::SC_DEFUALT_SOUND_BUFFER_SIZE ];
	}
	//-------------------------------------------------------------------------------------------------------
	SoundResourceDecoder::~SoundResourceDecoder(void)
	{
	}
	//-------------------------------------------------------------------------------------------------------
	void SoundResourceDecoder::ReleaseDecoder()
	{
		m_isActived = false;
		SAFE_DELETE( m_pBuffer );
		SAFE_DELETE( m_pFileStream );
		SAFE_DELETE_ARRAY( m_pBuffer );
	}
	//-------------------------------------------------------------------------------------------------------
	void SoundResourceDecoder::StorageBuffer()
	{
		if ( !m_isStorageBuffer )
		{
			m_isStorageBuffer = true;
			SAFE_DELETE_ARRAY( m_pBuffer );//删除旧的buffer
			m_pBuffer = NEW byte[ m_nBufferSize ];//分配一个完整的buffer
			memset( m_pBuffer, 0, m_nBufferSize );
			_RequestDecode( 0, SoundResourceManager::SC_DEFUALT_SOUND_BUFFER_SIZE );
		}
	}
	//-------------------------------------------------------------------------------------------------------
	SoundResourceDecoder::BufferChunk SoundResourceDecoder::GetBufferChunk( int pos )
	{	
		int buffersize = pos + SoundResourceManager::SC_DEFUALT_SOUND_BUFFER_SIZE > m_nBufferSize ? m_nBufferSize - pos : SoundResourceManager::SC_DEFUALT_SOUND_BUFFER_SIZE;
		if ( m_isStorageBuffer )
		{
			return BufferChunk( m_pBuffer + pos, buffersize );
		}
		else if ( !m_isDecodeing )//解码中就不反回数据
		{
			_RequestDecode( pos, _GetEndPosition( pos ) );
			return BufferChunk( m_pBuffer, buffersize );
		}
		return BufferChunk(NULL,0);
	}
	//-------------------------------------------------------------------------------------------------------
	void SoundResourceDecoder::_RequestDecode( uint form, uint to )
	{
		m_isDecodeing = true;
		//添加到解码线程队列，进行解码
	}
	//-------------------------------------------------------------------------------------------------------
	void SoundResourceDecoder::OnEndDecode( uint form, uint to )
	{
		m_isDecodeing = false;
		if ( m_isStorageBuffer && to != m_nBufferSize )
		{
			_RequestDecode( to, _GetEndPosition( to ) );
		}
	}
}