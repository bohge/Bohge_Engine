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


namespace BohgeEngine
{
	RTTI_TYPE_IMPLEMENT( SkeletonResource );
	//------------------------------------------------------------------------------------------------------
	SkeletonResource::SkeletonResource(void)
	{
	}
	//------------------------------------------------------------------------------------------------------
	SkeletonResource::~SkeletonResource(void)
	{
	}
	//------------------------------------------------------------------------------------------------------
	void SkeletonResource::ReleaseResource()
	{

	}
	//------------------------------------------------------------------------------------------------------
	void SkeletonResource::SetupSkeletone( uint boneCount, const eastl::vector<SkeletonJoint>& joints, const SkeletonKeyFrame& keyframe )
	{
		m_MaxBoneIndexCount = boneCount;
		m_JointVector = joints;
		m_KeyFrame = keyframe;
	}
	//------------------------------------------------------------------------------------------------------
	void SkeletonResource::_BuildBones( float sec )
	{
		m_KeyFrame.SetTime(sec);
		for ( int i = 0 ; i < m_JointVector.size(); i++ )
		{
			eastl::pair<Quaternionf, Quaternionf> kf;
			kf = m_KeyFrame.GetFrame( m_JointVector[i].m_KeyFrameIndex );
			int parent =   m_JointVector[i].m_Parent;
			if( -1 != parent )
			{
				if( kf.first.Dot( m_JointVector[ parent ].m_RealBind ) < 0 )
				{
					kf.first = -kf.first;
					kf.second = -kf.second;
				}
				m_JointVector[i].m_RealBind = kf.first * m_JointVector[parent].m_RealBind;
				m_JointVector[i].m_DaulBind = kf.first * m_JointVector[parent].m_DaulBind + kf.second * m_JointVector[parent].m_RealBind;
			}
			else
			{
				m_JointVector[i].m_RealBind = kf.first;
				m_JointVector[i].m_DaulBind = kf.second;
			}
		}
		for ( int i = 0 ; i < m_JointVector.size(); i++ )//乘最原始的逆矩阵,构造渲染用四元数
		{
			m_AnimationDual.first.Set( i, m_JointVector[i].m_InversPair.first * m_JointVector[i].m_RealBind );
			m_AnimationDual.second.Set( i, m_JointVector[i].m_InversPair.first * m_JointVector[i].m_DaulBind + m_JointVector[i].m_InversPair.second * m_JointVector[i].m_RealBind );
		}
	}

}