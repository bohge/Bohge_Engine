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
#include "IThreadSystem.h"
#include "IThreadEntity.h"

namespace BohgeEngine
{
	//-------------------------------------------------------------------------------------------------------
	static void IThreadSystem_Cleaner( Object* rec, Object* obj )
	{
		static_cast< IThreadSystem* >(rec)->RecycleBin( static_cast<IThreadEntity*>( obj ) );
	}
	//-------------------------------------------------------------------------------------------------------
	static void IThreadSystemThreadEntityFactory( ScriptValue* res, IThreadSystem* sys, float* param )
	{
		IThreadEntity* thread = sys->ThreadEntityFactory( static_cast<ThreadProperty::EntityType>( (int)(*param) ) );
		res->SetValue( thread, sys, &IThreadSystem_Cleaner );
	}
	//-------------------------------------------------------------------------------------------------------
	static void Register_IThreadSystem( IScriptSystem& sys )
	{
		{
			FunctionRegister sf;
			sf.Result( sys, &IThreadEntity::RTTI() );
			sf.Arguments( sys, &Number::RTTI() );
			sf.Function( &BohgeEngine::IThreadSystemThreadEntityFactory, "ThreadEntityFactory" );

			ClassRegister sc;
			sc.ClassType( &IThreadSystem::RTTI() );
			sc.ClassFunction( sf );

			sys.RegisterClass(sc);
		}
		{
			ClassRegister sc;
			sc.ClassType( &ThreadProperty::RTTI() );
			sc.ClassConstant( ThreadProperty::ET_MUTEX, "ET_MUTEX" );
			sc.ClassConstant( ThreadProperty::ET_RWLOCK, "ET_RWLOCK" );
			sc.ClassConstant( ThreadProperty::ET_EVENT, "ET_EVENT" );
			sc.ClassConstant( ThreadProperty::ET_JOB_DISPATCHER, "ET_JOB_DISPATCHER" );
			
			sys.RegisterClass(sc);
		}
	}
}