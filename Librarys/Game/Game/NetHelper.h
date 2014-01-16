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
//			网络组件		   //
/////////////////////////////////

#pragma once
#include "Correspondent.h"
#include "NetBase.h"
#include <string>

namespace BohgeGame
{

	class NetHelper
	{
	private:
		bool							m_isConnect;//是否是连接的
		bool							m_isClickBattel;
		bool							m_isRoomHost;//是否是房主
		uint							m_Location;//所在区
		uint							m_RoomID;	
		std::string						m_strUDID;//udid
		std::wstring					m_Name;//玩家姓名
		uint							m_Win;
		uint							m_Lose;
	private:
		NetHelper(void);
	public:
		~NetHelper(void);
		void CheckReconnect();//检测是否需从新链接
		void ConnectServer();//全部的连接服务器代码都要掉这个
		void DisconnectServer();
		//void ConnectToPlayer( const BohgeNet::Address& add);//连接到另一个玩家
	public:
		BOHGE_FORCEINLINE static NetHelper& Instance()
		{
			static NetHelper nc;
			return nc;
		}
		BOHGE_FORCEINLINE std::string& UDID()
		{
			return m_strUDID;
		}
		BOHGE_FORCEINLINE std::wstring& Name()
		{
			return m_Name;
		}
		BOHGE_FORCEINLINE bool& isRoomHost()
		{
			return m_isRoomHost;
		}
		BOHGE_FORCEINLINE uint& RoomID()
		{
			return m_RoomID;
		}
		BOHGE_FORCEINLINE uint& Location()
		{
			return m_Location;
		}
		BOHGE_FORCEINLINE bool& isBattel()
		{
			return m_isClickBattel;
		}
		BOHGE_FORCEINLINE uint& Win()
		{
			return m_Win;
		}
		BOHGE_FORCEINLINE uint& Lose()
		{
			return m_Lose;
		}
	};

}