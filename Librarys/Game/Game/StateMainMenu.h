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
//			State Manage class		   //
/////////////////////////////////////////

#pragma once


#include "StateInterface.h"

namespace BohgeEngine
{
	class UIBase;
	class ICamera;
	class Emitter;
	class UIGroup;
	class IPostEffect;
}

namespace BohgeGame
{
	class GameMessageBox;
	class StateMainMenu : public IGameState
	{
	private:
		BohgeEngine::IPostEffect*	m_Bloom;
		BohgeEngine::IPostEffect*	m_SSAO;
		BohgeEngine::IPostEffect*	m_DOF;
		BohgeEngine::IPostEffect*	m_FXAA;
	public:
		virtual void LoadResource(BohgeEngine::Engine& engine);
		virtual void ReleaseResource(BohgeEngine::Engine& engine);
		virtual void OnEnter(BohgeEngine::Engine& engine, StateManage::GameState Previous);
		virtual void Update(StateManage& state, BohgeEngine::Engine& engine, uint millisecond);
		virtual void OnLeave(BohgeEngine::Engine& engine);
	public:
		int diff(int a, int b)
		{
			return a - b;
		}
	};
}