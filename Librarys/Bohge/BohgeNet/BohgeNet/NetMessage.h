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
//		消息类型	   //
/////////////////////////
#pragma once


namespace BohgeNet
{
	enum SystemMessage
	{
		SM_RECEV_FRAGMENT = 0xffffffff,
	};
	enum NetMessage
	{
		NM_CONNECT_SERVER = 0x80000001, //客户端发送的进入请求
		NM_SERVER_NOTICE,//服务器通告
		NM_CHANGE_LOCATION,//改变服务器地址
		NM_NEW_USER,//您是新用户，需要用户信息
		NM_USER_DATA,//返回的用户信息
		NM_LOBBY_INFO, //请求lobby信息
		NM_NO_GAME_TIMES,//没有游戏时间了，拒绝进入
		NM_LOBBY_PORT, //服务器返还的LOBBY地址
		NM_LOBBY_ENTER,//进入lobby
		NM_LOBBY_ROOM_REFESH,//刷新房间信息
		NM_LOBBY_ROOM_DATA,//进入后服务发出的房间的当前房间的信息
		NM_LOBBY_ROOM_NEW,//广播一个新房间
		NM_LOBBY_ROOM_CLOSE,//需要广播的房间消失消息
		NM_LOBBY_ROOM_ENTER,//进入房间请求
		NM_LOBBY_ROOM_FIND,//查询指定房间
		NM_LOBBY_ROOM_PASSWORD,//要求输入密码
		NM_LOBBY_ROOM_QUICK,//快速开始
		NM_LOBBY_ROOM_ENTER_PASS,//准许进入房间
		NM_LOBBY_ROOM_ENTER_REFUES,//拒绝静茹房间
		NM_ROOM_PORT,//房间服务器端口
		NM_ROOM_ENTER,//进入房间
		NM_ROOM_UDP_ADDRESS_GET,//得到了你的UDP地址
		NM_ROOM_UDP_PUNCHING,//UDP打洞
		NM_ROOM_SUCCESS_PUNCHING,//UDP打洞成功
		NM_ROOM_ENTER_PASS,//进入房间成功
		NM_ROOM_ENTER_REFUES,//进入房间失败
		NM_ROOM_DATA,//发送房间数据
		NM_ROOM_ACTION,//准备或者开始游戏
		NM_ROOM_LEAVE_PASS,//允许客户端退出
		NM_ROOM_HEART,//心跳包
		NM_ROOM_LEAVE,//客户端发给服务器的，信息要具体
		NM_TEXT_CONTENT,//发送文本

		NM_GAME_READY,//复机准备好了
		NM_GAME_BEGIN,//游戏开始
		NM_GAEM_IM_READY,//玩家准备好了
		NM_GAME_IM_START,//对方知道你准备好了，已经开始了
		NM_GAME_STACK_DATA,//板上的数据
		NM_GAME_SKILL_DATA,//技能使用的数据
		NM_GAME_DEAD,//失败了
		NM_GAME_LEAVE,//在游戏中离开了

		NM_RECEIPT_STATUS,//检查客户端是否版本相同，用户是否存在
		NM_RECEIPT_DATA,//收据数据
		NM_RECEIPT_RECEVED,//接收到订单了，不要发送了，等待验证吧
		NM_RECEIPT_RESULT,//服务器的验证结果

		NM_LEAVE,//离开，服务器发给客户端，根据当前状态机决定怎么处理
	};
}