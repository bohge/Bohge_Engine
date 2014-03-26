#include "SoundResource.h"
#include "DecoderManager.h"
#include "Log.h"

namespace BohgeEngine
{
	//-------------------------------------------------------------------------------------------------------
	SoundResource::SoundResource( Decoder* decoder )
		:m_pDecoder(decoder ),
		m_nBufferSize(0),
		m_nCurrentBufferIndex(0),
		m_isDone(false)
	{
	}
	//-------------------------------------------------------------------------------------------------------
	SoundResource::~SoundResource(void)
	{
		DecoderManager::Instance()->UnloadSoundDecoder( m_pDecoder );
	}
	//-------------------------------------------------------------------------------------------------------
	void SoundResource::FlushBufferData()
	{
		Decoder::BufferChunk buffer = m_pDecoder->GetBufferChunk(m_nCurrentBufferIndex);
		m_nCurrentBufferIndex = buffer.GetNextIndex();
		//DEBUGLOG("next index %d\n", m_nCurrentBufferIndex );
		m_nBufferSize = buffer.GetSize();
		m_BufferAddress = buffer.GetBuffer();
		m_isDone = buffer.isDone();
		RequestDecode();//要求下一段数据
		//DEBUGLOG("m_isDone %s\n", m_isDone ? "true" : "false" );
	}

}