#include "Predefine.h"
#include "DecoderManager.h"
#include "Decoder.h"
#include "SoundResource.h"
#include "LessThread.h"
#include "Utility.h"
#include "MPTDecoder.h"
#include "OGGDecoder.h"
#include "WAVDecoder.h"
#include "Log.h"


using namespace std;

namespace BohgeEngine
{

	DecoderManager* DecoderManager::m_pInstance;

	//-------------------------------------------------------------------------------------------------------
	void DecoderManager::Create()
	{
		m_pInstance = NEW DecoderManager();
	}
	//-------------------------------------------------------------------------------------------------------
	void DecoderManager::Destroy()
	{
		SAFE_DELETE( m_pInstance );
	}
	//-------------------------------------------------------------------------------------------------------
	DecoderManager::DecoderManager(void)
	{
		m_pDecodingLessThread = NEW LessThread();
		m_pDecodingLessThread->SetPriority( Threading::TP_HIGH );
		m_pDecodingLessThread->Start();
	}
	//-------------------------------------------------------------------------------------------------------
	DecoderManager::~DecoderManager(void)
	{
		m_pDecodingLessThread->Stop();
		SAFE_DELETE( m_pDecodingLessThread );
	}
	//-------------------------------------------------------------------------------------------------------
	Decoder* DecoderManager::_DecoderFactory( const std::string& path )
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
	void DecoderManager::_DestoryDecoder( Decoder* decoder )
	{
		SAFE_DELETE( decoder );
	}
	//-------------------------------------------------------------------------------------------------------
	void DecoderManager::Update()
	{
		for( DecoderTrashList::iterator it = m_DecoderTrashList.begin();
			it != m_DecoderTrashList.end(); )
		{
			if ( !(*it)->isDecoding() && !(*it)->isRequested() )
			{
				DecoderTrashList::iterator temp = it;
				it++;
				(*temp)->ReleaseDecoder();
				_DestoryDecoder( *temp );
				m_DecoderTrashList.erase( temp );
			}
			else
			{
				it++;
			}
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void DecoderManager::PushDecodeJob( Decoder* job )
	{
		m_pDecodingLessThread->PushJob( job );
	}
	//-------------------------------------------------------------------------------------------------------
	Decoder* DecoderManager::LoadSoundDecoder( const std::string& path )
	{
		Decoder* decoder;
		uint hash = Utility::HashCode( path );
		DecoderReferenceMap::iterator refDecoder = m_DecoderMap.find( hash );
		if ( m_DecoderMap.end() == refDecoder )//没找到解码器，创建新的解码器
		{
			decoder = _DecoderFactory( path );
			DecoderReference* dr = NEW DecoderReference( decoder );
			m_DecoderMap.insert( make_pair( hash, dr ) );
		}
		else
		{
			refDecoder->second->m_nReference ++;
			decoder = refDecoder->second->m_pDecoder;
		}
		return decoder;
	}
	//-------------------------------------------------------------------------------------------------------
	void DecoderManager::UnloadSoundDecoder( Decoder* sr )
	{
		uint hash = Utility::HashCode( sr->GetFilePath() );
		DecoderReferenceMap::iterator refDecoder = m_DecoderMap.find( hash );
		ASSERT( m_DecoderMap.end() != refDecoder );
		if ( 0 == -- refDecoder->second->m_nReference )
		{
			//Decoder::DestoryDecoder( refDecoder->second->m_pDecoder );//不能立即删除，可能在队列中
			refDecoder->second->m_pDecoder->Deactive();
			m_DecoderTrashList.push_back( refDecoder->second->m_pDecoder );//放入垃圾箱，等待回收
			SAFE_DELETE( refDecoder->second );
			m_DecoderMap.erase( refDecoder );
		}
	}
}
