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
#include "SkeletonEntity.h"
#include "IComponent.h"
#include "3DMath.h"



namespace BohgeEngine
{
	class BOHGE_FRAMEWORK SkeletonComponent : public IComponent
	{
		RTTI_DRIVER_TYPE_DECLEAR( SkeletonComponent, IComponent );
	private:
		SkeletonEntity*			m_pSkeletonEntity;
	public:
		SkeletonComponent(void);
		~SkeletonComponent(void);
	private:
		virtual void _DoOnAttachNode( );//当挂接在node上
	public:
		virtual void OnRootNodeChange( );//当所属node的根节点变化时
		virtual void Release( );//将自己释放掉
	public:
		const Object* AttributeAnimationReal() const;
		const Object* AttributeAnimationDual() const;
	public:
		void LoadSkeleton( const eastl::string& path );
	public:
		BOHGE_FORCEINLINE void SetAnimationInterval( int begin, int end )
		{
			m_pSkeletonEntity->SetAnimationInterval( begin, end );
		}
		BOHGE_FORCEINLINE void ResetAnimation()
		{
			m_pSkeletonEntity->ResetAnimation();
		}
		BOHGE_FORCEINLINE void AnimationPlay()
		{
			m_pSkeletonEntity->AnimationPlay();
		}
		BOHGE_FORCEINLINE void AnimationStop()
		{
			m_pSkeletonEntity->AnimationStop();
		}
		BOHGE_FORCEINLINE void AnimationLoop(bool b)
		{
			m_pSkeletonEntity->AnimationLoop( b );
		}
		//关于模型骨骼挂节点
		BOHGE_FORCEINLINE eastl::vector< eastl::string > GetJointNameList()//得到可以挂物体的骨骼节点
		{
			return m_pSkeletonEntity->GetJointNameList();
		}
	};
}

