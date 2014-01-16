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



//////////////////////////////
//			旋转环		   //
/////////////////////////////
#pragma once
#include "3DMath.h"
#include "Ray.h"
#include "aabbox.hpp"
#include "LocatableObject.h"
#include "ISceneNode.h"


namespace BohgeEngine
{
	class SharedBuffer;
	class VertexBuffer;
	class IndicesBuffer;
	class RotatingRing : public ISceneNode, public LocatableObject
	{
	private:
		enum Constant
		{
			RC_AXIS = 3,
			RC_POINT = 359,
		};
	private:
		bool				m_isMaked;
		RendBuffer*			m_pRingBuffer;
		VertexBuffer*		m_pVertexBuffer;
		IndicesBuffer*		m_pIndicesBuffer;
		float				m_fRadius;
	protected:
		Matrix44f			m_Transform;
	public:
		RotatingRing( );
		virtual ~RotatingRing( );
	public:
		virtual void CreateRing( const float& r );
	public:
		virtual void Update( uint milisecon );
		virtual bool isInview( const Frustumf& cFru );
		virtual void EndRender( Engine& engine );
		virtual const void* Attribute(const Node_Attribute& name) const;
	};







	class SelectableRotatingRing : public RotatingRing //可以点击选取到每个轴的Axis
	{
	public:
		enum Selected
		{
			SR_NON,
			SR_BY_X_AXIS,
			SR_BY_Y_AXIS,
			SR_BY_Z_AXIS,
		};
	private:
		aabbox3df		m_XBind;
		aabbox3df		m_YBind;
		aabbox3df		m_ZBind;
	public:
		virtual void CreateRing( const float& r );
		virtual void EndRender( Engine& engine );
	public:
		Selected IsSelected( const Rayf& r );
		BOHGE_FORCEINLINE const aabbox3df& XBind()
		{
			return m_XBind;
		}
		BOHGE_FORCEINLINE const aabbox3df& YBind()
		{
			return m_YBind;
		}
		BOHGE_FORCEINLINE const aabbox3df& ZBind()
		{
			return m_ZBind;
		}
	};
}