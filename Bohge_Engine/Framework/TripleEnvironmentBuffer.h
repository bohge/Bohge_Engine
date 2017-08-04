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
#include "EnvironmentBuffer.h"
#include "IInserter.h"

namespace BohgeEngine
{
	class IThreadMutex;
	//使用三条buffer缓存多线程需要渲染的数据
	//第一条在渲染中，第二条已经填充完毕等待渲染，第三条填充中
	class BOHGE_FRAMEWORK TripleEnvironmentBuffer : public IInserter
	{
		RTTI_DRIVER_TYPE_DECLEAR( TripleEnvironmentBuffer, IInserter );
	private:
		enum Constant
		{
			TRC_BUFFER_SIZE	= 3,
		};
	private:
		EnvironmentBuffer			m_RenderPipeArray[TRC_BUFFER_SIZE];
		IThreadMutex*				m_pMutex;
		EnvironmentBuffer*			m_rpRendering;//正在渲染的
		EnvironmentBuffer*			m_rpWaiting;//等待渲染的
		EnvironmentBuffer*			m_rpFlushing;//正在填充的
	public:
		TripleEnvironmentBuffer(void);
		~TripleEnvironmentBuffer(void);
	public:
		void SwapFlushingBuffer();//交替填充buffer
		void SwapActivedBuffer();//交替
	public:
		virtual IntersectedType::Volume Intersect( const Aabbox3df& entity );
		virtual const Frustumf& GetFrustum( );
		virtual void Pushback( IComponent* com );
	public:
		BOHGE_FORCEINLINE const PipelineProperty::VisableLight& GetVisableLight()
		{
			return m_rpRendering->GetVisableLight();
		}
		BOHGE_FORCEINLINE void SetCamera( ICamera* came )
		{
			m_rpFlushing->SetCamera( came );
		}
	};
}
