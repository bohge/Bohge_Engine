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
//		  输出文本		   //
/////////////////////////////
#pragma once

#include "3DMath.h"
#include <string>
#include <list>



struct FT_LibraryRec_;
typedef struct FT_LibraryRec_  *FT_Library;
struct FT_FaceRec_;
typedef struct FT_FaceRec_*  FT_Face;
struct FT_GlyphRec_;
typedef struct FT_GlyphRec_*  FT_Glyph;

namespace BohgeEngine
{
	class StringManager;
	class Texture2D;
	class VertexTextureColor;
	class RendBuffer;
	class Engine;
	class VertexBuffer;
	class IndicesBuffer;
	//这是每个顶点的示意图，绘制是在二维世界的
	//		y
	//		|
	//		|
	//-x----0---2-----x
	//		|   |
	//		1---3
	//		|
	//		-y

	class BString	//文本打印
	{
	public:
		enum Alignment
		{
			Center,
			Left,
			LeftTop,
			LeftBottom,
			Right,
			RightTop,
			RightBottom,
		};
		enum StringConst
		{
			COUNT_NODE			= 1,
			COUNT_OF_VERTEX		= 4,
			COUNT_OF_INDIES		= 4,
		};
	private:
		friend class StringManager;
	private:
		StringManager*					m_Father;
		bool							m_isManaged;//如果是true则引擎绘制，否则自己绘制
		Alignment						m_eAlign;
		vector2f						m_fScale;
		std::list<BString*>::iterator	m_Iterator;//迭代器，为了快速删除
		bool							m_isShow;//是否绘制
		vector2f						m_ScreenFontSize;//生成时候要求的尺寸
		vector2d						m_StringTextureSize;//字符串的像素宽度
		int								m_MaxHeight;//行高
		vector2f						m_Size;//文本的相对与屏幕像素的大小[0,1]区间，对应纹理坐标系
		vector2f						m_Pos;//绘制地点；
		VertexTextureColor*				m_pVertex;//顶点序列
		RendBuffer*						m_pRendBuffer;
		Texture2D*						m_pTex;//文本纹理
		std::wstring					m_String;

	private:
		BString(StringManager* father, const std::wstring& wstr, int height, const vector2f& screensize, const vector2d& txtSize, const Color& clr, RendBuffer* buffer, const vector2d& texSize, const byte* pBuffer );
		~BString();
	public:
		void Render(Engine & engine);//使用默认位置绘制
		void SetColor(const Color& clr);
		void SetScale(const vector2f& scale);
		vector4f GetStringCoordnate( const std::wstring& wstr );//得到某一段字符串在字符串纹理中的坐标，xy是lt，zw是rb
	public:
		BOHGE_FORCEINLINE Texture2D& GetTexture()
		{
			return *m_pTex;
		}
		BOHGE_FORCEINLINE const vector2f& GetSize() const
		{
			return m_Size;
		}
		BOHGE_FORCEINLINE void SetIterator(const std::list<BString*>::iterator& it)
		{
			m_Iterator = it;
		}
		BOHGE_FORCEINLINE void SetPosition(const vector2f& pos, Alignment ali)
		{
			m_eAlign = ali;
			vector2f add;
			switch( ali )
			{
			case Center: add = m_Size/2.0f; add.m_x = -add.m_x; break;
			case Left: add = vector2f(0,m_Size.m_y/2.0f); break;
			case LeftTop: add = vector2f(0,0); break;
			case LeftBottom: add = vector2f(0,m_Size.m_y); break;
			case Right: add = vector2f(m_Size.m_x,m_Size.m_y/2.0f) /*  / 2.0f * 2.0f  */; add.m_x = -add.m_x; break;
			case RightTop: add = vector2f(m_Size.m_x,0) /*  / 2.0f * 2.0f  */; add.m_x = -add.m_x; break;
			case RightBottom: add = m_Size /*  / 2.0f * 2.0f  */; add.m_x = -add.m_x; break;
			}
			m_Pos = pos + add;
		}
		BOHGE_FORCEINLINE void Hide()
		{
			m_isShow = false;
		}
		BOHGE_FORCEINLINE void Show()
		{
			m_isShow = true;
		}
		BOHGE_FORCEINLINE void SetManaged(bool b)
		{
			m_isManaged = b;
		}
		BOHGE_FORCEINLINE bool isManaged()
		{
			return m_isManaged;
		}
	};



	/////////////////////////
	//		  管理		   //
	/////////////////////////
	//文本中的#被视作是换行
	class StringManager//文本管理生成器
	{
	private:
		class BChar //读取的每一个char
		{
		public:
			FT_Glyph	m_pGlyph;
			vector2d	m_PixelSize;
			int			m_nBeginY;
			bool		m_isSpace;//是否是空格
			BChar(int beginy, const vector2d& size, FT_Glyph glyph, bool isspace );
			~BChar();
		};
		typedef std::list<RendBuffer*> RendBufferList;//BString的rendbuffer向StringManager申请
		typedef std::list<BChar*> BCharList;//读取每一个string中的char使用
		typedef std::list<BString*> BStringList;//读取进来的每一个string
	private:
		bool				m_isLoad;
		vector2f			m_FaceSize;//默认的字体大小
		BStringList			m_StringList;//已经加载的全部的文本
		FT_Library			m_FT2Lib;
		FT_Face				m_FT_Face;
		RendBufferList		m_FreeBufferList;//空闲的buffer表，就不需要申请了
	private:
		BChar* _LoadChar(wchar_t wchar);
		BString* _InternalMakeString(bool isScale, const vector2f& fontsize, const Color& clr, const std::wstring& wstr);//构造装载一个文本，返回一个可打印的字符串
		vector2d _InternalGetStringSize(bool isScale, const vector2f& fontsize, const std::wstring& wstr);//构造装载一个文本，返回一个可打印的字符串
		RendBuffer* _AllocBuffer();
	public:
		StringManager();
		~StringManager();
		void OnSizeChange( const vector2d& size );
		void LoadFont(const std::string& filename);
		BString* MakeString(const std::wstring& wstr);
		BString* MakeString(const vector2f& fontsize, const std::wstring& wstr);
		BString* MakeString(const vector2f& fontsize, const Color& clr,const std::wstring& wstr);
		vector2d GetStringSize( const std::wstring& wstr ); //得到字符串的像素长度
		vector2d GetStringSize( const vector2f& fontsize, const std::wstring& wstr ); //得到字符串的像素长度
		void DestroyString(BString* bstr);
		void Render(Engine & engine);
	public:
		BOHGE_FORCEINLINE bool isLoaded()
		{
			return m_isLoad;
		}
	};

}