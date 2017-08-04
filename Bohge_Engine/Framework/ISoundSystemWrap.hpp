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
#pragma once
#include "FunctionRegister.h"
#include "IScriptSystem.h"
#include "ClassRegister.h"
#include "ScriptValue.h"
#include "ISoundComponent.h"
#include "ISoundSystem.h"


namespace BohgeEngine
{
	//-------------------------------------------------------------------------------------------------------
	static void ISoundSystem_Cleaner( Object* rec, Object* obj )
	{
		static_cast< ISoundSystem* >(rec)->UnloadSound( static_cast<ISoundComponent*>( obj ) );
	}
	//-------------------------------------------------------------------------------------------------------
	static void ISoundSystem_LoadSound( ScriptValue* res, ISoundSystem* sys, char* path )
	{
		ISoundComponent* sound = sys->LoadSound( path );
		res->SetValue( sound, sys, &ISoundSystem_Cleaner );
	}
	//-------------------------------------------------------------------------------------------------------
	static void ISoundSystem_SetGlobalVolume( ISoundSystem* sys, float* volume )
	{
		sys->SetGlobalVolume( *volume );
	}
	//-------------------------------------------------------------------------------------------------------
	static void ISoundSystem_SetGlobalPitch( ISoundSystem* sys, float* pitch )
	{
		sys->SetGlobalPitch( *pitch );
	}
	//-------------------------------------------------------------------------------------------------------
	static void ISoundSystem_SetGlobalMute( ISoundSystem* sys, bool* mute )
	{
		sys->SetGLobalMute( *mute );
	}
	//-------------------------------------------------------------------------------------------------------
	static void ISoundSystem_GetGlobalVolume( float* res, ISoundSystem* sys )
	{
		*res = sys->GetGlobalVolume();
	}
	//-------------------------------------------------------------------------------------------------------
	static void ISoundSystem_GetGlobalPitch( float* res, ISoundSystem* sys )
	{
		*res = sys->GetGlobalPitch();
	}
	//-------------------------------------------------------------------------------------------------------
	static void ISoundSystem_isGlobalMute( bool* res, ISoundSystem* sys )
	{
		*res = sys->isGlobalMute();
	}
	//-------------------------------------------------------------------------------------------------------
	static void Register_ISoundSystem( IScriptSystem& sys )
	{
		FunctionRegister sf1;
		sf1.Result( sys, &ISoundComponent::RTTI() );
		sf1.Arguments( sys, &String::RTTI() );
		sf1.Function( &ISoundSystem_LoadSound, "LoadSound" );

		FunctionRegister sf3;
		sf3.Arguments( sys, &Number::RTTI() );
		sf3.Function( &ISoundSystem_SetGlobalVolume, "SetGlobalVolume" );

		FunctionRegister sf4;
		sf4.Arguments( sys, &Number::RTTI() );
		sf4.Function( &ISoundSystem_SetGlobalPitch, "SetGlobalPitch" );

		FunctionRegister sf5;
		sf5.Arguments( sys, &Bool::RTTI() );
		sf5.Function( &ISoundSystem_SetGlobalMute, "SetGlobalMute" );
		
		FunctionRegister sf6;
		sf6.Result( sys, &Number::RTTI() );
		sf6.Function( &ISoundSystem_GetGlobalVolume, "GetGlobalVolume" );

		FunctionRegister sf7;
		sf7.Result( sys, &Number::RTTI() );
		sf7.Function( &ISoundSystem_GetGlobalPitch, "GetGlobalPitch" );

		FunctionRegister sf8;
		sf8.Result( sys, &Bool::RTTI() );
		sf8.Function( &ISoundSystem_isGlobalMute, "isGlobalMute" );

		ClassRegister sc;
		sc.ClassType( &ISoundSystem::RTTI() );
		sc.ClassFunction( sf1 );
		sc.ClassFunction( sf3 );
		sc.ClassFunction( sf4 );
		sc.ClassFunction( sf5 );
		sc.ClassFunction( sf6 );
		sc.ClassFunction( sf7 );
		sc.ClassFunction( sf8 );

		sys.RegisterClass(sc);
	}
}