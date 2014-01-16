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



/////////////////////////////////////////
//		游戏中post effect的基础类	   //
/////////////////////////////////////////
#pragma once
#include "Predefine.h"
#include "3DMath.h"


namespace BohgeEngine
{
	class PaperNode;
	class RenderTarget;
	class Engine;
	class RendBuffer;
	class VertexBuffer;
	class IndicesBuffer;
	class IPostEffect
	{
	private:
		enum Const
		{
			COUNT_NODE			= 1,
			COUNT_OF_VERTEX		= 4,
			COUNT_OF_INDIES		= 4,
		};
	private:
		int				m_Level;//优先级别
		bool			m_isActive;//是否激活
	protected:
		RendBuffer*			m_pRendBuffer;
		VertexBuffer*		m_pVertexBuffer;
		IndicesBuffer*		m_pIndicesBuffer;
	public:
		IPostEffect( int l );
		virtual ~IPostEffect();
		virtual void OnSizeChange( const vector2d& size ) = 0;
		//参数:原始画面，灯光视角画面，上一个ps处理后的画面，输出画面
		virtual void Process(Engine& engine, const RenderTarget* const org, const RenderTarget* const scene, RenderTarget* const out ) = 0; //唯一公用借口，处理后期效果
	public:
		BOHGE_FORCEINLINE int Level()
		{
			return m_Level;
		}
		BOHGE_FORCEINLINE const bool& isActive()
		{
			return m_isActive;
		}
		BOHGE_FORCEINLINE void SetActive( bool b )
		{
			m_isActive = b;
		}
	};

}