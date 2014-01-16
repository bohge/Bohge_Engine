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
//			坐标轴		   //
/////////////////////////////
#pragma once
#include "3DMath.h"
#include "Ray.h"
#include "aabbox.hpp"
#include "ISceneNode.h"


namespace BohgeEngine
{
	class BString;
	class Texture2D;
	class Billboard;
	class SharedBuffer;
	class VertexBuffer;
	class IndicesBuffer;
	class AxisNode : public ISceneNode//这里没有直接使用LineNode的原因是drall call太多了
	{
	public:
		enum
		{
			COUNT_OF_VERTEX		= 6,
			COUNT_OF_INDIES		= 6,
			MAX_BILLBOARD_SIZE	= 30,
			MAX_TEXTURE_SIZE	= 10,
		};
	private:
		bool				m_isMaked;
		vector3f			m_Center;
		float				m_AsixLength;
		int					m_TextureCount;

		VertexColor*		m_pLineArray;//线的
		RendBuffer*			m_pLineBuffer;
		VertexBuffer*		m_pLineVertex;
		IndicesBuffer*		m_pLineIndices;

		int					m_BillboardSize;
		Billboard*			m_pBillboardArray;
		SharedBuffer*		m_pBillboardSharedBuffer;//共享内存的buffer提取器
		RendBuffer*			m_pBillboardBuffer;
		VertexBuffer*		m_pBillboardBufferVertex;
		IndicesBuffer*		m_pBillboardBufferIndices;
		BString*			m_pBillboardString;

		Matrix44f			m_Transfrom;
	public:
		AxisNode(void);
		virtual ~AxisNode(void);
	public:
		virtual void Update( uint milisecon );
		virtual bool isInview( const Frustumf& cFru );
		virtual void EndRender( Engine& engine );
		virtual const void* Attribute(const Node_Attribute& name) const;
	public:
		virtual void CreateAxis( const vector3f& center, float l, int countText );//设置轴的长度,每个轴上[0, &]和[-&, 0]显示数字的个数
		virtual void SetCenter( const vector3f& center );
	public:
		BOHGE_FORCEINLINE const float& GetAsixLength()
		{
			return m_AsixLength;
		}
	};






	class SelectableAxisNode : public AxisNode //可以点击选取到每个轴的Axis
	{
	public:
		enum Selected
		{
			SA_NON,
			SA_X_AXIS,
			SA_Y_AXIS,
			SA_Z_AXIS,
		};
	private:
		aabbox3df		m_XBind;
		aabbox3df		m_YBind;
		aabbox3df		m_ZBind;
	public:
		virtual void CreateAxis( const vector3f& center, float l, int countText );//设置轴的长度,每个轴上[0, &]和[-&, 0]显示数字的个数
		virtual void SetCenter( const vector3f& center );
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