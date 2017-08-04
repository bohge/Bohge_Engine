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
#include "SkeletonKeyFrame.h"
#include "SkeletonJoint.hpp"
#include "IMonopolyResource.h"



namespace BohgeEngine
{
	class BOHGE_FRAMEWORK SkeletonResource : public IMonopolyResource
	{
		RTTI_ABSTRACT_DRIVER_TYPE_DECLEAR( SkeletonResource, IMonopolyResource )
	private:
		typedef eastl::pair< vector4arrayf, vector4arrayf > AnimationDual;//最后要传入shader的数据
	private:
		uint								m_MaxBoneIndexCount;//最大索引数目，用于确定调用什么样子的shader
		eastl::vector<SkeletonJoint>		m_JointVector;
		SkeletonKeyFrame					m_KeyFrame;
		AnimationDual						m_AnimationDual;
	public:
		SkeletonResource(void);
		virtual ~SkeletonResource(void);
	public:
		virtual void ReleaseResource();
	private:
		void _BuildBones(float sec);//从建骨骼动画
	public:
		void SetupSkeletone( uint boneCount, const eastl::vector<SkeletonJoint>& joints, const SkeletonKeyFrame& keyframe );
	public:
		BOHGE_FORCEINLINE void SetAnimationInterval( int begin, int end )
		{
			m_KeyFrame.SetAnimationInterval( begin, end );
		}
		BOHGE_FORCEINLINE void ResetAnimation()
		{
			m_KeyFrame.ResetAnimation();
			_BuildBones(0.0f);
		}
		BOHGE_FORCEINLINE void AnimationPlay()
		{
			m_KeyFrame.Play();
		}
		BOHGE_FORCEINLINE void AnimationStop()
		{
			m_KeyFrame.Stop();
		}
		BOHGE_FORCEINLINE void AnimationLoop(bool b)
		{
			m_KeyFrame.Loop( b );
		}
		//关于模型骨骼挂节点
		BOHGE_FORCEINLINE eastl::vector< eastl::string > GetJointNameList()//得到可以挂物体的骨骼节点
		{
			return m_KeyFrame.GetJointNameList();
		}
		BOHGE_FORCEINLINE const vector4arrayf* RealVector()
		{
			return &m_AnimationDual.first;
		}
		BOHGE_FORCEINLINE const vector4arrayf* DualVector()
		{
			return &m_AnimationDual.second;
		}
	};
}

