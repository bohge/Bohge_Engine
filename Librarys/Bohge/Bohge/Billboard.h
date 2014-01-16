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
//			广告版		   //
/////////////////////////////

#pragma once

#include "3DMath.h"
#include "Vertex.h"
#include "Device.h"
#include "ISceneNode.h"

#include <string>



namespace BohgeEngine
{
	class Engine;
	class Texture;
	class SharedBuffer;
	//0---2
	//|	  |
	//1---3
	class Billboard : public IGetBuffer //可以多个打包的，所以center当流传进取
	{
	public:
		typedef	Vertex2TextureNormal VertexType;
	protected:
		enum Const
		{
			COUNT_OF_VERTEX		= 4,
			COUNT_OF_INDIES		= 6,
		};
	protected:
		Vertex2TextureNormal		m_VertexArray[COUNT_OF_VERTEX];//使用法线数据段保存中心值
	public:
		explicit Billboard();
		explicit Billboard( const vector2f& b, const vector2f& e );
		virtual ~Billboard();
	public://继承的接口
		virtual void GetVertexBuffer(SharedBuffer& input) const ;
	public:
		virtual void SetBillBorad( const vector3f& pos, const vector2f& rectsize );
		virtual void SetBillCoordnate( const vector2f& lt, const vector2f& rb );
	public:
		static BOHGE_FORCEINLINE VertexLayoutList GetLayout()
		{
			return Vertex2TextureNormal::Layout();
		}
		static BOHGE_FORCEINLINE uint GetElementCount()
		{
			return Vertex2TextureNormal::GetElementCount();
		}
		static BOHGE_FORCEINLINE uint GetSizeOfVertex()
		{
			return sizeof( Vertex2TextureNormal );
		}
		static BOHGE_FORCEINLINE uint GetCountOfVertex()
		{
			return static_cast<uint>(COUNT_OF_VERTEX);
		}
		static BOHGE_FORCEINLINE uint GetCountOfIndes()
		{
			return static_cast<uint>(COUNT_OF_INDIES);
		}
		static BOHGE_FORCEINLINE Device::RenderMode GetRenderMode()
		{
			return Device::RM_TRIANGLES;
		}
	};






	class VertexBuffer;
	class IndicesBuffer;
	class BillboardNode : public Billboard,  public ISceneNode
	{
	private:
		RendBuffer*			m_pRendBuffer;
		VertexBuffer*		m_pVertexBuffer;
		IndicesBuffer*		m_pIndicesBuffer;
		Texture2D*			m_pTexture;
	public:
		explicit BillboardNode();
		~BillboardNode();
	public:
		virtual void SetBillBorad( const vector3f& pos, const vector2f& rectsize );
		virtual void SetBillCoordnate( const vector2f& lt, const vector2f& rb );
		virtual bool isInview( const Frustumf& cFru );//判断当前帧是否可见是否需要绘制
		virtual void EndRender( Engine& engine );
	public:
		BOHGE_FORCEINLINE void SetTexture( Texture2D* tex )
		{
			m_pTexture = tex;
		}
	};
}