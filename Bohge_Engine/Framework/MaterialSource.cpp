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
#include "MaterialSource.h"
#include "MaterialResource.h"
#include "PipelineProperty.h"
#include "IMaterialSystem.h"
#include "IGraphicSystem.h"
#include "ShaderEntity.h"
#include "RenderState.h"
#include "IIOSystem.h"
#include "Utility.hpp"
#include "IFile.h"


#include <algorithm>
#include <EASTL/string.h>
#include <EASTL/vector.h>
#include <EASTL/list.h>

using namespace eastl;



namespace BohgeEngine
{
	//---------------------------------------------------------------------------------------------
	MaterialSource::MaterialSource(void)
		:ISharedSource( IResourceProperty::SST_MATERIAL, IResourceProperty::DT_RENDER_THREAD, true )
	{
		memset( m_RenderState, NULL, sizeof(m_RenderState) );
		memset( m_ShaderEntity, NULL, sizeof(m_ShaderEntity) );
	}
	//---------------------------------------------------------------------------------------------
	MaterialSource::~MaterialSource(void)
	{
		const int passCount = IMaterialSystem::Instance()->GetActivePassTypeCount();
		for ( int i = 0 ; i < passCount ; ++ i )
		{
			PipelineProperty::PassType pt = IMaterialSystem::Instance()->GetPassType( i );
			SAFE_DELETE( m_RenderState[pt] );
			SAFE_DELETE( m_ShaderEntity[pt] );
		}
	}
	//---------------------------------------------------------------------------------------------
	void MaterialSource::_DoLoadResource()
	{
		string materialPath;
		string* path = (*GetSourceMetadata().begin())->GetMetadata<string>();
		materialPath = path->substr( 0, path->find_last_of("/\\") + 1 );
		IFile* readfile = IIOSystem::Instance()->FileFactory( *path );
		readfile->OpenFile( IFile::AT_READ );
		char* datas = NEW char[readfile->GetSize()+1];
		readfile->ReadFile( datas, readfile->GetSize() );
		datas[readfile->GetSize()] = 0;
		readfile->CloseFile();
		IIOSystem::Instance()->RecycleBin( readfile );
		string source = datas;
		SAFE_DELETE_ARRAY(datas);
		Utility::RemoveComment( "//", source );
		//处理include
		list<string> includes;
		Utility::FindIncludes( includes, source );//得到includ文件
		PipelineProperty::ActivePassTypeArray apta = IMaterialSystem::Instance()->ParseMaterial( *path, source );
		FOR_EACH( PipelineProperty::ActivePassTypeArray, pass, apta )
		{
			const PipelineProperty::PassType pt = static_cast<PipelineProperty::PassType>( *pass );
			m_RenderState[pt] = NEW RenderState();
			m_ShaderEntity[pt] = NEW ShaderEntity();
			IMaterialSystem::Instance()->FlushRenderState( pt, *m_RenderState[pt] );
			m_RenderState[pt]->CaculateHashCode();//计算哈希值

			//MaterialProperty::TextureSlotPropertyMap slots =
			//	MaterialParser::Instance().GetTextureSlotStatus( static_cast<Pipeline::PassType>(i) );
			//m_TextureSlotMap.insert( slots.begin(), slots.end() );
			{//加载shader
#ifdef _DEBUG
				m_ShaderEntity[pt]->SetShaderName( IMaterialSystem::Instance()->GetPassShaderName( pt ) );
#endif
				m_ShaderEntity[pt]->SetIncludePath( materialPath );
				FOR_EACH( list<string>, file, includes )
				{
					m_ShaderEntity[pt]->PushInclude( *file );
				}
				vector<string> names =
					IMaterialSystem::Instance()->GetVertexSegmentNames( pt );
				FOR_EACH( vector<string>, name, names )
				{
					m_ShaderEntity[pt]->PushVertexSegmentName( *name );
				}
				names =	IMaterialSystem::Instance()->GetFragmentegmentNames( pt );
				FOR_EACH( vector<string>, name, names )
				{
					m_ShaderEntity[pt]->PushFragmentSegmentName( *name );
				}
				m_ShaderEntity[pt]->DependenceSource( this );
			}
		}
		for ( int passtype = 0 ; passtype < PipelineProperty::PT_PASS_TYPE_COUNT ; ++ passtype )
		{
			if ( NULL != m_ShaderEntity[passtype] )
			{
				m_ShaderEntity[passtype]->CreateResource();
			}
		}
	}
	//---------------------------------------------------------------------------------------------
	ISharedResource* MaterialSource::MakeResource()
	{
		MaterialResource* mr = static_cast<MaterialResource*>( IGraphicSystem::Instance()->CreateResource( IResourceProperty::SRT_MATERIAL ) );
		//mr->SetTextureProperty( &m_TextureSlotMap );

		const int passCount = IMaterialSystem::Instance()->GetActivePassTypeCount();
		for ( int i = 0 ; i < passCount ; ++ i )
		{
			PipelineProperty::PassType pt = IMaterialSystem::Instance()->GetPassType( i );
			mr->SetRenderState( pt, m_RenderState[pt] );
			mr->SetShaderEntity( pt, m_ShaderEntity[pt] );
		}
		return mr;
	}
	//---------------------------------------------------------------------------------------------
	void MaterialSource::_DoReleaseUnnecessaryDataAfterLoaded()
	{

	}

}