#pragma once
#include "Decoder.h"


#include <string>



namespace BohgeEngine
{
	class Decoder;
	class SoundResource//声音资源
	{
		//声音资源包在decoder的上层的用意是在加载资源的可以进行相同资源的加载管理
		//当一个声音资源被重复使用的时候，decoder不在流式加载资源，换言之不会再去释放旧资源
		//这样做可以用一定的空间资源换取时间
	private:
		Decoder*		m_pDecoder;
		const char*		m_BufferAddress;
		uint			m_nBufferSize;
		uint			m_nCurrentBufferIndex;//当前声音buffer的index
		bool			m_isDone;
	public:
		SoundResource( Decoder* decoder );
		~SoundResource(void);
	public:
		void FlushBufferData();
	public:
		BOHGE_FORCEINLINE const std::string& GetFilePath() const
		{
			return m_pDecoder->GetFilePath();
		}
		BOHGE_FORCEINLINE void RequestDecode()//要求解析数据段
		{
			m_pDecoder->RequestDecode();
		}
		BOHGE_FORCEINLINE void ReleaseResource()//sl需要释放资源，应为sl不需要读取
		{
			m_pDecoder->ReleaseDecoder();
		}
		BOHGE_FORCEINLINE int GetFrequency() const
		{
			return m_pDecoder->GetFrequency();
		}
		BOHGE_FORCEINLINE Decoder::Format GetFormat() const
		{
			return m_pDecoder->GetFormat();
		}
		BOHGE_FORCEINLINE int GetChannels() const
		{
			return m_pDecoder->GetChannels();
		}
		BOHGE_FORCEINLINE int GetBufferSize() const
		{
			return m_nBufferSize;
		}
		BOHGE_FORCEINLINE int GetSourceSize() const
		{
			return m_pDecoder->GetSourceSize();
		}
		BOHGE_FORCEINLINE const char* GetBufferChunk() const//得到数据段
		{
			return m_BufferAddress;
		}
		BOHGE_FORCEINLINE bool isChunkReady() const
		{
			return m_pDecoder->isChunkReady( m_nCurrentBufferIndex );
		}
		BOHGE_FORCEINLINE bool isDone() const //是否播放完毕
		{
			return m_isDone;
		}
	};
}
