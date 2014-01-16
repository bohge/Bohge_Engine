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
//		注册摄像机	   //
/////////////////////////
#pragma once
#include "Camera.h"
#include "ScriptBase.h"


namespace BohgeEngine
{
	static void RegisterICamera( ScriptBase* s, const char* classname )
	{
		//注册一个类，实际是table
		s->RegClass<ICamera >( classname );
		//注册构造函数
		s->RegClassCons<ICamera >();
		//注册类成员变量
		//s->RegClassVar<ICamera >( "a11", &Matrix33f::a11 );
		//注册table成员函数
		s->RegClassFunc< ICamera, void (ICamera::*)(float, float)  >( "CreatePerspectiveProjectionNF", &ICamera::CreatePerspectiveProjection );
		s->RegClassFunc< ICamera, void (ICamera::*)(float, float, float, float)  >( "CreatePerspectiveProjectionRANF", &ICamera::CreatePerspectiveProjection );
		s->RegClassFunc< ICamera, void (ICamera::*)(float, float)  >( "CreateOrthographiProjectionNF", &ICamera::CreateOrthographiProjection );
		s->RegClassFunc< ICamera, void (ICamera::*)(float, float, float, float)  >( "CreateOrthographiProjectionWHNF", &ICamera::CreateOrthographiProjection );
		//s->RegClassFunc< ICamera  >( "Update", &ICamera::Update );//脚本中不要调用
		s->RegClassFunc< ICamera  >( "PickRay", &ICamera::PickRay );
		s->RegClassFunc< ICamera  >( "Move", &ICamera::Move );
		s->RegClassFunc< ICamera  >( "AttachCamera", &ICamera::AttachCamera );
		s->RegClassFunc< ICamera  >( "SetPosition", &ICamera::SetPosition );
		s->RegClassFunc< ICamera  >( "SetForward", &ICamera::SetForward );
		s->RegClassFunc< ICamera  >( "SetMoveRotateZoom", &ICamera::SetMoveRotateZoom );
		s->RegClassFunc< ICamera  >( "isNearFarChange", &ICamera::isNearFarChange );
		s->RegClassFunc< ICamera  >( "MoveSpeed", &ICamera::MoveSpeed );
		s->RegClassFunc< ICamera  >( "RotateSpeed", &ICamera::RotateSpeed );
		s->RegClassFunc< ICamera  >( "ZoomSpeed", &ICamera::ZoomSpeed );
		s->RegClassFunc< ICamera  >( "Fov", &ICamera::Fov );
		s->RegClassFunc< ICamera  >( "Aspect", &ICamera::Aspect );
		s->RegClassFunc< ICamera  >( "LookAt", &ICamera::LookAt );
		s->RegClassFunc< ICamera  >( "GetForward", &ICamera::GetForward );
		s->RegClassFunc< ICamera  >( "GetView", &ICamera::GetView );
		s->RegClassFunc< ICamera  >( "GetProject", &ICamera::GetProject );
		s->RegClassFunc< ICamera  >( "GetViewProj", &ICamera::GetViewProj );
		s->RegClassFunc< ICamera  >( "GetViewProjBias", &ICamera::GetViewProjBias );
		s->RegClassFunc< ICamera  >( "GetPosition", &ICamera::GetPosition );
		s->RegClassFunc< ICamera  >( "GetLookAt", &ICamera::GetLookAt );
		s->RegClassFunc< ICamera  >( "GetUp", &ICamera::GetUp );
		s->RegClassFunc< ICamera  >( "GetNear", &ICamera::GetNear );
		s->RegClassFunc< ICamera  >( "GetFar", &ICamera::GetFar );
		s->RegClassFunc< ICamera  >( "GetMiddle", &ICamera::GetMiddle );
		s->RegClassFunc< ICamera  >( "GetUnViewProj", &ICamera::GetUnViewProj );
		s->RegClassFunc< ICamera  >( "GetUnView", &ICamera::GetUnView );
		s->RegClassFunc< ICamera  >( "GetUnProject", &ICamera::GetUnProject );
		s->RegClassFunc< ICamera  >( "GetFrustum", &ICamera::GetFrustum );
	}

	static void RegisterStaticCamera( ScriptBase* s, const char* classname )
	{
		s->LoadClasses( ScriptManager::CLASS_ICAMERA );
		//注册一个类，实际是table
		s->RegClass<StaticCamera >( classname );
		//注册构造函数
		s->RegClassCons<StaticCamera >();
		//继承
		s->RegClassInh<StaticCamera, ICamera >();
	}


	static void RegisterTrackballCamera( ScriptBase* s, const char* classname )
	{
		s->LoadClasses( ScriptManager::CLASS_ICAMERA );
		//注册一个类，实际是table
		s->RegClass<TrackballCamera >( classname );
		//注册构造函数
		s->RegClassCons<TrackballCamera >();
		//继承
		s->RegClassInh<TrackballCamera, ICamera >();
		//注册table成员函数
		s->RegClassFunc< TrackballCamera >( "Rotate", &TrackballCamera::Rotate );
		s->RegClassFunc< TrackballCamera >( "ZoomInOut", &TrackballCamera::ZoomInOut );
	}


	static void RegisterFPSCamera( ScriptBase* s, const char* classname )
	{
		s->LoadClasses( ScriptManager::CLASS_ICAMERA );
		//注册一个类，实际是table
		s->RegClass<FPSCamera >( classname );
		//注册构造函数
		s->RegClassCons<FPSCamera >();
		//继承
		s->RegClassInh<FPSCamera, ICamera >();
		//注册table成员函数
		s->RegClassFunc< FPSCamera >( "Rotate", &TrackballCamera::Rotate );
		s->RegClassFunc< FPSCamera >( "Move", &TrackballCamera::Move );
	}
}