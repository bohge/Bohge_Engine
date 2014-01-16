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



/////////////////////////////
//		脚本类的接口	   //
/////////////////////////////
#pragma once
#include "3DMath.h"
#include "ScriptBase.h"

////关于函数接口错误的例子与解释
//void FlushTiggerInterface( ScriptBase* ptr )
//{
//	ptr->RegClass<ITigger>( "ITigger" ); //注册一个类，实际是table
//	ptr->RegClassFunc<ITigger>( "StartTigger", &ITigger::Script_StartTigger );//注册table成员
//	ptr->RegClassFunc<ITigger>( "StopTigger", &ITigger::Script_StopTigger );//注册table成员
//}
//这个例子会导致调用该函数的崩溃
//原因是当c++端掉用lua的时候，由于这个table注册的函数指针是Tigger的函数地址，而这个函数是个虚函数
//c++实现虚函数是一个虚函数表来索引上级的函数地址
//然而我们注册的是ITigger的地址，这个地址指向一个纯虚函数，这样在runtime调用的时候就指向了一个错误的地址
//我们需要改用模板的泛型编程，这样不仅不需要include "ScriptBase.h"
//而且我们注册的函数指针是实际的engine类的函数地址，这样也间接的时lua支持了多态

//重要
//luatinker不支持引用，当遇到引用luatinker会构造以各local变量，这回导致很多问题
//所以在注册函数的时候关键返回类型一定不能是引用

//注册类的继承的需要注册的函数不能使用基类的成员函数，全部的目标函数需要重新填写，
//在函数名前面添加Lua_xxx，具体参见ScriptEmeitter


namespace BohgeEngine
{
	//接口填充
	template< typename T1 >
	void AsIAnimation( T1* ptr )
	{
		ptr->T1::template RegClassFunc< T1 >( "StartAnimation", &T1::Lua_StartAnimation );//注册table成员
		ptr->T1::template RegClassFunc< T1 >( "StopAnimation", &T1::Lua_StopAnimation );//注册table成员
	}

	template< typename T1 >
	void AsILocatable( T1* ptr )
	{
		ptr->T1::template RegClassFunc<T1>( "SetLocalRotation", &T1::Lua_SetLocalRotation );//注册table成员
		ptr->T1::template RegClassFunc<T1>( "SetLocalPosition", &T1::Lua_SetLocalPosition );//注册table成员
		ptr->T1::template RegClassFunc<T1>( "SetLocalScale", &T1::Lua_SetLocalScale );//注册table成员
		ptr->T1::template RegClassFunc<T1>( "GetLocalPosition", &T1::Lua_GetLocalPosition );//注册table成员
		ptr->T1::template RegClassFunc<T1>( "GetLocalScale", &T1::Lua_GetLocalScale );//注册table成员
		ptr->T1::template RegClassFunc<T1>( "GetLocalRotation", &T1::Lua_GetLocalRotation );//注册table成员
		ptr->T1::template RegClassFunc<T1>( "GetWorldPosition", &T1::Lua_GetWorldPosition );//注册table成员
		ptr->T1::template RegClassFunc<T1>( "GetWorldScale", &T1::Lua_GetWorldScale );//注册table成员
		ptr->T1::template RegClassFunc<T1>( "GetWorldRotation", &T1::Lua_GetWorldRotation );//注册table成员
	}


	//取消接口定义，浪费性能
	////接口定义
	//class ITigger //当碰撞或者其他一些时间时被触发
	//{
	//public:
	//	virtual ~ITigger(){}
	//public:
	//	virtual void Script_StartTigger() = 0; //可触发的逻辑被触发
	//	virtual void Script_StopTigger() = 0;//停止
	//};
	//
	//class ILocatable
	//{
	//public:
	//	virtual ~ILocatable(){}
	//public:
	//	virtual void Script_SetRotate( const Quaternionf& r ) = 0;
	//	virtual void Script_SetPosition( const vector3f& p ) = 0;
	//	virtual void Script_SetScale( const vector3f& s ) = 0;
	//	virtual const vector3f& Script_GetPosition( ) = 0;
	//	virtual const vector3f& Script_GetScale( ) = 0;
	//	virtual const Quaternionf& Script_GetRotation( ) = 0;
	//};

}