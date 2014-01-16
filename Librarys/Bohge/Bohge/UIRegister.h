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



/////////////////////////////////
//		注册engine和管理	   //
/////////////////////////////////
#pragma once
#include "UIManager.h"
#include "UIAnimation.h"
#include "UI.h"
#include <string>


namespace BohgeEngine
{
	static void RegisterUIGroup( ScriptBase* s, const char* classname )
	{
		//注册一个类，实际是table
		s->RegClass<UIGroup >( classname );
		//注册table成员函数
		s->RegClassFunc< UIGroup >( "SetCenter", &UIGroup::SetCenter );
		s->RegClassFunc< UIGroup >( "SetScale", &UIGroup::SetScale );
		s->RegClassFunc< UIGroup >( "SetShow", &UIGroup::SetShow );
		s->RegClassFunc< UIGroup >( "setResponse", &UIGroup::setResponse );
		s->RegClassFunc< UIGroup >( "SetUIAnimation", &UIGroup::SetUIAnimation );
		s->RegClassFunc< UIGroup >( "ResetAnimation", &UIGroup::ResetAnimation );
		s->RegClassFunc< UIGroup >( "TopMost", &UIGroup::TopMost );
		s->RegClassFunc< UIGroup >( "SetCorlor", &UIGroup::SetCorlor );
		s->RegClassFunc< UIGroup >( "isResponse", &UIGroup::isResponse );
		s->RegClassFunc< UIGroup >( "isAnimation", &UIGroup::isAnimation );
		s->RegClassFunc< UIGroup >( "isShow", &UIGroup::isShow );
		s->RegClassFunc< UIGroup >( "GetPostion", &UIGroup::GetPostion );
		s->RegClassFunc< UIGroup >( "GetUIAnimation", &UIGroup::GetUIAnimation );
		s->RegClassFunc< UIGroup >( "PauseAnimation", &UIGroup::PauseAnimation );
		s->RegClassFunc< UIGroup >( "isManaged", &UIGroup::isManaged );
		s->RegClassFunc< UIGroup >( "SetManaged", &UIGroup::SetManaged );
		s->RegClassFunc< UIGroup, UIStatic* (UIGroup::*)(const char*)  >( "ControlUIStatic", &UIGroup::Control<UIStatic> );
		s->RegClassFunc< UIGroup, UIButton* (UIGroup::*)(const char*)  >( "ControlUIButton", &UIGroup::Control<UIButton> );
		s->RegClassFunc< UIGroup, UISlider* (UIGroup::*)(const char*)  >( "ControlUISlider", &UIGroup::Control<UISlider> );
		s->RegClassFunc< UIGroup, UIList* (UIGroup::*)(const char*)  >( "ControlUIList", &UIGroup::Control<UIList> );
		s->RegClassFunc< UIGroup, UIEdit* (UIGroup::*)(const char*)  >( "ControlUIEdit", &UIGroup::Control<UIEdit> );
	}



	static void RegisterUIAnimation( ScriptBase* s, const char* classname )
	{
		//注册一个类，实际是table
		s->RegClass<UIAnimation >( classname );
		//注册构造函数
		s->RegClassCons<UIAnimation, int >();
		//注册table成员函数
		s->RegClassFunc< UIAnimation >( "ColorAnimation", &UIAnimation::ColorAnimation );
		s->RegClassFunc< UIAnimation >( "PostionAnimation", &UIAnimation::PostionAnimation );
		s->RegClassFunc< UIAnimation >( "ScaleAnimation", &UIAnimation::ScaleAnimation );
		s->RegClassFunc< UIAnimation >( "FlashAnimation", &UIAnimation::FlashAnimation );
		s->RegClassFunc< UIAnimation >( "SetDelay", &UIAnimation::SetDelay );
		s->RegClassFunc< UIAnimation >( "Loop", &UIAnimation::Loop );
	}


