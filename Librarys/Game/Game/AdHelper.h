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



//////////////////////////////////
//		是否显示广告			//
//////////////////////////////////
#pragma once
#include "Predefine.h"
#include <string>


namespace BohgeGame
{
	class AdHelper
	{
	private:
		enum 
		{
			ORGID_LENGTH = 15,
			AESID_LENGTH = 16,	//这个是为加密的ID长度，原始ID15位，加密后16位
		};
	public:
		enum Platfrom
		{
			PF_ANDROID,
			PF_IPHONE,
			PF_IPAD,
		};
		enum On_Screen //针对android把广告显示在什么位置
		{
			OS_TOPCENTER,
			OS_TOPLEFT,
			OS_TOPRIGHT,
		};
	private:
		std::string		m_striPad;
		std::string		m_striPhone;
		std::string		m_strAndroid;
		bool			m_isGetID;
		bool			m_isShow;
		bool			m_isConnect;//是否有网络
		On_Screen		m_Position;
	private:
		AdHelper();
	public:
		static BOHGE_FORCEINLINE AdHelper& Instance()
		{
			static AdHelper ad;
			return ad;
		}
	public:
		std::string GetAdmobID( Platfrom p );
		void Connected();
	public:
		BOHGE_FORCEINLINE bool isShowAd()
		{
			return m_isShow;
		}
		BOHGE_FORCEINLINE void setShowAd(bool isshow)
		{
			m_isShow = isshow;
		}
		BOHGE_FORCEINLINE bool isConnect()
		{
			return m_isConnect;
		}
		BOHGE_FORCEINLINE On_Screen& Where()
		{
			return m_Position;
		}
	};
}