#include "SoundResource.h"
#include "DecoderManager.h"


namespace BohgeEngine
{
	//-------------------------------------------------------------------------------------------------------
	SoundResource::SoundResource( Decoder* decoder )
		:m_pDecoder(decoder ),
		m_nBufferSize(0),
		m_nCurrentBufferPosition(0)
	{
	}
	//-------------------------------------------------------------------------------------------------------
	SoundResource::~SoundResource(void)
	{
		DecoderManager::Instance()->UnloadSoundDecoder( m_pDecoder );
	}
	//-------------------------------------------------------------------------------------------------------
	void SoundResource::DecoderNextChunk()
	{
		m_pDecoder->RequestDecode( m_nCurrentBufferPosition );
	}
	//-------------------------------------------------------------------------------------------------------
	void SoundResource::_CopyBufferData()
	{
		Decoder::BufferChunk buffer = m_pDecoder->GetBufferChunk(m_nCurrentBufferPosition);
		m_nCurrentBufferPosition = buffer.GetNextPosition();
		m_nBufferSize = buffer.GetSize();
		m_BufferAddress = buffer.GetBuffer();
	}

}