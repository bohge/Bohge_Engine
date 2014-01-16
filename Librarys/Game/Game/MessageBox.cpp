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


#include "MessageBox.h"
#include "UIManager.h"
#include "UI.h"
#include "UIAnimation.h"
#include "GameResourceList.h"

using namespace BohgeEngine;

namespace BohgeGame
{
	//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	GameMessageBox::GameMessageBox(int ButtonCount)
		:m_isShow(false),
		m_ButtonCount( ButtonCount )
	{
		//m_pBack = NEW UIGroup(UIFileName[ui_MessageBack]);
		//m_pBack->SetShow(false);
		//m_pBack->SetCorlor( Color(0,0,0,0.5) );
		//switch ( ButtonCount )
		//{
		//case 0 :
		//	{
		//		m_pBox = NEW UIGroup(UIFileName[ui_MessageBox]);
		//		m_pBox->SetShow(false);
		//	}break;
		//case 1 :
		//	{
		//		m_pBox = NEW UIGroup(UIFileName[ui_MessageBox1Button]);
		//		m_pBox->SetShow(false);
		//		UIButton* temp = m_pBox->Control<UIButton>("Button1");
		//		temp->AddResponse( MEMBER_FUNC_PTR(&GameMessageBox::OnButton1Click) );
		//	}break;
		//case 2 :
		//	{
		//		m_pBox = NEW UIGroup(UIFileName[ui_MessageBoxButton]);
		//		m_pBox->SetShow(false);
		//		UIButton* temp = m_pBox->Control<UIButton>("Button1");
		//		temp->AddResponse( MEMBER_FUNC_PTR(&GameMessageBox::OnButton1Click) );
		//		temp = m_pBox->Control<UIButton>("Button2");
		//		temp->AddResponse( MEMBER_FUNC_PTR(&GameMessageBox::OnButton2Click) );
		//	}break;
		//default:
		//	{
		//		ASSERT(false);
		//	}break;
		//}
		//UIAnimation anim(500);
		//anim.ColorAnimation( Color(1,1,1,0), Color(1,1,1,1) );
		//anim.ScaleAnimation( vector2f::Zero() , vector2f(1,1) );
		//m_pBox->SetUIAnimation( anim , MEMBER_FUNC_PTR(&GameMessageBox::OnAnimationDone) );	
		//m_pBox->setResponse(false);

	}
	//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	GameMessageBox::~GameMessageBox()
	{
		Engine::Instance().GetUIManager()->DeleteUIGroup(m_pBack);
		Engine::Instance().GetUIManager()->DeleteUIGroup(m_pBox);
	}
	//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void GameMessageBox::SetResponse( IFunction<bool,UIBase&>* const button1 )
	{
		m_EventButton1.ClearConnect();
		m_EventButton1.Connect( button1 );
	}
	//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void GameMessageBox::SetResponse( IFunction<bool,UIBase&>* const button1, IFunction<bool,UIBase&>* const button2 )
	{
		m_EventButton1.ClearConnect();
		m_EventButton2.ClearConnect();
		m_EventButton1.Connect( button1 );
		m_EventButton2.Connect( button2 );
	}
	//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void GameMessageBox::SetContext(const std::wstring& button1str, const std::wstring& str)
	{
		m_pBox->Control<UIButton>("Button1")->SetString(vector2f(0.06f, 0.08f), Color::Black, true, button1str);
		UIStatic* temp = m_pBox->Control<UIStatic>("box");
		temp->SetString(vector2f(0.06f, 0.08f), Color::Black, true, str);
		temp->SetStringPostion( temp->GetCenter() + vector2f( 0, 0.2) );
	}
	//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void GameMessageBox::SetContext(const std::wstring& button1str, const std::wstring& button2str, const std::wstring& str)
	{
		m_pBox->Control<UIButton>("Button1")->SetString(vector2f(0.06f, 0.08f), Color::Black, true, button1str);
		m_pBox->Control<UIButton>("Button2")->SetString(vector2f(0.06f, 0.08f), Color::Black, true, button2str);
		UIStatic* temp = m_pBox->Control<UIStatic>("box");
		temp->SetString(vector2f(0.06f, 0.08f), Color::Black, true, str);
		temp->SetStringPostion( temp->GetCenter() + vector2f( 0, 0.2) );
	}
	//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void GameMessageBox::SetContext(const std::wstring& str)
	{
		UIStatic* temp = m_pBox->Control<UIStatic>("box");
		temp->SetString(vector2f(0.06f, 0.08f), Color::Black, true, str);
		if( 0 == m_ButtonCount )
		{
			temp->SetStringPostion( temp->GetCenter() );
		}
		else
		{
			temp->SetStringPostion( temp->GetCenter() + vector2f( 0, 0.2) );
		}	
	}
	//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void GameMessageBox::Popup()
	{
		m_isShow = true;
		m_pBack->TopMost();
		m_pBox->TopMost();
		m_pBack->SetShow(true);
		m_pBox->SetShow(true);
		m_pBox->setResponse(true);
		m_pBox->ResetAnimation();
	}
	//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void GameMessageBox::Hide()
	{
		m_isShow = false;
		m_pBack->SetShow(false);
		m_pBox->SetShow(false);
		m_pBox->setResponse(false);
	}
	//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	bool GameMessageBox::OnButton1Click(UIBase& sender)
	{
		m_isShow = false;
		m_pBack->SetShow(false);
		m_pBox->SetShow(false);
		m_EventButton1.Multicast(sender);
		return true;
	}
	//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	bool GameMessageBox::OnButton2Click(UIBase& sender)
	{
		m_isShow = false;
		m_pBack->SetShow(false);
		m_pBox->SetShow(false);
		m_EventButton2.Multicast(sender);
		return true;
	}
	//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	bool GameMessageBox::OnAnimationDone(UIGroup& sender)
	{
		m_pBox->setResponse(true);
		return true;
	}






