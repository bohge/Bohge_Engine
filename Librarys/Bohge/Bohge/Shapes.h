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



/////////////////////////////////////
//			线,矩形等图形		   //
/////////////////////////////////////
#pragma once
#include "3DMath.h"
#include "ISceneNode.h"
#include "Device.h"

namespace BohgeEngine
{
	class Engine;
	class VertexColor;
	class RendBuffer;
	class SharedBuffer;
	class VertexBuffer;
	class IndicesBuffer;
	class LineNode : public ISceneNode
	{
	public:
		enum
		{
			COUNT_OF_VERTEX		= 2,
			COUNT_OF_INDIES		= 2,
		};
	private:
		//以下参数为绘制使用参数
		VertexColor*		m_pVertexArray;
		RendBuffer*			m_pRendBuffer;
		VertexBuffer*		m_pVertexBuffer;
		IndicesBuffer*		m_pIndicesBuffer;
		Matrix44f			m_Transfrom;
	public:
		LineNode(const vector4f& b, const vector4f& e, const Color& c);
		~LineNode();
	public:
		virtual bool isInview( const Frustumf& cFru ){ return true; }//判断当前帧是否可见是否需要绘制
		virtual void Render( Engine& engine );
	public:
		void SetLine( const vector4f& b, const vector4f& e );
	};
	//顶点的定义
	//0------3
	//|		 |
	//|		 |
	//1------2
	class Rect2D : public IGetBuffer//一个可同意绘制，可自绘制的矩形
	{
	public:
		enum
		{
			COUNT_OF_VERTEX		= 4,
			COUNT_OF_INDIES		= 8,
		};
	private:
		//以下参数为绘制使用参数
		VertexColor*			m_pVertexArray;
		RendBuffer*				m_pRendBuffer;
		VertexBuffer*			m_pVertexBuffer;
		IndicesBuffer*			m_pIndicesBuffer;
	private:
		void NewRect2D();
	public:
		Rect2D();
		Rect2D(const Rect2D& input);
		Rect2D(const vector2f& leftTop, const vector2f& rightBottom, const Color& color);
		~Rect2D();
		void SetRect(const vector2f& leftTop, const vector2f& rightBottom);
		void SetColor(const Color& color);
		void GetRect(vector2f& leftTop, vector2f& rightBottom) const;
	public://继承的接口
		virtual void GetVertexBuffer(SharedBuffer& izennput) const;
		/*使用ShapesShader 的相对坐标方式绘制该节点*/
		void Render(Engine& engine) const ;
	public:
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
			return Device::RM_LINES;
		}
	};





	//顶点定义
	//		4------5
	//     /|     /|
	//    0-+----1 |
	//    | 7----|-6
	//    |/     |/
	//    3------2
	class Rect3D : public IGetBuffer //一个可同意绘制，可自绘制的矩形
	{
	public:
		enum
		{
			COUNT_OF_VERTEX		= 8,
			COUNT_OF_INDIES		= 24,
		};
	private:
		//以下参数为绘制使用参数
		VertexColor*		m_pVertexArray;
		RendBuffer*			m_pRendBuffer;
		VertexBuffer*			m_pVertexBuffer;
		IndicesBuffer*			m_pIndicesBuffer;
		Matrix44f			m_Transform;
	public:
		Rect3D(const vector3f& ulb, const vector3f& drf, const Color& color);
		~Rect3D();
	public://继承的接口
		virtual void GetVertexBuffer(SharedBuffer& izennput) const;
	public:
		void Render(Engine& engine) const ;
		void ResetRect3D( const vector3f& ulb, const vector3f& drf );
		void SetTransform( const Matrix44f& trf );
	public:
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
			return Device::RM_LINES;
		}
	};

}