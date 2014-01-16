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
//			购买物品中间类		   //
/////////////////////////////////////

#pragma once
#include "Event.hpp"
#include "Correspondent.h"
#include "NetBase.h"
#include <map>
#include <string>


namespace BohgeEngine
{
	class Engine;
}
namespace BohgeGame
{
	class ShopHelper
	{
	public:
		enum ShopStatus
		{
			NONE,
			VERSTION_NOT_MATCH,
			NEW_USER,
			NORMAL,
		};
	private:
		enum
		{
			ITEM_COUNT	=	6,//六个物品数量
		};
	private:
		typedef std::map<int, std::wstring> ItemMap;
	private:
		bool				m_isResend;
		bool				m_isAllowIAP;
		bool				m_isLoadDone;
		bool				m_isIAPProcess;//是否有任意订单在等待反馈状态
		ItemMap				m_ItmeString;

		//网络部分
		ShopStatus				m_ShopStatus;
		bool					m_isRecevCustomData;//但收到需要shop处理的信号
		bool					m_isSendedReceipt;//发送订单数据后设置为true，当服务器返回数据后设置为false
		bool					m_isServerRecevr;//服务器是否收到了订单
		std::string				m_Recepit;
		Event<bool,bool>		m_OnReceiptResult;//订单操作响应函数
	private:
		ShopHelper();
		bool CheckReceipt();
	public:
		void Request();//重新请求
		void InsertItemString(int price, const std::wstring& pri, const std::wstring& str);//添加数据字符串
		const std::wstring& GetString(int index);
		void BuyItem(int index);
		void PaymentTransactionStatePurchased( const std::string& receipt);
		void PaymentTransactionStateFailed();
		bool isUnprocessReceipt();//进入时本地没有处理的订单
		void ShopServerStatus(BohgeEngine::Engine& engine);
	public://针对订单的操作。
		bool ListenReceipt( BohgeNet::TCPCorrespondent& sender, BohgeNet::NetPackage& pak);
		void SendReceipt(const std::string& receiptEncode);
	public:
		static BOHGE_FORCEINLINE ShopHelper& Instance()
		{
			static ShopHelper sh;
			return sh;
		}
		BOHGE_FORCEINLINE bool isLoadDone()
		{
			return m_isLoadDone;
		}
		BOHGE_FORCEINLINE bool& isAllowIAP()
		{
			return m_isAllowIAP;
		}
		BOHGE_FORCEINLINE bool isWaitReslut()//是否在等待反馈
		{
			return m_isIAPProcess;
		}
		BOHGE_FORCEINLINE Event<bool,bool>& ReceiptReslutCallBack()
		{
			return m_OnReceiptResult;
		}
		BOHGE_FORCEINLINE ShopStatus GetShopStatus()
		{
			return m_ShopStatus;
		}
		BOHGE_FORCEINLINE bool isRecevData()
		{
			if( m_isRecevCustomData )
			{
				m_isRecevCustomData = false;
				return true;
			}
			return false;
		}
	};
}