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
#pragma once
#include "Emitter.h"


namespace BohgeEngine
{
	//支持脚本的例子发生器
	class ScriptEmitter : public Emitter, public ScriptBase
	{
	public:
		ScriptEmitter(const vector3f& pos,//粒子的发射位置
			const vector3f& posNoise,//粒子的抖动噪点
			const vector3f& dir,//粒子的发生放向
			const vector3f& acce,//粒子的加速度
			const vector2f& speedPmill,//粒子的速度
			float endspeed,//粒子结束速度
			const vector3f& angle,//粒子的角度
			const Color& begin1,//粒子的颜色
			const Color& begin2,
			const Color& end1,
			const Color& end2,
			const vector2f& clrpow,//粒子颜色曲率
			const vector2d& life,//粒子生命时间
			const vector2f& alphaPow,//粒子alpha的曲率
			const vector2f& sizebegin,//粒子尺寸
			const vector2f& sizeend,
			const vector2f& sizepow,//粒子尺寸曲率
			const vector2d& countSec,//每秒喷射
			const int emitterlife,//发射器生命周期
			std::string texName );//纹理
	public:
		virtual void Update(uint millisecond);
	private:
		virtual void _OnScriptLoaded();//当脚本加载后
	public:
		void Lua_StartAnimation() //可触发的逻辑被触发
		{
			Relife();
		}
		void Lua_StopAnimation()//停止
		{
			StopEmit();
		}
	public:
		BOHGE_FORCEINLINE void Lua_SetLocalRotation( const Quaternionf& r )
		{
			SetLocalRotation( r );
		}
		BOHGE_FORCEINLINE void Lua_SetLocalPosition( const vector3f& p )
		{
			SetLocalPosition( p );
		}
		BOHGE_FORCEINLINE void Lua_SetLocalScale( const vector3f& s )
		{
			SetLocalScale( s );
		}
		BOHGE_FORCEINLINE const vector3f& Lua_GetLocalPosition( ) const
		{
			return GetLocalPosition();
		}
		BOHGE_FORCEINLINE const vector3f& Lua_GetLocalScale( ) const
		{
			return GetLocalScale();
		}
		BOHGE_FORCEINLINE const Quaternionf& Lua_GetLocalRotation( ) const
		{
			return GetLocalRotation();
		}
		BOHGE_FORCEINLINE const Matrix44f& Lua_GetWorldTransform() const
		{
			return GetWorldTransform();
		}
		BOHGE_FORCEINLINE const Matrix33f& Lua_GetWorldRotationMatrix() const
		{
			return GetWorldRotationMatrix();
		}
		BOHGE_FORCEINLINE const vector3f& Lua_GetWorldPosition() const
		{
			return GetWorldPosition();
		}
		BOHGE_FORCEINLINE const vector3f& Lua_GetWorldScale() const
		{
			return GetWorldScale();
		}
		BOHGE_FORCEINLINE const Quaternionf& Lua_GetWorldRotation() const
		{
			return GetWorldRotation();
		}
	};
}