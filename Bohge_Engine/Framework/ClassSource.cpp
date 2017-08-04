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
#include "IClassResource.h"
#include "IScriptSystem.h"
#include "ClassSource.h"
#include "IMetadata.h"


#include <EASTL/string.h>


using namespace eastl;
namespace BohgeEngine
{
	//------------------------------------------------------------------------------------------------------
	ClassSource::ClassSource(void)
		:ISharedSource( IResourceProperty::SST_SCRIPT, IResourceProperty::DispatchThread(-1), true )
	{
	}
	//------------------------------------------------------------------------------------------------------
	ClassSource::~ClassSource(void)
	{
	}
	//------------------------------------------------------------------------------------------------------
	void ClassSource::_DoLoadResource()
	{
		(*GetSourceMetadata().begin())->ProcessMetadata();
		ScriptProperty::ScriptData* data = (*GetSourceMetadata().begin())->GetMetadata<ScriptProperty::ScriptData>();
#ifdef _DEBUG
		m_FileName = data->m_Path;
#endif
		m_pResource = IScriptSystem::Instance()->LoadScript( data->m_Thread, data->m_Path );
		switch( data->m_Thread )
		{
		case ScriptProperty::ST_LOGIC: SetDispatchThread( IResourceProperty::DT_LOGIC_THREAD ); break;
		case ScriptProperty::ST_RENDER: SetDispatchThread( IResourceProperty::DT_RENDER_THREAD ); break;
		default:ASSERT(false);
		}
		m_pResource->SetScriptHash( GetHashCode() );
	}
	//------------------------------------------------------------------------------------------------------
	IClassResource* ClassSource::MakeResource()
	{
		m_pResource->Initialize();
		return m_pResource;
	}
	//------------------------------------------------------------------------------------------------------
	void ClassSource::_DoReleaseUnnecessaryDataAfterLoaded()
	{

	}
}