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



namespace BohgeEngine
{
	struct SkeletonJoint//骨骼信息
	{
		eastl::string							m_Name; //骨骼名称
		int										m_Parent;//骨骼的父
		int										m_KeyFrameIndex;//骨骼在关键帧动画中的索引
		Quaternionf								m_RealBind;
		Quaternionf								m_DaulBind;
		eastl::pair<Quaternionf, Quaternionf>	m_InversPair;//骨骼初始状态的逆
	private:
		BOHGE_FORCEINLINE void _DoCopy( const SkeletonJoint& rhs )
		{
			//memcpy( this, &rhs, sizeof( Joint ) );//不能用会崩溃，字符串析沟，这里的std字符串不是实际复制构造的，共用了一个内存
			m_Name = rhs.m_Name;
			m_Parent = rhs.m_Parent;
			m_KeyFrameIndex = rhs.m_KeyFrameIndex;
			m_RealBind = rhs.m_RealBind;
			m_DaulBind = rhs.m_DaulBind;
			m_InversPair.first = rhs.m_InversPair.first;
			m_InversPair.second = rhs.m_InversPair.second;
		}
	public:
		SkeletonJoint()
		{

		}
		SkeletonJoint( const SkeletonJoint& rhs )
		{
			_DoCopy(rhs);
		}
		SkeletonJoint& operator = ( const SkeletonJoint& rhs )
		{
			_DoCopy(rhs);
			return *this;
		}
	};
}