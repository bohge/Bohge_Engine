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
		SmartPtr<Decoder>		m_pDecoder;
		const char*				m_BufferAddress;
		uint					m_nBufferSize;
		uint					m_nCurrentBufferIndex;//当前声音buffer的index
		bool					m_isDone;
	public:
		SoundResource( SmartPtr<Decoder> decoder );
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
			DecoderManager::Instance()->PushDecodeJob( m_pDecoder );
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
