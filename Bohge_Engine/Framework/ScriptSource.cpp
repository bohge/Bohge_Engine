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
#include "IScriptResource.h"
#include "IScriptSystem.h"
#include "ScriptSource.h"
#include "ClassEntity.h"
#include "IMetadata.h"


#include <EASTL/string.h>


using namespace eastl;
namespace BohgeEngine
{
	//------------------------------------------------------------------------------------------------------
	ScriptSource::ScriptSource(void)
		:IMonopolySource( IResourceProperty::DispatchThread(-1), false ),
		m_pClassEntity(NEW ClassEntity)
	{
	}
	//------------------------------------------------------------------------------------------------------
	ScriptSource::~ScriptSource(void)
	{
		SAFE_DELETE( m_pClassEntity );
	}
	//------------------------------------------------------------------------------------------------------
	void ScriptSource::_DoLoadResource()
	{
		m_pClassEntity->DependenceSource( this );
		m_pClassEntity->PushMetadata( **GetSourceMetadata().begin() );
		m_pClassEntity->CreateResource();
		(*GetSourceMetadata().begin())->ProcessMetadata();
		ScriptProperty::ScriptData* data = (*GetSourceMetadata().begin())->GetMetadata<ScriptProperty::ScriptData>();
		m_pResource = IScriptSystem::Instance()->ScriptFactory( data->m_Thread );
		switch( data->m_Thread )
		{
		case ScriptProperty::ST_LOGIC: SetDispatchThread( IResourceProperty::DT_LOGIC_THREAD ); break;
		case ScriptProperty::ST_RENDER: SetDispatchThread( IResourceProperty::DT_RENDER_THREAD ); break;
		default:ASSERT(false);
		}
		m_pResource->SetScriptThread( data->m_Thread );
		m_pResource->SetScriptHash( GetHashCode() );
	}
	//------------------------------------------------------------------------------------------------------
	IMonopolyResource* ScriptSource::MakeResource()
	{
		m_pResource->SetInstanceHandle( m_pClassEntity->GetInstance() );
		return m_pResource;
	}
	//------------------------------------------------------------------------------------------------------
	void ScriptSource::_DoReleaseUnnecessaryDataAfterLoaded()
	{

	}
}