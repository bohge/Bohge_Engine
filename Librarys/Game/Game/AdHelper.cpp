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
#include "AdHelper.h"
#include "Predefine.h"
#include "AES.h"

using namespace BohgeEngine;

namespace BohgeGame
{
	AdHelper::AdHelper()
		:m_isShow(false),
		m_isConnect(false),
		m_isGetID(false),
		m_Position( OS_TOPCENTER )
	{
		char padID[]={0x7a,0x59,0x04,0x92,0x29,0xf9,0xe5,0x2e,   0xe6,0x2d,0x72,0xf5,0xc6,0xc0,0x04,0x47};//加密后的ID
		char phoID[]={0x7e,0x07,0x7a,0xf2,0x85,0x6a,0x85,0xe8,   0xb4,0xd0,0xb0,0xa0,0x7f,0xc0,0x74,0x77};
		char andID[]={0x90,0x3c,0x90,0x6f,0x88,0x90,0xa3,0xfa,   0x5b,0xdf,0xdb,0x94,0x10,0x35,0x4e,0x8f};
		m_striPad.resize(AESID_LENGTH);
		m_striPhone.resize(AESID_LENGTH);
		m_strAndroid.resize(AESID_LENGTH);
		for ( int i = 0 ; i < AESID_LENGTH ; i ++ )
		{
			m_striPad[i] = padID[i];
			m_striPhone[i] = phoID[i];
			m_strAndroid[i] = andID[i];
		}
	}
	//-------------------------------------------------------------------
	std::string AdHelper::GetAdmobID( Platfrom p )
	{
		m_isGetID = true;
		std::string result;
		switch( p )
		{
		case PF_ANDROID:
			{
				result.resize(m_strAndroid.size());
				AES::Instance().Decrypt( m_strAndroid.c_str(), &result[0], m_strAndroid.size() );
			}break;
		case PF_IPHONE:
			{
				result.resize(m_striPhone.size());
				AES::Instance().Decrypt( m_striPhone.c_str(), &result[0], m_striPhone.size() );
			}break;
		case PF_IPAD:
			{
				result.resize(m_striPad.size());
				AES::Instance().Decrypt( m_striPad.c_str(), &result[0], m_striPad.size() );
			}break;
		}
		result.erase( (++result.rbegin()).base() );//去除最后一位，应为AdmobID是15位的，最后一位是为了对齐aes的要求
		return result;
	}

	void AdHelper::Connected()
	{
		if( true != m_isGetID )
		{
			ASSERT( false );
			char* p = NULL;//故意crash app
			*p = 'a';
		}
		m_isConnect = true;//已经连接上了
	}
}