	/////////////////////////
	//		UI对话框	   //
	/////////////////////////
	//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	EditMessageBox::EditMessageBox(bool is2Button, uint editmax, bool isNumOnly)
		:m_isShow(false),
		m_ButtonFontSize( 0.06f, 0.08f )
	{
		//m_pBack = NEW UIGroup(UIFileName[ui_MessageBack]);
		//m_pBack->SetShow(false);
		//m_pBack->SetCorlor( Color(0,0,0,0.5) );

		//if( is2Button )
		//{
		//	m_pBox = NEW UIGroup(UIFileName[ui_EditMessage2Button]);
		//	m_pBox->SetShow(false);
		//	UIButton* temp = m_pBox->Control<UIButton>("Button1");
		//	temp->AddResponse( MEMBER_FUNC_PTR(&EditMessageBox::OnButton1Click) );
		//	temp = m_pBox->Control<UIButton>("Button2");
		//	temp->AddResponse( MEMBER_FUNC_PTR(&EditMessageBox::OnButton2Click) );
		//}
		//else
		//{
		//	m_pBox = NEW UIGroup(UIFileName[ui_EditMessage3Button]);
		//	m_pBox->SetShow(false);
		//	UIButton* temp = m_pBox->Control<UIButton>("Button1");
		//	temp->AddResponse( MEMBER_FUNC_PTR(&EditMessageBox::OnButton1Click) );
		//	temp = m_pBox->Control<UIButton>("Button2");
		//	temp->AddResponse( MEMBER_FUNC_PTR(&EditMessageBox::OnButton2Click) );
		//	temp = m_pBox->Control<UIButton>("Button3");
		//	temp->AddResponse( MEMBER_FUNC_PTR(&EditMessageBox::OnButton3Click) );
		//}


		//m_Edit = m_pBox->Control<UIEdit>( "Edit" );
		//m_Edit->MaxInput() = editmax;
		//m_Edit->NumOnly() = isNumOnly;
		//m_Edit->FontSize() = vector2f(0.045f, 0.065f);
		//m_Edit->FontOffset() = vector2f(0.05f, 0);

		//UIAnimation anim(500);
		//anim.ColorAnimation( Color(1,1,1,0), Color(1,1,1,1) );
		//anim.ScaleAnimation( vector2f::Zero() , vector2f(1,1) );
		//m_pBox->SetUIAnimation( anim , MEMBER_FUNC_PTR(&EditMessageBox::OnAnimationDone) );	
		//m_pBox->setResponse(false);

	}
	//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	EditMessageBox::~EditMessageBox()
	{
		Engine::Instance().GetUIManager()->DeleteUIGroup(m_pBack);
		Engine::Instance().GetUIManager()->DeleteUIGroup(m_pBox);
	}
	//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void EditMessageBox::SetResponse( IFunction<bool,std::wstring&>* const button1 )
	{
		m_EventButton1.ClearConnect();
		m_EventButton1.Connect( button1 );
	}
	//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void EditMessageBox::SetResponse( IFunction<bool,std::wstring&>* const button1, IFunction<bool,std::wstring&>* const button2 )
	{
		m_EventButton1.ClearConnect();
		m_EventButton2.ClearConnect();
		m_EventButton1.Connect( button1 );
		m_EventButton2.Connect( button2 );
	}
	//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void EditMessageBox::SetResponse( IFunction<bool,std::wstring&>* const button1, IFunction<bool,std::wstring&>* const button2, IFunction<bool,std::wstring&>* const button3 )
	{
		m_EventButton1.ClearConnect();
		m_EventButton2.ClearConnect();
		m_EventButton3.ClearConnect();
		m_EventButton1.Connect( button1 );
		m_EventButton2.Connect( button2 );
		m_EventButton3.Connect( button3 );
	}
	//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void EditMessageBox::SetContext(const std::wstring& button1str, const std::wstring& button2str, const std::wstring& str)
	{
		m_pBox->Control<UIButton>("Button1")->SetString( m_ButtonFontSize, Color::Black, true, button1str);
		m_pBox->Control<UIButton>("Button2")->SetString( m_ButtonFontSize, Color::Black, true, button2str);
		UIStatic* temp = m_pBox->Control<UIStatic>("splie");
		temp->SetString(vector2f(0.04f, 0.07f), Color::Black, true, str);
		temp->SetStringPostion( temp->GetCenter() + vector2f( 0, 0.08) );
	}
	//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void EditMessageBox::SetContext(const std::wstring& button1str, const std::wstring& button2str, const std::wstring& button3str, const std::wstring& str)
	{
		m_pBox->Control<UIButton>("Button1")->SetString( m_ButtonFontSize, Color::Black, true, button1str);
		m_pBox->Control<UIButton>("Button2")->SetString( m_ButtonFontSize, Color::Black, true, button2str);
		m_pBox->Control<UIButton>("Button3")->SetString( m_ButtonFontSize, Color::Black, true, button3str);
		UIStatic* temp = m_pBox->Control<UIStatic>("splie");
		temp->SetString(vector2f(0.04f, 0.07f), Color::Black, true, str);
		temp->SetStringPostion( temp->GetCenter() + vector2f( 0, 0.08) );
	}
	//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void EditMessageBox::Popup()
	{
		m_isShow = true;
		m_pBack->TopMost();
		m_pBox->TopMost();
		m_pBack->SetShow(true);
		m_pBox->SetShow(true);
		m_pBox->setResponse(true);
		m_pBox->ResetAnimation();
	}
	//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void EditMessageBox::Hide()
	{
		m_isShow = false;
		m_pBack->SetShow(false);
		m_pBox->SetShow(false);
		m_pBox->setResponse(false);
	}
	//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	bool EditMessageBox::OnButton1Click(UIBase& sender)
	{
		m_isShow = false;
		m_pBack->SetShow(false);
		m_pBox->SetShow(false);
		m_EventButton1.Multicast( m_Edit->GetContent() );
		return true;
	}
	//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	bool EditMessageBox::OnButton2Click(UIBase& sender)
	{
		m_isShow = false;
		m_pBack->SetShow(false);
		m_pBox->SetShow(false);
		m_EventButton2.Multicast( m_Edit->GetContent() );
		return true;
	}
	//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	bool EditMessageBox::OnButton3Click(UIBase& sender)
	{
		m_isShow = false;
		m_pBack->SetShow(false);
		m_pBox->SetShow(false);
		m_EventButton3.Multicast( m_Edit->GetContent() );
		return true;
	}
	//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	bool EditMessageBox::OnAnimationDone(UIGroup& sender)
	{
		m_pBox->setResponse(true);
		return true;
	}
	//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void EditMessageBox::SetEditContent( const std::wstring& str )
	{
		m_Edit->OnReciveText(str);
	}
}