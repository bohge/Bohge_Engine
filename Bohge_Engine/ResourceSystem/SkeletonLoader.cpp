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
#include "Framework/IIOSystem.h"
#include "Framework/IFile.h"
#include "SkeletonLoader.h"

using namespace BohgeEngine;
namespace ResourceSystemPlugin
{
	//------------------------------------------------------------------------------------------------------
	SkeletonLoader::SkeletonLoader(void)
		:ILoader( IResourceProperty::SLT_SKELETON )
	{
	}
	//------------------------------------------------------------------------------------------------------
	SkeletonLoader::~SkeletonLoader(void)
	{
	}
	//------------------------------------------------------------------------------------------------------
	void* SkeletonLoader::_DoLoad( const eastl::string& path )
	{
		Serializer loader( path );
		for ( bool isDone = false; !isDone ; )
		{
			int chunk = -1;
			loader>>chunk;//读取chunk
			switch ( chunk )
			{
			case static_cast<int>(SkeletonProperty::SC_JOINT) : _LoadBones(loader); break;
			case static_cast<int>(SkeletonProperty::SC_KEY_FRAME) : _LoadKeyFrame(loader); break;
			case static_cast<int>(SkeletonProperty::SC_SKELETON_END) : isDone = true; break;
			}
		}
		return NULL;
	}
	//------------------------------------------------------------------------------------------------------
	void SkeletonLoader::_LoadBones( BohgeEngine::Serializer& file )
	{
		int size;
		file>>m_SourceData.m_MaxBoneIndexCount>>size;//得到包含骨骼的数量
		m_SourceData.m_JointVector.resize(size);
		for( int i = 0; i < size ; i ++ )
		{
			file>>m_SourceData.m_JointVector[i].m_Name>>m_SourceData.m_JointVector[i].m_Parent;//读取joint名称和父ID
			file>>m_SourceData.m_JointVector[i].m_RealBind.x()
				>>m_SourceData.m_JointVector[i].m_RealBind.y()
				>>m_SourceData.m_JointVector[i].m_RealBind.z()
				>>m_SourceData.m_JointVector[i].m_RealBind.w();
			file>>m_SourceData.m_JointVector[i].m_DaulBind.x()
				>>m_SourceData.m_JointVector[i].m_DaulBind.y()
				>>m_SourceData.m_JointVector[i].m_DaulBind.z()
				>>m_SourceData.m_JointVector[i].m_DaulBind.w();
			m_SourceData.m_JointVector[i].m_InversPair = Math::Inverse(m_SourceData.m_JointVector[i].m_RealBind, m_SourceData.m_JointVector[i].m_DaulBind);
		}
	}
	//------------------------------------------------------------------------------------------------------
	void SkeletonLoader::_LoadKeyFrame( BohgeEngine::Serializer& file )
	{
		{
			int fb,fe,rate;
			file>>fb>>fe>>rate;//读取开始帧，结束帧，帧率
			m_SourceData.m_KeyFrame.SetupAnimation( fb, fe, rate );
		}
		int BoneCount = 0;
		file>>BoneCount;
		m_SourceData.m_KeyFrame.SetupBoneCount( BoneCount );
		for (int i = 0 ; i < BoneCount; i ++ )
		{
			eastl::string name;
			int frameCount = 0 ;
			file>>name>>frameCount;//读取当前骨骼包含的key个数
			m_SourceData.m_KeyFrame.SetupKeyFrame( i, name, frameCount );
			for (int j = 0 ; j < frameCount ; j ++)
			{
				Quaternionf real,dual;
				file>>real.x() >>real.y() >>real.z() >>real.w();
				file>>dual.x() >>dual.y() >>dual.z() >>dual.w();
				m_SourceData.m_KeyFrame.SetupFrameData( i, j, real, dual );
			}
		}
	}
}
