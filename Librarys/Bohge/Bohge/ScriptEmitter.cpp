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



/////////////////////////////////////////
//			粒子系统粒子发射器	   //
/////////////////////////////////////////
#include "ScriptEmitter.h"
#include "ScriptInterface.hpp"


namespace BohgeEngine
{
	//--------------------------------------------------------------------------------------------------------------------------------------
	ScriptEmitter::ScriptEmitter(
		const vector3f& pos,
		const vector3f& posNoise,
		const vector3f& dir,
		const vector3f& acce,
		const vector2f& speedPmill,
		float endspeed,
		const vector3f& angle,
		const Color& begin1,
		const Color& begin2,
		const Color& end1,
		const Color& end2,
		const vector2f& clrpow,
		const vector2d& life,
		const vector2f& alphaPow,
		const vector2f& sizebegin,
		const vector2f& sizeend,
		const vector2f& sizepow,
		const vector2d& countSec,
		const int emitterlife,
		std::string texName)
		:Emitter(pos,posNoise,dir,acce,speedPmill,endspeed,angle,begin1,begin2,end1,end2,clrpow,life,alphaPow,sizebegin,sizeend,sizepow,countSec,emitterlife,texName)
	{

	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	void ScriptEmitter::Update( uint millisecond )
	{
		this->CallLua<bool>( FUNCNAME(Script::FN_UPDATA), millisecond );
		Emitter::Update( millisecond );
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	void ScriptEmitter::_OnScriptLoaded()
	{
		this->RegClass<ScriptEmitter>("Emitter");
		//注册继承关系
		this->RegClassInh<ScriptEmitter, ScriptBase>();
		//注册成员函数
		AsIAnimation( this );
		AsILocatable( this );
		//设置全局变量
		SetGlobal( "this", this );//这里实际利用了lua的特性，这个数值已经在ScriptBase中被赋值，这里再次赋值
		this->CallLua<bool>( FUNCNAME(Script::FN_INIT) );//初始化
	}
}