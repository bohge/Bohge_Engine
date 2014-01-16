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



/////////////////////////
//		UI对话框	   //
/////////////////////////

#pragma once
#include "3DMath.h"
#include "Event.hpp"
#include <string>


namespace BohgeEngine
{
	class UIEdit;
	class UIBase;
	class UIGroup;
}

namespace BohgeGame
{
	class GameMessageBox
	{
	private:
		uint									m_ButtonCount;
		bool									m_isShow;
		BohgeEngine::UIGroup*					m_pBox;
		BohgeEngine::UIGroup*					m_pBack;
		Event<bool,BohgeEngine::UIBase&>		m_EventButton1;//Button1代理
		Event<bool,BohgeEngine::UIBase&>		m_EventButton2;//Button2代理
	public:
		GameMessageBox(int ButtonCount);//按钮个数，最大2个
		~GameMessageBox();
		void SetResponse( IFunction<bool,BohgeEngine::UIBase&>* const button1 );
		void SetResponse( IFunction<bool,BohgeEngine::UIBase&>* const button1, IFunction<bool,BohgeEngine::UIBase&>* const button2 );
		void SetContext(const std::wstring& button1str, const std::wstring& str);
		void SetContext(const std::wstring& button1str, const std::wstring& button2str, const std::wstring& str);
		void SetContext(const std::wstring& str);
		void Popup();
		void Hide();
	private:
		bool OnButton1Click(BohgeEngine::UIBase& sender);
		bool OnButton2Click(BohgeEngine::UIBase& sender);
		bool OnAnimationDone(BohgeEngine::UIGroup& sender);
	public:
		BOHGE_FORCEINLINE bool isShowing()
		{
			return m_isShow;
		}
	};

	class UIEdit;
	class EditMessageBox
	{
	private:
		BohgeEngine::vector2f	m_ButtonFontSize;//按钮字体尺寸
		bool					m_isShow;
		BohgeEngine::UIGroup*	m_pBox;
		BohgeEngine::UIGroup*	m_pBack;
		BohgeEngine::UIEdit*	m_Edit;
		Event<bool,std::wstring&>	m_EventButton1;//Button1代理
		Event<bool,std::wstring&>	m_EventButton2;//Button2代理
		Event<bool,std::wstring&>	m_EventButton3;//Button3代理
	public:
		EditMessageBox(bool is2Button, uint editmax, bool isNumOnly);//两个按钮和三个按钮
		~EditMessageBox();
		void SetResponse( IFunction<bool,std::wstring&>* const button1 );
		void SetResponse( IFunction<bool,std::wstring&>* const button1, IFunction<bool,std::wstring&>* const button2 );
		void SetResponse( IFunction<bool,std::wstring&>* const button1, IFunction<bool,std::wstring&>* const button2, IFunction<bool,std::wstring&>* const button3 );
		void SetContext(const std::wstring& button1str, const std::wstring& button2str, const std::wstring& str);
		void SetContext(const std::wstring& button1str, const std::wstring& button2str, const std::wstring& button3str, const std::wstring& str);
		void Popup();
		void Hide();
		void SetEditContent( const std::wstring& str );
	private:
		bool OnButton1Click(BohgeEngine::UIBase& sender);
		bool OnButton2Click(BohgeEngine::UIBase& sender);
		bool OnButton3Click(BohgeEngine::UIBase& sender);
		bool OnAnimationDone(BohgeEngine::UIGroup& sender);
	public:
		BOHGE_FORCEINLINE bool isShowing()
		{
			return m_isShow;
		}
		BOHGE_FORCEINLINE BohgeEngine::vector2f& ButtonFontSize()
		{
			return m_ButtonFontSize;
		}
	};
}