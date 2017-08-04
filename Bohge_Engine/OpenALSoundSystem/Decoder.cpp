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

#include "Decoder.h"
#include "DecoderManager.h"
#include "Framework/IIOSystem.h"
#include "Framework/IFile.h"

#include <EASTL/string.h>


using namespace eastl;
using namespace BohgeEngine;

namespace OpenALSoundSystemPlugin
{
	//-------------------------------------------------------------------------------------------------------
	Decoder::Decoder( SoundType type )
		:m_isActived( true ),
		m_pFileStream( NULL ),
		m_eDecoderTypes( type ),
		m_nFrequency( 0 ),
		m_nFormat( DF_MONO_8 ),
		m_nChannels( 0 ),
		m_nSourceTotalSize( 0 ),
		m_nLoadedBufferIndex( 0 ),
		m_isRequested(false),
		m_isDecoding(false),
		m_isLoaded(false)
	{
	}
	//-------------------------------------------------------------------------------------------------------
	Decoder::~Decoder(void)
	{
		ASSERT( false == m_isLoaded );
	}
	//-------------------------------------------------------------------------------------------------------
	void Decoder::LoadResource( const eastl::string& path )
	{
		m_isLoaded = true;
		m_FilePath = path;
		m_pFileStream = IIOSystem::Instance()->FileFactory( path );
		m_pFileStream->OpenFile( IFile::AT_READ );
		_DoInitialize(m_nFrequency, m_nFormat, m_nChannels, m_nSourceTotalSize, m_dTime);
		m_nSourceTotalSize -= ( m_nSourceTotalSize % (m_nChannels*2) );//确保整数倍
		int vecSize = Math::Ceil( static_cast<float>(m_nSourceTotalSize) / DecoderManager::DC_DEFUALT_SOUND_BUFFER_SIZE );
		m_BufferVector.resize( vecSize );
		for ( int i = 0 ; i < vecSize ; i ++ )
		{
			m_BufferVector[i] = NEW char[DecoderManager::DC_DEFUALT_SOUND_BUFFER_SIZE];
			memset( m_BufferVector[i], 0, DecoderManager::DC_DEFUALT_SOUND_BUFFER_SIZE );
		}
		SetJobType( IJob::JT_ASYNCHRONOUS );
	}
	//-------------------------------------------------------------------------------------------------------
	void Decoder::ReleaseDecoder( )
	{
		if ( m_isLoaded )
		{
			m_isLoaded = false;
			_DoReleaseDecoder();
			m_pFileStream->CloseFile();
			IIOSystem::Instance()->RecycleBin( m_pFileStream );
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
	bool Decoder::PrepareDecode()
	{
		if ( !m_isRequested && m_nLoadedBufferIndex < m_BufferVector.size() )//如果已经要求解码了，就不重复在队列中添加了
		{
			m_isRequested = true;
			//添加到解码线程队列，进行解码
			SetPriority( m_nLoadedBufferIndex );//优先级就是读到第几个buffer
			return true;
		}
		return false;
	}
	//-------------------------------------------------------------------------------------------------------
	void Decoder::_DoJob( SmartPtr<IJob>& self )
	{
		m_isDecoding = true;
		if ( m_isActived )
		{
			uint from = m_nLoadedBufferIndex * DecoderManager::DC_DEFUALT_SOUND_BUFFER_SIZE;
			uint to = _GetEndPosition(from);
			_DoDecodeAsyn( from, to );
			m_nLoadedBufferIndex ++;
		}		
		if ( !m_isActived )
		{
			ReleaseDecoder();//如果释放的时候decoder已经在异步队列中,则需要在这里释放资源
		}
		m_isRequested = false;
		m_isDecoding = false;
	}
	//-------------------------------------------------------------------------------------------------------
	void Decoder::_DoYieldJob()
	{
		//暂时不实现了
	}

}