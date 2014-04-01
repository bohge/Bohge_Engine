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



/////////////////////////////////
//			材质合成系统		   //
/////////////////////////////////

#include "MaterialManager.h"
#include "IOSystem.h"
#include "Utility.h"
#include "Material.h"
#include "IFile.h"
#include "Utility.h"
#include "MaterialCompiler.h"




#include <string>
#include <vector>

#define BOHGE_MATERIAL_EXPAND ".Material"
using namespace std;


namespace BohgeEngine
{
	//-------------------------------------------------------------------------------------------------------
	MaterialManager::MaterialManager(void)
	{
		m_pMaterialCompiler = NEW MaterialCompiler;
	}
	//-------------------------------------------------------------------------------------------------------
	MaterialManager::~MaterialManager(void)
	{
		SAFE_DELETE( m_pMaterialCompiler );
		FOR_EACH( MaterialMap, it, m_GeneratedMaterial )
		{
			SAFE_DELETE( it->second );
		}
	}
	//-------------------------------------------------------------------------------------------------------
	Material* MaterialManager::_LoadMaterial( const std::string& path )
	{
		IFile* readfile = FILEFACTORY( IOINSTANCE.ShaderFolder() + path );
		readfile->OpenFile( IFile::AT_READ );
		char* datas = NEW char[readfile->GetSize()+1];
		readfile->ReadFile( datas, readfile->GetSize() );
		datas[readfile->GetSize()] = 0;
		readfile->CloseFile();
		FILEDESTROY( readfile );
		string source = datas;
		SAFE_DELETE_ARRAY(datas);
		Utility::RemoveComment( "//", source );
		Material* mat = m_pMaterialCompiler->CompilerMaterial( source );
		mat->SetName( Utility::RemoveTargetString( path, BOHGE_MATERIAL_EXPAND ) );
		return mat;
	}
	//-------------------------------------------------------------------------------------------------------
	void MaterialManager::Initialization( )
	{
		m_pMaterialCompiler->Initialization( );
		vector<string> files = IOINSTANCE.GetFileNamesWithExpand( IOINSTANCE.ShaderFolder(), BOHGE_MATERIAL_EXPAND );
		for ( vector<string>::iterator it = files.begin();
			it != files.end();
			it ++ )
		{
			Material* mat = _LoadMaterial( *it );
			m_GeneratedMaterial.insert( make_pair( Utility::HashCode( mat->GetName() ), mat ) );
		}
	}
	//-------------------------------------------------------------------------------------------------------
	const Material* MaterialManager::GetMaterial( const std::string& name )
	{
		MaterialMap::iterator it = m_GeneratedMaterial.find( Utility::HashCode(name) );
		if ( it != m_GeneratedMaterial.end() )
		{
			return it->second;
		}
		return NULL;
	}

}