	static void RegisterUIBase( ScriptBase* s, const char* classname )
	{
		//注册一个类，实际是table
		s->RegClass<UIBase >( classname );
		//注册table成员函数
		s->RegClassFunc< UIBase >( "SetCenter", &UIBase::SetCenter );
		s->RegClassFunc< UIBase >( "SetCenterForce", &UIBase::SetCenterForce );
		s->RegClassFunc< UIBase >( "SetScale", &UIBase::SetScale );
		s->RegClassFunc< UIBase >( "Move", &UIBase::Move );
		s->RegClassFunc< UIBase >( "isSelected", &UIBase::isSelected );
		s->RegClassFunc< UIBase >( "isUI", &UIBase::isUI );
		s->RegClassFunc< UIBase, void (UIBase::*)(const vector2f&, const Color&, bool, const char* ) >( "SetString", &UIBase::SetString );
		s->RegClassFunc< UIBase >( "SetShow", &UIBase::SetShow );
		s->RegClassFunc< UIBase >( "Reset", &UIBase::Reset );
		s->RegClassFunc< UIBase >( "SetStringShow", &UIBase::SetStringShow );
		s->RegClassFunc< UIBase >( "SetActive", &UIBase::SetActive );
		s->RegClassFunc< UIBase >( "UIType", &UIBase::UIType );
		s->RegClassFunc< UIBase >( "isShow", &UIBase::isShow );
		s->RegClassFunc< UIBase >( "isResponse", &UIBase::isResponse );
		s->RegClassFunc< UIBase >( "isPushDonw", &UIBase::isPushDonw );
		s->RegClassFunc< UIBase >( "isActive", &UIBase::isActive );
		s->RegClassFunc< UIBase >( "GetLayer", &UIBase::GetLayer );
		s->RegClassFunc< UIBase >( "GetBindBox", &UIBase::GetBindBox );
		s->RegClassFunc< UIBase >( "GetSize", &UIBase::GetSize );
		s->RegClassFunc< UIBase >( "GetCenter", &UIBase::GetCenter );
		s->RegClassFunc< UIBase, void (UIBase::*)(const vector2f&)  >( "SetStringPostion", &UIBase::SetStringPostion );
		s->RegClassFunc< UIBase, void (UIBase::*)(const vector2f&, BString::Alignment)  >( "SetStringPostion", &UIBase::SetStringPostion );
	}


	static void RegisterUIStatic( ScriptBase* s, const char* classname )
	{
		s->LoadClasses( ScriptManager::CLASS_UIBASE );
		//注册一个类，实际是table
		s->RegClass<UIStatic >( classname );
		//继承
		s->RegClassInh<UIStatic, UIBase >();
	}


	static void RegisterUIButton( ScriptBase* s, const char* classname )
	{
		s->LoadClasses( ScriptManager::CLASS_UIBASE );
		//注册一个类，实际是table
		s->RegClass<UIButton >( classname );
		//继承
		s->RegClassInh<UIButton, UIBase >();
		//注册table成员函数
		s->RegClassFunc< UIButton >( "SetActive", &UIButton::SetActive );
		s->RegClassFunc< UIButton, void (UIButton::*)( ScriptBase*, const char*)  >( "AddResponse", &UIButton::AddResponse );
	}


	static void RegisterUISlider( ScriptBase* s, const char* classname )
	{
		s->LoadClasses( ScriptManager::CLASS_UIBASE );
		//注册一个类，实际是table
		s->RegClass<UISlider >( classname );
		//继承
		s->RegClassInh<UISlider, UIBase >();
		//注册table成员函数
		s->RegClassFunc< UISlider >( "SetPercentage", &UISlider::SetPercentage );
		s->RegClassFunc< UISlider >( "SetMinPercentage", &UISlider::SetMinPercentage );
		s->RegClassFunc< UISlider >( "Percentage", &UISlider::Percentage );
		s->RegClassFunc< UISlider >( "SetActive", &UISlider::SetActive );
		s->RegClassFunc< UISlider, void (UISlider::*)( ScriptBase*, const char*)  >( "AddResponse", &UISlider::AddResponse );
	}



	static void RegisterUIList( ScriptBase* s, const char* classname )
	{
		s->LoadClasses( ScriptManager::CLASS_UIBASE );
		//注册一个类，实际是table
		s->RegClass<UIList >( classname );
		//继承
		s->RegClassInh<UIList, UIBase >();
		//注册table成员函数
		s->RegClassFunc< UIList >( "SetActive", &UIList::SetActive );
		s->RegClassFunc< UIList >( "Clear", &UIList::Clear );
		s->RegClassFunc< UIList, void (UIList::*)( ScriptBase*, const char*)  >( "AddResponse", &UIList::AddResponse );
	}



	static void RegisterUIEdit( ScriptBase* s, const char* classname )
	{
		s->LoadClasses( ScriptManager::CLASS_UIBASE );
		//注册一个类，实际是table
		s->RegClass<UIEdit >( classname );
		//继承
		s->RegClassInh<UIEdit, UIBase >();
		//注册table成员函数
		s->RegClassFunc< UIEdit >( "Clear", &UIEdit::Clear );
		s->RegClassFunc< UIEdit >( "SetFontSize", &UIEdit::SetFontSize );
		s->RegClassFunc< UIEdit >( "SetFontColor", &UIEdit::SetFontColor );
		s->RegClassFunc< UIEdit >( "SetMaxInput", &UIEdit::SetMaxInput );
		s->RegClassFunc< UIEdit >( "NumOnly", &UIEdit::NumOnly );
		s->RegClassFunc< UIEdit >( "FontOffset", &UIEdit::FontOffset );
	}
}