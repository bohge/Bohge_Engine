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
#include "Framework/ScriptComponent.h"
#include "Application/IApplication.h"
#include "Framework/IWebSystem.h"
#include "Framework/Framework.h"
#include "Framework/IIOSystem.h"
#include "Framework/IFile.h"
#include "DefualtResource.h"
#include "Framework/Node.h"
#include "Framework/Log.h"
#include "Checking.h"
#include "BLiteapp.h"
#include "tinyxml2.h"



using namespace eastl;
using namespace tinyxml2;
using namespace BohgeApp;
using namespace BohgeEngine;
namespace BohgeHub
{
	//---------------------------------------------------------------------------------------------------------
	BLiteapp::BLiteapp(void)
		:m_pRootNode(NULL),
		m_pScriptComponent(NULL),
		m_isQuit(false)
	{
	}
	//---------------------------------------------------------------------------------------------------------
	BLiteapp::~BLiteapp(void)
	{
	}
	//---------------------------------------------------------------------------------------------------------
	void BLiteapp::OpenConfig( const eastl::string& path, const eastl::string& name )
	{
		Framework::Instance()->SetLitePath( path );
		IIOSystem::Instance()->SetLiteappPath( path );
		IFile* file = IIOSystem::Instance()->FileFactory( name );
		file->OpenFile( IFile::AT_READ );
		eastl::string locallif;
		locallif.resize( file->GetSize() );
		file->ReadFile( &locallif[0], file->GetSize() );
		file->CloseFile();
		IIOSystem::Instance()->RecycleBin( file );

		XMLDocument doc;
		doc.Parse( locallif.c_str() );
		XMLElement* entry = doc.FirstChildElement( "Entry" );
		if ( NULL != entry )
		{
			m_EntryScript = entry->GetText();
		}
		XMLElement* Plugins = doc.FirstChildElement( "Plugins" );
		if ( NULL != Plugins )
		{
			XMLElement* Plugin = Plugins->FirstChildElement("Plugin");
			while( Plugin )
			{
				_PushPlugin(
					Plugin->Attribute( "name" ),
					Plugin->Attribute( "library" ));
				Plugin = Plugin->NextSiblingElement();
			}
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void BLiteapp::_OnBeginLaunch( BohgeEngine::Framework& framework )
	{
		DEBUGLOG( "Entry liteapp %s\n", framework.GetConfig().GetLitePath().c_str() );
		m_pRootNode = NEW Node;
		m_pRootNode->InsertIntoManager("Root");
	}
	//---------------------------------------------------------------------------------------------------------
	void BLiteapp::_OnEndLaunch( BohgeEngine::Framework& framework )
	{
		IWebSystem::Instance()->SetCachePath( "lite:WebCache/" );
		m_pScriptComponent = NEW ScriptComponent;
		m_pRootNode->AttachComponent( m_pScriptComponent );
		m_pScriptComponent->SetJobType( IJob::JT_SYNCHRONOUS );
		m_pScriptComponent->SetScriptPath( m_EntryScript );
		m_pScriptComponent->CreateResource();
	}
	//---------------------------------------------------------------------------------------------------------
	void BLiteapp::_OnUpdata( BohgeEngine::Framework& framework )
	{
		if ( m_isQuit )
		{
			BohgeHub::Checking* ret = NEW BohgeHub::Checking;
			ret->SetLiteappUrl( BohgeHub::DefaultResource::HUB_RESOURCE_WEBSITE );
			IApplication::Instance()->SwapLiteapp( ret );
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void BLiteapp::_OnExit( Framework& framework )
	{
		IIOSystem::Instance()->SetLiteappPath( DefaultResource::RESOURCE_PATH );
		m_pRootNode->ReleaseComponent();
		SAFE_DELETE( m_pRootNode );
		SAFE_DELETE( m_pScriptComponent );
	}
	//---------------------------------------------------------------------------------------------------------
	void BLiteapp::Quit()
	{
		m_isQuit = true;
	}

}