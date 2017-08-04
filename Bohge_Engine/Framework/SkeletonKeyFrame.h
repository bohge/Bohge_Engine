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
#include "3DMath.h"


#include <EASTL/string.h>
#include <EASTL/vector.h>

namespace BohgeEngine
{
	class BOHGE_FRAMEWORK SkeletonKeyFrame	: public Object
	{
		RTTI_TYPE_DECLEAR( SkeletonKeyFrame )
	private:
		struct KeyFrame
		{
			eastl::string											m_Name;//对应骨骼名称
			eastl::vector< eastl::pair<Quaternionf, Quaternionf> >	m_QuaternionArray;//关键帧队列
		private:
			BOHGE_FORCEINLINE void _DoCopy( const KeyFrame& rhs )
			{
				m_Name = rhs.m_Name;
				m_QuaternionArray.resize( rhs.m_QuaternionArray.size() );
				for ( int i = 0 ; i < rhs.m_QuaternionArray.size(); i ++ )
				{
					m_QuaternionArray[i].first = rhs.m_QuaternionArray[i].first;
					m_QuaternionArray[i].second = rhs.m_QuaternionArray[i].second;
				}
			}
		public:
			KeyFrame()
			{

			}
			KeyFrame( const KeyFrame& rhs )
			{
				_DoCopy(rhs);
			}
			KeyFrame& operator = ( const KeyFrame& rhs )
			{
				_DoCopy(rhs);
				return *this;
			}
		};
	private://读入的数据
		int							m_FrameBging;	//开始帧
		int							m_FrameEnd;		//结束帧
		int							m_Rate;			//帧率
		eastl::vector<KeyFrame>		m_BoneKeyFrames;//有关键真的骨骼
	private:
		bool						m_isPlayAnimation;//是否播放动画
		bool						m_isLoop;//是否循环播放
		int							m_PlayBegin;	//需要播放的动画区间
		int							m_PlayEnd;	//需要播放的动画区间
		float						m_LastFrame;//前一帧
		float						m_CurrentFrame;//当前帧
	private:
		BOHGE_FORCEINLINE void _DoCopy( const SkeletonKeyFrame& rhs )
		{
			m_FrameBging = rhs.m_FrameBging;
			m_FrameEnd = rhs.m_FrameEnd;
			m_Rate = rhs.m_Rate;
			m_isPlayAnimation = rhs.m_isPlayAnimation;
			m_isLoop = rhs.m_isLoop;
			m_PlayBegin = rhs.m_PlayBegin;
			m_PlayEnd = rhs.m_PlayEnd;
			m_LastFrame = rhs.m_LastFrame;
			m_CurrentFrame = rhs.m_CurrentFrame;
			m_BoneKeyFrames.resize( rhs.m_BoneKeyFrames.size() );
			for ( int i = 0 ; i < rhs.m_BoneKeyFrames.size(); i ++ )
			{
				m_BoneKeyFrames[i] = rhs.m_BoneKeyFrames[i];
			}
		}
	public:
		SkeletonKeyFrame( const SkeletonKeyFrame& rhs )
		{
			_DoCopy(rhs);
		}
		SkeletonKeyFrame& operator = ( const SkeletonKeyFrame& rhs )
		{
			_DoCopy(rhs);
			return *this;
		}
	private:
		BOHGE_FORCEINLINE bool isPlay()
		{
			return m_isPlayAnimation;
		}
	public:
		SkeletonKeyFrame();
		~SkeletonKeyFrame();
	public://初始化函数
		void SetupAnimation( int begin, int end, int rate );//设置骨骼动画的开始结束帧，帧率
		void SetupBoneCount( int count );//设置骨骼数量
		void SetupKeyFrame( int boneindex, const eastl::string& name, int framecount );
		void SetupFrameData( int boneindex, int frameindex, const Quaternionf& real, const Quaternionf& dual );
	public:
		eastl::pair<Quaternionf, Quaternionf> GetFrame(int jointIndex);
		int	GetIndex(const eastl::string& jointName);
		void SetTime(float sec);
		eastl::vector< eastl::string > GetJointNameList();
	public:
		BOHGE_FORCEINLINE void SetAnimationInterval( int begin, int end)//设置需要播放的帧区间
		{
			ASSERT( begin >= m_FrameBging );
			ASSERT( end <= m_FrameEnd );
			m_PlayBegin = begin;
			m_PlayEnd = end;
		}
		BOHGE_FORCEINLINE void ResetAnimation()
		{
			m_LastFrame = m_CurrentFrame;
			m_CurrentFrame = m_PlayBegin;
		}
		BOHGE_FORCEINLINE void Play()
		{
			m_isPlayAnimation = true;
		}
		BOHGE_FORCEINLINE void Stop()
		{
			m_isPlayAnimation = false;
		}
		BOHGE_FORCEINLINE void Loop(bool b)
		{
			m_isLoop = b;
		}
	};
}

