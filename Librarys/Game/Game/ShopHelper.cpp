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
#include "ShopHelper.h"
#include "Predefine.h"
#include "BohgeNet.h"
#include "NetMessage.h"
#include "NetBase.h"
#include "NetHost.h"
#include "Serializer.h"
#include "Base64.h"
#include "Engine.h"
#include "NetHelper.h"
#include "IOSystem.h"

#include <sstream>
#include <math.h>


using namespace std;
using namespace BohgeNet;
using namespace BohgeEngine;

extern void RefreshItem();
extern void IAPBuyItem(int index);


namespace BohgeGame
{
	const string TempRecepitName = "receipt.tmp";

	//-------------------------------------------------------------------------------------------------------
	ShopHelper::ShopHelper()
		:m_isLoadDone(false),
		m_isAllowIAP(false),
		m_isIAPProcess(false),
		m_isResend(false),
		m_isSendedReceipt(false),
		m_isServerRecevr(false),
		m_isRecevCustomData(false),
		m_ShopStatus(NONE)
	{

	}
	//-------------------------------------------------------------------------------------------------------
	bool ShopHelper::CheckReceipt()
	{
		if( IOINSTANCE.isExist( TempRecepitName )//如果文件目录下有次文件说明上次没有处理完成
			&& !m_isIAPProcess)//且不能有正在处理的IAP
		{
			Serializer ser( IOINSTANCE.WriteFolder() + TempRecepitName );
			string receipt;
			ser>>receipt;
			m_Recepit = receipt;
			this->SendReceipt( receipt );
			m_isIAPProcess = true;
			return true;
		}
		return false;
	}
	//-------------------------------------------------------------------------------------------------------
	bool ShopHelper::isUnprocessReceipt()
	{
		if( IOINSTANCE.isExist( TempRecepitName )//如果文件目录下有次文件说明上次没有处理完成
			&& !m_isIAPProcess)//且不能有正在处理的IAP
		{
			//this->ShopServerStatus();
			return true;
		}
		return false;
	}
	//-------------------------------------------------------------------------------------------------------
	void ShopHelper::InsertItemString(int price, const std::wstring& pri, const std::wstring& str)
	{
		wstringstream ws;
		ws<<str<<" "<<pri;
		m_ItmeString.insert( make_pair(price, ws.str() ) );
		m_isLoadDone = m_ItmeString.size() == ITEM_COUNT ? true : false;
	}
	//-------------------------------------------------------------------------------------------------------
	const std::wstring& ShopHelper::GetString(int index)
	{
		ASSERT( index < ITEM_COUNT );
		int i = 0 ;
		for ( ItemMap::iterator it = m_ItmeString.begin();
			it != m_ItmeString.end();
			it ++)
		{
			if( index == i )
			{
				return it->second;
			}
			i++;
		}
		return m_ItmeString.begin()->second;
	}
	//-------------------------------------------------------------------------------------------------------
	void ShopHelper::BuyItem(int index)
	{
		//购买的时候检查本地是否还有没处理的账单，有的话，先处理这个
		if( !this->CheckReceipt()
			&& !m_isIAPProcess)//且不能有正在处理的IAP
		{
			m_isIAPProcess = true;
			IAPBuyItem(index);
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void ShopHelper::Request()
	{
		if( false == m_isLoadDone )
		{
			m_isLoadDone = false;
			m_ItmeString.clear();
			RefreshItem();
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void ShopHelper::PaymentTransactionStatePurchased( const std::string& receipt)
	{
		m_Recepit = Base64::Encode( receipt );;//加密订单
		Serializer ser( IOINSTANCE.WriteFolder() + TempRecepitName );
		ser<<m_Recepit;//保存加密后的订单
		this->SendReceipt( m_Recepit );
	}
	//-------------------------------------------------------------------------------------------------------
	void ShopHelper::PaymentTransactionStateFailed()
	{
		//直接通知付款失败
		m_isIAPProcess = false;
		m_OnReceiptResult.Multicast(false);
	}
	//-------------------------------------------------------------------------------------------------------
	void ShopHelper::ShopServerStatus(Engine& engine)
	{
		m_isResend = true;
		//m_Package.Begin();
		//m_Package<<NM_RECEIPT_STATUS<<SERVER_VISION<<NetContent::Instance().UDID();
		//m_pReceiptClient->Send( m_Package, true );
		engine.GetNet()->Package().Begin();
		engine.GetNet()->Package()<<NM_RECEIPT_STATUS<<SERVER_VISION<<NetHelper::Instance().UDID();
		engine.GetNet()->TCPSend( engine.GetNet()->Package() );
	}
	//-------------------------------------------------------------------------------------------------------
	bool ShopHelper::ListenReceipt(TCPCorrespondent& sender, NetPackage& pak)
	{
		//处理订单
		//static uint Times = 0;
		//if( m_isSendedReceipt //必须发过订单，才重复
		//	&& !m_isServerRecevr )//服务器必须没有收到订单
		//{
		//	Times += timespan;
		//	if( Times >= 10000 )//10秒一次自动重复发送订单
		//	{
		//		DEBUGLOG("Receipt resend to service !\n");
		//		Times = 0;
		//		this->SendReceipt( m_Recepit );
		//	}
		//}
		//static uint StatusTime = 0;
		//if ( m_isResend )
		//{
		//	StatusTime += timespan;
		//	if( StatusTime >= 5000 )//5秒一次自动重复发送
		//	{
		//		DEBUGLOG("StatusTime resend to service !\n");
		//		StatusTime = 0;
		//		this->ShopServerStatus( );
		//	}
		//}
		//if( 0 != m_pReceiptClient->Listen( m_Package ) )
		//{
		//	m_isSendedReceipt = false;
		//	m_Package.Begin();
		//	int message;
		//	m_Package>>message;
		//	switch( message )
		//	{
		//	case NM_RECEIPT_RECEVED:
		//		{
		//			m_isServerRecevr = true;
		//		}break;
		//	case NM_RECEIPT_STATUS:
		//		{
		//			bool isMatch;
		//			m_Package>>isMatch;
		//			if( isMatch )
		//			{
		//				bool isExites;
		//				m_Package>>isExites;
		//				if( isExites )
		//				{
		//					m_ShopStatus = NORMAL;
		//					this->CheckReceipt();
		//				}
		//				else
		//				{
		//					m_ShopStatus = NEW_USER;
		//				}
		//			}
		//			else
		//			{
		//				m_ShopStatus = VERSTION_NOT_MATCH;
		//			}
		//			m_isRecevCustomData = true;
		//			m_isResend = false;
		//			StatusTime = 0;
		//		}break;
		//	case NM_RECEIPT_RESULT:
		//		{
		//			bool isAppleRespones;
		//			bool isReal;
		//			m_Package>>isAppleRespones>>isReal;
		//			if( isAppleRespones )//如果验证了删除这个订单
		//			{
		//				File::Instance().DeleteLocalFile( File::Instance().WritePath() + TempRecepitName );
		//			}
		//			m_OnReceiptResult.Multicast(isReal);//如果有监听者，通知监听者处理结果
		//			m_isIAPProcess = false;
		//		}break;
		//	}
		//}
		m_isSendedReceipt = false;
		pak.Begin();
		int message;
		pak>>message;
		switch( message )
		{
		case NM_RECEIPT_RECEVED:
			{
				m_isServerRecevr = true;
			}break;
		case NM_RECEIPT_STATUS:
			{
				bool isMatch;
				pak>>isMatch;
				if( isMatch )
				{
					bool isExites;
					pak>>isExites;
					if( isExites )
					{
						m_ShopStatus = NORMAL;
						this->CheckReceipt();
					}
					else
					{
						m_ShopStatus = NEW_USER;
					}
				}
				else
				{
					m_ShopStatus = VERSTION_NOT_MATCH;
				}
				m_isRecevCustomData = true;
				m_isResend = false;
			}break;
		case NM_RECEIPT_RESULT:
			{
				bool isAppleRespones;
				bool isReal;
				pak>>isAppleRespones>>isReal;
				if( isAppleRespones )//如果验证了删除这个订单
				{
					IOINSTANCE.DeleteLocalFile( IOINSTANCE.WriteFolder() + TempRecepitName );
				}
				m_OnReceiptResult.Multicast(isReal);//如果有监听者，通知监听者处理结果
				m_isIAPProcess = false;
			}break;
		}
		return false;
	}
	//-------------------------------------------------------------------------------------------------------
	void ShopHelper::SendReceipt(const std::string& receiptEncode)
	{
		m_isServerRecevr = false;
		m_isSendedReceipt = true;
		int eachUDPdataszie = BUFFER_SIZE //总数据段长度
			- NetPackage::Sizeof( NetHelper::Instance().UDID() )//udid随需要的数据段长度
			- sizeof(short)//总包数
			- sizeof(short)//分包数
			- sizeof(int)//需要的数据断的大小
			- sizeof(NM_RECEIPT_DATA);//数据类型
		ushort countPacket = (ushort)ceil( static_cast<float>(receiptEncode.size()) / eachUDPdataszie );//需要的UDP包的数量
		//NetPackage package;
		for ( ushort i = 0 ; i < countPacket; i ++ )
		{
			//package.Begin();
			Engine::Instance().GetNet()->Package().Begin();
			string data;
			if( ( receiptEncode.size() - eachUDPdataszie * i ) > eachUDPdataszie )
			{//发送eachUDPdataszie大小数据
				data = receiptEncode.substr( eachUDPdataszie * i, eachUDPdataszie );
			}
			else
			{
				data = receiptEncode.substr( eachUDPdataszie * i, receiptEncode.size() - eachUDPdataszie * i );
			}
			//package<<NM_RECEIPT_DATA<<NetHelper::Instance().UDID()<<countPacket<<i<<(int)data.size();
			Engine::Instance().GetNet()->Package()<<NM_RECEIPT_DATA<<NetHelper::Instance().UDID()<<countPacket<<i<<(int)data.size();
			for ( int j = 0; j < data.size() ; j++ )//保存单独的字符串，不能使用<<string应为这个最后要自动添加一个0作为终止符，分包不需要
			{
				Engine::Instance().GetNet()->Package()<< data[j];
			}
			//m_pReceiptClient->SendUndropData( package, false );
			Engine::Instance().GetNet()->TCPSend( Engine::Instance().GetNet()->Package() );
		}
	}
}