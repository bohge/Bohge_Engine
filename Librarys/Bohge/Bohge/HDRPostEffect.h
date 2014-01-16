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



/////////////////////////////////////
//			后期渲染特效		   //
/////////////////////////////////////
#pragma once
#include "IPostEffect.h"
#include "ActionManage.h"



namespace BohgeEngine
{
	class Engine;
	class RenderTarget;
	class HDRPostEffect : public IPostEffect
	{
	private:
		RenderTarget*		m_QuarterRT;
		RenderTarget*		m_BlurredRT;
		RenderTarget*		m_ScratchRT;
		RenderTarget*		m_BrightPassRT;
		RenderTarget*		m_64x64RT;
		RenderTarget*		m_16x16RT;
		RenderTarget*		m_4x4RT;
		RenderTarget*		m_1x1RT;
		RenderTarget*		m_PreviousAverageLumRT;
		RenderTarget*		m_LastAverageLumRT;

	//private:
	//	RendBuffer*			m_pDebugRendBuffer;
	//	VertexBuffer*		m_pDebugVertexBuffer1;
	//	VertexBuffer*		m_pDebugVertexBuffer2;
	//	VertexBuffer*		m_pDebugVertexBuffer3;
	//	VertexBuffer*		m_pDebugVertexBuffer4;
	//	IndicesBuffer*		m_pDebugIndicesBuffer;
	private:
		void _MakeBlur( Engine& engine, const RenderTarget* const InputTargetRT, RenderTarget* BlurredTargetRT );
	public:
		HDRPostEffect(void);
		~HDRPostEffect(void);
		virtual void OnSizeChange( const vector2d& size );
		virtual void Process(Engine& engine, const RenderTarget* const org, const RenderTarget* const scene, RenderTarget* const out );
	public:
		bool OnKeyDown( ActionManage::Key_Down );
	};

}