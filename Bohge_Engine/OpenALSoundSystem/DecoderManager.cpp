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
#include "Framework/IThreadSystem.h"
#include "Framework/Utility.hpp"
#include "Framework/Framework.h"
#include "Framework/Log.h"
#include "DecoderManager.h"
#include "SoundResource.h"
#include "MPTDecoder.h"
#include "OGGDecoder.h"
#include "WAVDecoder.h"
#include "Decoder.h"



using namespace eastl;
using namespace BohgeEngine;

namespace OpenALSoundSystemPlugin
{

	DecoderManager* DecoderManager::m_pInstance;

	//-------------------------------------------------------------------------------------------------------
	void DecoderManager::Create( Framework& framework )
	{
		m_pInstance = NEW DecoderManager();
		m_pInstance->m_DecodJobThread =
			static_cast<IJobDispatcher*>(IThreadSystem::Instance()->ThreadEntityFactory( ThreadProperty::ET_JOB_DISPATCHER ));
		m_pInstance->m_DecodJobThread->SetThreadCount( 1 );
		m_pInstance->m_DecodJobThread->SetSortType( IJobDispatcher::ST_LESS );
		m_pInstance->m_DecodJobThread->Start();
	}
	//-------------------------------------------------------------------------------------------------------
	void DecoderManager::Destroy( Framework& framework )
	{
		m_pInstance->m_DecodJobThread->Stop();
		IThreadSystem::Instance()->RecycleBin( m_pInstance->m_DecodJobThread );
		SAFE_DELETE( m_pInstance );
	}
	//-------------------------------------------------------------------------------------------------------
	DecoderManager::DecoderManager(void)
		:m_DecodJobThread(NULL)
	{
	}
	//-------------------------------------------------------------------------------------------------------
	DecoderManager::~DecoderManager(void)
	{
	}
	//-------------------------------------------------------------------------------------------------------
	SmartPtr<Decoder> DecoderManager::_DecoderFactory( const eastl::string& path )
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
		return SmartPtr<Decoder>(decoder);
	}
	//-------------------------------------------------------------------------------------------------------
	void DecoderManager::Update()
	{
	}
	//-------------------------------------------------------------------------------------------------------
	void DecoderManager::PushDecodeJob( SmartPtr<Decoder>& job )
	{
		if ( job->PrepareDecode() )
		{
			m_DecodJobThread->PushJob( job );
		}
	}
	//-------------------------------------------------------------------------------------------------------
	SmartPtr<Decoder> DecoderManager::LoadSoundDecoder( const eastl::string& path )
	{
		SmartPtr<Decoder> decoder;
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
	void DecoderManager::UnloadSoundDecoder( SmartPtr<Decoder>& sr )
	{
		uint hash = Utility::HashCode( sr->GetFilePath() );
		DecoderReferenceMap::iterator refDecoder = m_DecoderMap.find( hash );
		ASSERT( m_DecoderMap.end() != refDecoder );
		if ( 0 == -- refDecoder->second->m_nReference )
		{
			refDecoder->second->m_pDecoder->Deactive();
			if ( !refDecoder->second->m_pDecoder->isRequested() )//如果不在解码队列中，直接销毁
			{
				refDecoder->second->m_pDecoder->ReleaseDecoder( );
			}
			SAFE_DELETE( refDecoder->second );
			m_DecoderMap.erase( refDecoder );
		}
	}
}
