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
#include "SkeletonResource.h"
#include "IResourceSystem.h"
#include "SkeletonSource.h"
#include "IMetadata.h"
#include "ILoader.h"


#include <EASTL/string.h>

using namespace eastl;
namespace BohgeEngine
{
	//------------------------------------------------------------------------------------------------------
	SkeletonSource::SkeletonSource(void)
		:IMonopolySource( IResourceProperty::DT_LOGIC_THREAD, true ),
		m_pSourceData(NULL)
	{
	}
	//------------------------------------------------------------------------------------------------------
	SkeletonSource::~SkeletonSource(void)
	{
	}
	//------------------------------------------------------------------------------------------------------
	void SkeletonSource::_DoLoadResource()
	{
		string* path = (*GetSourceMetadata().begin())->GetMetadata<string>();
		ILoader* loader = IResourceSystem::Instance()->LoaderFactory( IResourceProperty::SLT_SKELETON );
		m_pSourceData = loader->Load<SkeletonProperty::SkeletonSourceData>( *path );
		IResourceSystem::Instance()->RecycleBin( loader );
	}
	//------------------------------------------------------------------------------------------------------
	void SkeletonSource::_DoReleaseUnnecessaryDataAfterLoaded()
	{
		SAFE_DELETE( m_pSourceData );
	}
	//------------------------------------------------------------------------------------------------------
	IMonopolyResource* SkeletonSource::MakeResource()
	{
		SkeletonResource* res = NEW SkeletonResource();
		res->SetupSkeletone( m_pSourceData->m_MaxBoneIndexCount, m_pSourceData->m_JointVector, m_pSourceData->m_KeyFrame );
		return res;
	}

}