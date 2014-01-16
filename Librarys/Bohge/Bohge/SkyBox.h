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



/////////////////////////////
//			天空盒		   //
/////////////////////////////
#pragma once
#include "3DMath.h"
#include <list>
#include <string>

namespace BohgeEngine
{
	class Engine;
	class RendBuffer;
	class Vertex;
	class TextureCube;
	class VertexBuffer;
	class IndicesBuffer;
	class SkyBox
	{
	private:
		//		4------7
		//     /|     /|
		//    0-+----3 |
		//    | 5----|-6
		//    |/     |/
		//    1------2
		class Box	//内部的绘制盒子类型
		{
		private:
			enum
			{
				VERTEX_COUNT	= 8,
			};
		private:
			Vertex*				m_pVertexArray;
			RendBuffer*			m_pBuffer;
			VertexBuffer*		m_pVertexBuffer;
			IndicesBuffer*		m_pIndicesBuffer;
			TextureCube*		m_Texture;
			Matrix44f			m_LocalRotate;
			float				m_Times;
		public:
			Box( bool isAlpha, const std::string& f, const std::string& b, const std::string& u, const std::string& d, const std::string& l, const std::string& r );
			~Box();
			void Render(Engine& engine, const Matrix44f* trans);
			void RenderRotation(Engine& engine, const Matrix44f* trans);
			BOHGE_FORCEINLINE const TextureCube* Texture() const
			{
				return m_Texture;
			}
		};
	public:
		enum POS
		{
			BACKGROUND,
			FRONT,
		};
	private:
		typedef std::list<Box*>	SkyBoxList;
	private:
		Box*			m_pBackground;//背景天空盒
		SkyBoxList		m_Front;//前景天空盒
		Matrix44f*		m_pTransform;
	public:
		SkyBox();
		~SkyBox();
		//BACKGROUND 不会参加posteffect，而FRONT会参加
		void AndSkyBox(POS pos, const std::string& f, const std::string& b, const std::string& u, const std::string& d, const std::string& l, const std::string& r );
		void ClearAll();
		void RenderBackground(Engine& engine);
		void RenderSkyBox(Engine& engine);
		void Rotate(const vector3f& vector , const float radian);
	public:
		BOHGE_FORCEINLINE bool hasBackground()
		{
			return NULL != m_pBackground;
		}
		BOHGE_FORCEINLINE bool hasFrontSky()
		{
			return 0 != m_Front.size();
		}
		BOHGE_FORCEINLINE const TextureCube* Texture() const
		{
			if( NULL == m_pBackground )
				return NULL;
			return m_pBackground->Texture();
		}
	};

}