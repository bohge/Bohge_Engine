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
//		Lua脚本管理	   //
/////////////////////////////
#include "ScriptManager.h"
#include "ScriptBase.h"
#include "3DMath.h"
#include "3DMathRegister.h"
#include "CameraRegister.h"
#include "LogicBindRegister.h"
#include "EngineManagerRegister.h"
#include "UIRegister.h"



//重要
//luatinker不支持引用，当遇到引用luatinker会构造以各local变量，这回导致很多问题
//所以在注册函数的时候关键返回类型一定不能是引用

using namespace std;

static Event<bool, const string&>* s_pEvent;

void OnErrorEvent( const char* text )
{
	DEBUGLOG( "%s\n", text );
	s_pEvent->Multicast( text );
}

namespace BohgeEngine
{
	//--------------------------------------------------------------------------------------------------------------------------------------
	ScriptManager::ScriptManager()
	{
		s_pEvent = &m_ErrorCallBack;

		m_SupportedClassesInfo.insert( make_pair( LUALIB_BASE,		ClassInfo( "LUALIB_BASE",		"NULL", static_cast<FuncPtr>(NULL) ) ) );
		m_SupportedClassesInfo.insert( make_pair( LUALIB_COROUTINE, ClassInfo( "LUALIB_COROUTINE",	"NULL", static_cast<FuncPtr>(NULL) ) ) );
		m_SupportedClassesInfo.insert( make_pair( LUALIB_TABLE,		ClassInfo( "LUALIB_TABLE",		"NULL", static_cast<FuncPtr>(NULL) ) ) );
		m_SupportedClassesInfo.insert( make_pair( LUALIB_IO,		ClassInfo( "LUALIB_IO",			"NULL", static_cast<FuncPtr>(NULL) ) ) );
		m_SupportedClassesInfo.insert( make_pair( LUALIB_OS,		ClassInfo( "LUALIB_OS",			"NULL", static_cast<FuncPtr>(NULL) ) ) );
		m_SupportedClassesInfo.insert( make_pair( LUALIB_STRING,	ClassInfo( "LUALIB_STRING",		"NULL", static_cast<FuncPtr>(NULL) ) ) );
		m_SupportedClassesInfo.insert( make_pair( LUALIB_BIT32,		ClassInfo( "LUALIB_BIT32",		"NULL", static_cast<FuncPtr>(NULL) ) ) );
		m_SupportedClassesInfo.insert( make_pair( LUALIB_MATH,		ClassInfo( "LUALIB_MATH",		"NULL", static_cast<FuncPtr>(NULL) ) ) );
		m_SupportedClassesInfo.insert( make_pair( LUALIB_DEBUG,		ClassInfo( "LUALIB_DEBUG",		"NULL", static_cast<FuncPtr>(NULL) ) ) );
		m_SupportedClassesInfo.insert( make_pair( LUALIB_PACKAGE,	ClassInfo( "LUALIB_PACKAGE",	"NULL", static_cast<FuncPtr>(NULL) ) ) );

		//数学
		m_SupportedClassesInfo.insert( make_pair( CLASS_VECTOR2D, ClassInfo( "CLASS_VECTOR2D", "Vector2d", &RegisterVector2T<int> ) ) );
		m_SupportedClassesInfo.insert( make_pair( CLASS_VECTOR3D, ClassInfo( "CLASS_VECTOR3D", "Vector3d", &RegisterVector3T<int> ) ) );
		m_SupportedClassesInfo.insert( make_pair( CLASS_VECTOR4D, ClassInfo( "CLASS_VECTOR4D", "Vector4d", &RegisterVector4T<int> ) ) );
		m_SupportedClassesInfo.insert( make_pair( CLASS_VECTOR2F, ClassInfo( "CLASS_VECTOR2F", "Vector2f", &RegisterVector2T<float> ) ) );
		m_SupportedClassesInfo.insert( make_pair( CLASS_VECTOR3F, ClassInfo( "CLASS_VECTOR3F", "Vector3f", &RegisterVector3T<float> ) ) );
		m_SupportedClassesInfo.insert( make_pair( CLASS_VECTOR4F, ClassInfo( "CLASS_VECTOR4F", "Vector4f", &RegisterVector4T<float> ) ) );
		m_SupportedClassesInfo.insert( make_pair( CLASS_VECTOR2B, ClassInfo( "CLASS_VECTOR2B", "Vector2b", &RegisterVector2T<double> ) ) );
		m_SupportedClassesInfo.insert( make_pair( CLASS_VECTOR3B, ClassInfo( "CLASS_VECTOR3B", "Vector3b", &RegisterVector3T<double> ) ) );
		m_SupportedClassesInfo.insert( make_pair( CLASS_VECTOR4B, ClassInfo( "CLASS_VECTOR4B", "Vector4b", &RegisterVector4T<double> ) ) );
		m_SupportedClassesInfo.insert( make_pair( CLASS_COLOR, ClassInfo( "CLASS_COLOR", "Color", &RegisterColor ) ) );
		m_SupportedClassesInfo.insert( make_pair( CLASS_QUATERNIONF, ClassInfo( "CLASS_QUATERNIONF", "Quaternionf", &RegisterQuaternionT<float> ) ) );
		m_SupportedClassesInfo.insert( make_pair( CLASS_QUATERNIONB, ClassInfo( "CLASS_QUATERNIONB", "Quaternionb", &RegisterQuaternionT<double> ) ) );
		m_SupportedClassesInfo.insert( make_pair( CLASS_MATRIX33F, ClassInfo( "CLASS_MATRIX33F", "Matrix33f", &RegisterMatrix33 ) ) );
		m_SupportedClassesInfo.insert( make_pair( CLASS_MATRIX44F, ClassInfo( "CLASS_MATRIX44F", "Matrix44f", &RegisterMatrix44 ) ) );
		m_SupportedClassesInfo.insert( make_pair( CLASS_PLANEF, ClassInfo( "CLASS_PLANEF", "Planef", &RegisterPlaneT<float> ) ) );
		m_SupportedClassesInfo.insert( make_pair( CLASS_FRUSTUMF, ClassInfo( "CLASS_FRUSTUMF", "Frustumf", &RegisterFrustumT<float> ) ) );
		m_SupportedClassesInfo.insert( make_pair( CLASS_RAYF, ClassInfo( "CLASS_RAYF", "Rayf", &RegisterRayT<float> ) ) );
		m_SupportedClassesInfo.insert( make_pair( CLASS_AABBOXF, ClassInfo( "CLASS_AABBOXF", "Aabbox3df", &RegisterAabboxT<float> ) ) );
		m_SupportedClassesInfo.insert( make_pair( CLASS_AABRECTF, ClassInfo( "CLASS_AABRECTF", "Aabrectf", &RegisterAabrectT<float> ) ) );
		m_SupportedClassesInfo.insert( make_pair( CLASS_AABSPHEREF, ClassInfo( "CLASS_AABSPHEREF", "Aabspheref", &RegisterAabsphereT<float> ) ) );

		//摄像机
		m_SupportedClassesInfo.insert( make_pair( CLASS_ICAMERA, ClassInfo( "CLASS_ICAMERA", "ICamera", &RegisterICamera ) ) );
		m_SupportedClassesInfo.insert( make_pair( CLASS_STATICCAMERA, ClassInfo( "CLASS_STATICCAMERA", "StaticCamera", &RegisterStaticCamera ) ) );
		m_SupportedClassesInfo.insert( make_pair( CLASS_TRACKBALLCAMERA, ClassInfo( "CLASS_TRACKBALLCAMERA", "TrackballCamera", &RegisterTrackballCamera ) ) );
		m_SupportedClassesInfo.insert( make_pair( CLASS_FPSCAMERA, ClassInfo( "CLASS_FPSCAMERA", "FPSCamera", &RegisterFPSCamera ) ) );

		//engine和管理
		m_SupportedClassesInfo.insert( make_pair( CLASS_ENGINE, ClassInfo( "CLASS_ENGINE", "Engine", &RegisterEngine ) ) );
		m_SupportedClassesInfo.insert( make_pair( CLASS_UIMANAGER, ClassInfo( "CLASS_UIMANAGER", "UIManager", &RegisterUIManager ) ) );

		//ui相关
		m_SupportedClassesInfo.insert( make_pair( CLASS_UIGROUP, ClassInfo( "CLASS_UIGROUP", "UIGroup", &RegisterUIGroup ) ) );
		m_SupportedClassesInfo.insert( make_pair( CLASS_UIBASE, ClassInfo( "CLASS_UIBASE", "UIBase", &RegisterUIBase ) ) );
		m_SupportedClassesInfo.insert( make_pair( CLASS_UISTATIC, ClassInfo( "CLASS_UISTATIC", "UIStatic", &RegisterUIStatic ) ) );
		m_SupportedClassesInfo.insert( make_pair( CLASS_UIBUTTON, ClassInfo( "CLASS_UIBUTTON", "UIButton", &RegisterUIButton ) ) );
		m_SupportedClassesInfo.insert( make_pair( CLASS_UISLIDER, ClassInfo( "CLASS_UISLIDER", "UISlider", &RegisterUISlider ) ) );
		m_SupportedClassesInfo.insert( make_pair( CLASS_UILIST, ClassInfo( "CLASS_UILIST", "UIList", &RegisterUIList ) ) );
		m_SupportedClassesInfo.insert( make_pair( CLASS_UIEDIT, ClassInfo( "CLASS_UIEDIT", "UIEdit", &RegisterUIEdit ) ) );
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	ScriptManager::~ScriptManager()
	{
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	void ScriptManager::_SetGlobalClassesIndex( ScriptBase* s )
	{
		for ( SupportedClassesInfo::iterator it = m_SupportedClassesInfo.begin();
			it != m_SupportedClassesInfo.end();
			it ++ )
		{
			s->SetGlobal( it->second.m_IndexName.c_str(), it->first );
		}
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	void ScriptManager::_LoadClasses(  int name, ScriptBase* s )
	{
		switch( name )
		{
		case ScriptManager::LUALIB_BASE:		luaL_requiref( s->LuaState(), "base", luaopen_base, 1 ); break;
		case ScriptManager::LUALIB_COROUTINE:	luaL_requiref( s->LuaState(), "coroutine", luaopen_coroutine, 1 ); break;
		case ScriptManager::LUALIB_TABLE:		luaL_requiref( s->LuaState(), "table", luaopen_table, 1 ); break;
		case ScriptManager::LUALIB_IO:			luaL_requiref( s->LuaState(), "io", luaopen_io, 1 ); break;
		case ScriptManager::LUALIB_OS:			luaL_requiref( s->LuaState(), "os", luaopen_os, 1 ); break;
		case ScriptManager::LUALIB_STRING:		luaL_requiref( s->LuaState(), "string", luaopen_string, 1 ); break;
		case ScriptManager::LUALIB_BIT32:		luaL_requiref( s->LuaState(), "bit32", luaopen_bit32, 1 ); break;
		case ScriptManager::LUALIB_MATH:		luaL_requiref( s->LuaState(), "math", luaopen_math, 1 ); break;
		case ScriptManager::LUALIB_DEBUG:		luaL_requiref( s->LuaState(), "debug", luaopen_debug, 1 ); break;
		case ScriptManager::LUALIB_PACKAGE:		luaL_requiref( s->LuaState(), "package", luaopen_package, 1 ); break;
		default:
			{
				SupportedClassesInfo::iterator it = m_SupportedClassesInfo.find( name );
#ifdef _DEBUG
				ASSERT( it != m_SupportedClassesInfo.end() );
#endif
				(*it->second.m_RegFunction)( s, it->second.m_ClassName.c_str() );
			}
		}
	}
}