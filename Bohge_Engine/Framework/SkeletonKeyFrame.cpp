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
#include "SkeletonKeyFrame.h"
#include "Serializer.h"



using namespace eastl;
namespace BohgeEngine
{
	RTTI_TYPE_IMPLEMENT( SkeletonKeyFrame );
	//---------------------------------------------------------------------------------------------------------
	SkeletonKeyFrame::SkeletonKeyFrame()
		:m_FrameBging(0),
		m_FrameEnd(0),
		m_PlayBegin(0),
		m_PlayEnd(0),
		m_Rate(0),
		m_LastFrame(0.0),
		m_CurrentFrame(0.0),
		m_isLoop(false),
		m_isPlayAnimation(false)
	{

	}
	//---------------------------------------------------------------------------------------------------------
	SkeletonKeyFrame::~SkeletonKeyFrame()
	{

	}
	//---------------------------------------------------------------------------------------------------------
	void SkeletonKeyFrame::SetupAnimation( int begin, int end, int rate )
	{
		m_FrameBging = begin;
		m_FrameEnd = end;
		m_Rate = rate;
		m_PlayBegin = m_FrameBging;
		m_PlayEnd = m_FrameEnd;
	}
	//---------------------------------------------------------------------------------------------------------
	void SkeletonKeyFrame::SetupBoneCount( int count )
	{
		m_BoneKeyFrames.resize(count);
	}
	//---------------------------------------------------------------------------------------------------------
	void SkeletonKeyFrame::SetupKeyFrame( int boneindex, const eastl::string& name, int framecount )
	{
		m_BoneKeyFrames[boneindex].m_Name = name;
		m_BoneKeyFrames[boneindex].m_QuaternionArray.resize(framecount);
	}
	//---------------------------------------------------------------------------------------------------------
	void SkeletonKeyFrame::SetupFrameData( int boneindex, int frameindex, const Quaternionf& real, const Quaternionf& dual )
	{
		m_BoneKeyFrames[boneindex].m_QuaternionArray[frameindex].first = real;
		m_BoneKeyFrames[boneindex].m_QuaternionArray[frameindex].second = dual;
	}
	//---------------------------------------------------------------------------------------------------------
	int SkeletonKeyFrame::GetIndex(const string& jointName)
	{
		for ( int i = 0; i < m_BoneKeyFrames.size(); i ++)
		{
			if( jointName == m_BoneKeyFrames[i].m_Name )
			{
				return i;
			}
		}
		ASSERT(false);//不能存在骨骼在动画关键桢中不存在
		return -1;
	}
	//---------------------------------------------------------------------------------------------------------
	void SkeletonKeyFrame::SetTime(float sec)
	{
		if( m_isLoop )
		{
			m_LastFrame = m_CurrentFrame;
			float IncreaseFrame = sec * m_Rate;
			m_CurrentFrame += IncreaseFrame;
			if( m_CurrentFrame >= static_cast<float>( m_PlayEnd - m_FrameBging ) ) //如果超过最大值
			{
				m_CurrentFrame = m_PlayBegin 
					+ static_cast<int>( m_CurrentFrame ) % ( m_PlayEnd  - m_FrameBging )
					+ m_CurrentFrame - static_cast<int>( m_CurrentFrame );
			}
		}
		else
		{
			if( m_CurrentFrame < static_cast<float>( m_PlayEnd  - m_FrameBging - 1 ) )
			{
				m_LastFrame = m_CurrentFrame;
				float IncreaseFrame = sec * m_Rate;
				m_CurrentFrame += IncreaseFrame;
			}
			if( m_CurrentFrame >= static_cast<float>( m_PlayEnd - m_FrameBging - 1 ) ) //如果超过最大值
			{
				m_CurrentFrame = m_PlayEnd  - m_FrameBging - 1;
			}
		}
	}
	//---------------------------------------------------------------------------------------------------------
	pair<Quaternionf, Quaternionf> SkeletonKeyFrame::GetFrame(int jointIndex)
	{
		int floor = static_cast<int>(m_CurrentFrame);
		float weight = m_CurrentFrame - floor;
		int ceili = floor + 1;
		//if ( ceili >= m_FrameEnd )
		//{
		//	ceili = m_FrameBging;
		//}
		if ( ceili >= m_PlayEnd - m_FrameBging )
		{
			ceili = m_PlayBegin;
		}
		pair<Quaternionf, Quaternionf>& FloorQuat = m_BoneKeyFrames[jointIndex].m_QuaternionArray[floor];
		pair<Quaternionf, Quaternionf>&  CeilQuate = m_BoneKeyFrames[jointIndex].m_QuaternionArray[ceili];
		return Math::Sclerp(FloorQuat.first, FloorQuat.second, CeilQuate.first, CeilQuate.second, weight);
	}
	//---------------------------------------------------------------------------------------------------------
	vector< string > SkeletonKeyFrame::GetJointNameList()
	{
		vector< string > nameVer;
		for ( int i = 0; i < m_BoneKeyFrames.size(); i ++)
		{
			nameVer.push_back( m_BoneKeyFrames[i].m_Name );
		}
		return nameVer;
	}
}
