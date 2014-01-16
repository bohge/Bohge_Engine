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



/////////////////////////////////////////
//			State Manage class		   //
/////////////////////////////////////////

#pragma once


#include "Predefine.h"
#include "3DMath.h"
#include "Device.h"
#include "BaseShader.h"
#include <list>
#include <vector>

//注意，全部的顶点类型不能有任何的虚函数，应为顶点类型最终是通过opengl的内存指针的形式被指定出来，然后进行绘制
//这种形式要求数据在内存上是连续的，任何虚函数可能导致数据的不连续，所以不能使用虚函数

namespace BohgeEngine
{
	class VertexLayout;
	typedef std::list<VertexLayout*> VertexLayoutList;
	//-------------------------------------------------------------------
	class Vertex
	{

	protected:
		vector4f	m_Vertex;
	public:
		Vertex();
		Vertex(const vector4f& position);
		Vertex(const Vertex& vertex);
		//得到这个顶点的layout属性
		Vertex& operator= (const Vertex& rhs);
	public:
		static VertexLayoutList Layout();
	public:
		BOHGE_FORCEINLINE void SetVertex(const vector4f& vertex)
		{
			m_Vertex = vertex;
		}
		BOHGE_FORCEINLINE vector4f& Position()
		{
			return m_Vertex;
		}
		BOHGE_FORCEINLINE const vector4f& Position() const
		{
			return m_Vertex;
		}
		static BOHGE_FORCEINLINE uint GetElementCount()
		{
			return vector4f::Size();
		}
	};
	//-------------------------------------------------------------------
	class VertexColor : public Vertex
	{

	protected:
		Color		m_Color;
	public:
		VertexColor();
		VertexColor(const vector4f& position, const Color& color);
		VertexColor(const VertexColor& vertex);
		VertexColor& operator= (const VertexColor& rhs);
	public:
		static VertexLayoutList Layout();
	public:
		BOHGE_FORCEINLINE void SetColor(const Color& color)
		{
			m_Color = color;
		}
		BOHGE_FORCEINLINE Color& Colors()
		{
			return m_Color;
		}
		BOHGE_FORCEINLINE const Color& Colors() const
		{
			return m_Color;
		}
		static BOHGE_FORCEINLINE uint GetElementCount()
		{
			return Vertex::GetElementCount() + Color::Size();
		}
	};


	//-------------------------------------------------------------------
	class VertexTexture : public Vertex
	{

	protected:
		vector2f	m_Coordinate;
	public:
		VertexTexture();
		VertexTexture(const vector4f& position, const vector2f& coordinate);
		VertexTexture(const VertexTexture& vertex);
		VertexTexture& operator= (const VertexTexture& rhs);
	public:
		static VertexLayoutList Layout();
	public:
		BOHGE_FORCEINLINE vector2f& Coordinate()
		{
			return m_Coordinate;
		}
		BOHGE_FORCEINLINE const vector2f& Coordinate() const
		{
			return m_Coordinate;
		}
		static BOHGE_FORCEINLINE uint GetElementCount()
		{
			return Vertex::GetElementCount() + vector2f::Size();
		}
	};

	//-------------------------------------------------------------------
	class VertexNormal : public Vertex
	{
	protected:
		vector3f	m_Normal;
	public:
		VertexNormal();
		VertexNormal(const vector4f& position, const vector3f& normal);
		VertexNormal(const VertexNormal& vertex);
		VertexNormal& operator= (const VertexNormal& rhs);
	public:
		static VertexLayoutList Layout();
	public:
		BOHGE_FORCEINLINE vector3f& Normal()
		{
			return m_Normal;
		}
		BOHGE_FORCEINLINE const vector3f& Normal() const
		{
			return m_Normal;
		}
		static BOHGE_FORCEINLINE uint GetElementCount()
		{
			return Vertex::GetElementCount() + vector3f::Size();
		}
	};

	//-------------------------------------------------------------------
	class Vertex2Texture : public Vertex
	{
	protected:
		vector4f	m_Coordinate;
	public:
		Vertex2Texture();
		Vertex2Texture(const vector4f& position, const vector4f& coordinate);
		Vertex2Texture(const Vertex2Texture& vertex);
		Vertex2Texture& operator= (const Vertex2Texture& rhs);
	public:
		static VertexLayoutList Layout();
	public:
		BOHGE_FORCEINLINE vector4f& Coordinates()
		{
			return m_Coordinate;
		}
		BOHGE_FORCEINLINE const vector4f& Coordinates() const
		{
			return m_Coordinate;
		}
		static BOHGE_FORCEINLINE uint GetElementCount()
		{
			return Vertex::GetElementCount() + vector4f::Size();
		}
	};

	//-------------------------------------------------------------------
	class VertexTextureColor : public VertexTexture
	{

	protected:
		Color	m_Color;
	public:
		VertexTextureColor();
		VertexTextureColor(const vector4f& position, const vector2f& coordinate, const Color& color);
		VertexTextureColor& operator= (const VertexTextureColor& rhs);
	public:
		BOHGE_FORCEINLINE void SetColor(const Color& clr)
		{
			m_Color = clr;
		}
		BOHGE_FORCEINLINE Color& Colors()
		{
			return m_Color;
		}
		BOHGE_FORCEINLINE const Color& Colors() const
		{
			return m_Color;
		}
		static BOHGE_FORCEINLINE uint GetElementCount()
		{
			return VertexTexture::GetElementCount() + Color::Size();
		}
	public:
		static VertexLayoutList Layout();
	};


	//-------------------------------------------------------------------
	class VertexTextureNormal : public VertexTexture
	{

	protected:
		vector3f	m_Normal;
	public:
		VertexTextureNormal();
		VertexTextureNormal(const vector4f& position, const vector2f& coordinate, const vector3f& normal);
		VertexTextureNormal(const VertexTextureNormal& vertex);
		VertexTextureNormal& operator= (const VertexTextureNormal& rhs);
	public:
		BOHGE_FORCEINLINE vector3f& Normal()
		{
			return m_Normal;
		}
		BOHGE_FORCEINLINE const vector3f& Normal() const
		{
			return m_Normal;
		}
	public:
		static VertexLayoutList Layout();
	public:
		static BOHGE_FORCEINLINE uint GetElementCount()
		{
			return VertexTexture::GetElementCount() + vector3f::Size();
		}
	};

	//-------------------------------------------------------------------
	class Vertex2TextureNormal : public Vertex2Texture
	{

	protected:
		vector3f	m_Normal;
	public:
		Vertex2TextureNormal();
		Vertex2TextureNormal(const vector4f& position, const vector4f& coordinate, const vector3f& normal);
		Vertex2TextureNormal(const Vertex2TextureNormal& vertex);
		Vertex2TextureNormal& operator= (const Vertex2TextureNormal& rhs);
	public:
		static VertexLayoutList Layout();
	public:
		BOHGE_FORCEINLINE vector3f& Normal()
		{
			return m_Normal;
		}
		BOHGE_FORCEINLINE const vector3f& Normal() const
		{
			return m_Normal;
		}
		static BOHGE_FORCEINLINE uint GetElementCount()
		{
			return Vertex2Texture::GetElementCount() + vector3f::Size();
		}
	};


	//-------------------------------------------------------------------
	class Vertex2TextureNormalTangent : public Vertex2TextureNormal
	{

	protected:
		vector3f	m_Tangent;
	public:
		Vertex2TextureNormalTangent();
		Vertex2TextureNormalTangent(const vector4f& position, const vector4f& coordinate, const vector3f& normal, const vector3f& tangent);
		Vertex2TextureNormalTangent(const Vertex2TextureNormalTangent& vertex);
		Vertex2TextureNormalTangent& operator= (const Vertex2TextureNormalTangent& rhs);
	public:
		static VertexLayoutList Layout();
	public:
		BOHGE_FORCEINLINE vector3f& Tangent()
		{
			return m_Tangent;
		}
		BOHGE_FORCEINLINE const vector3f& Tangent() const
		{
			return m_Tangent;
		}
		static BOHGE_FORCEINLINE uint GetElementCount()
		{
			return Vertex2TextureNormal::GetElementCount() + vector3f::Size();
		}
	};


	//-------------------------------------------------------------------
	class VertexCubeTexture : public Vertex
	{

	protected:
		vector3f	m_Coordinate;
	public:
		VertexCubeTexture();
		VertexCubeTexture(const vector4f& position, const vector3f& coordinate);
		VertexCubeTexture(const VertexCubeTexture& vertex);
		VertexCubeTexture& operator= (const VertexCubeTexture& rhs);
	public:
		static VertexLayoutList Layout();
	public:
		static BOHGE_FORCEINLINE uint GetElementCount()
		{
			return Vertex::GetElementCount() + vector3f::Size();
		}
	};

	//-------------------------------------------------------------------
	class VertexTextureNormalTangent : public VertexTextureNormal
	{
	protected:
		vector4f m_Tangent;//切线是四位的，最后一位是是否需要反转副法线的标识
	public:
		VertexTextureNormalTangent();
		VertexTextureNormalTangent(const vector4f& position, const vector2f& coordinate, const vector3f& normal, const vector4f& tangent);
		VertexTextureNormalTangent(const VertexTextureNormalTangent& vertex);
		VertexTextureNormalTangent& operator= (const VertexTextureNormalTangent& rhs);
	public:
		static VertexLayoutList Layout();
	public:
		BOHGE_FORCEINLINE vector4f& Tangent()
		{
			return m_Tangent;
		}
		BOHGE_FORCEINLINE const vector4f& Tangent() const
		{
			return m_Tangent;
		}
		static BOHGE_FORCEINLINE uint GetElementCount()
		{
			return VertexTextureNormal::GetElementCount() + vector3f::Size();
		}
	};


	//-------------------------------------------------------------------
	class VertexTextureNormalTangentBinormal : public VertexTextureNormalTangent
	{
	protected:
		vector3f m_Binormal;
	public:
		VertexTextureNormalTangentBinormal();
		VertexTextureNormalTangentBinormal(const vector4f& position, const vector2f& coordinate, const vector3f& normal, const vector4f& tangent, const vector3f& binormal);
		VertexTextureNormalTangentBinormal(const VertexTextureNormalTangentBinormal& vertex);
		VertexTextureNormalTangentBinormal& operator= (const VertexTextureNormalTangentBinormal& rhs);
	public:
		static VertexLayoutList Layout();
	public:
		BOHGE_FORCEINLINE vector3f& Binormal()
		{
			return m_Binormal;
		}
		BOHGE_FORCEINLINE const vector3f& Binormal() const
		{
			return m_Binormal;
		}
		static BOHGE_FORCEINLINE uint GetElementCount()
		{
			return VertexTextureNormalTangent::GetElementCount() + vector3f::Size();
		}
	};

	//-------------------------------------------------------------------
	class VertexTextureNormalBoneWeight : public VertexTextureNormal
	{
	protected:
		vector4f m_BoneIndex;
		vector4f m_Weight;
	public:
		VertexTextureNormalBoneWeight();
		VertexTextureNormalBoneWeight(const vector4f& position, const vector2f& coordinate, const vector3f& normal, const vector4f& index, const vector4f& weight);
		VertexTextureNormalBoneWeight(const VertexTextureNormalBoneWeight& vertex);
		VertexTextureNormalBoneWeight& operator= (const VertexTextureNormalBoneWeight& rhs);
	public:
		static VertexLayoutList Layout();
	public:
		static BOHGE_FORCEINLINE uint GetElementCount()
		{
			return VertexTextureNormal::GetElementCount() + vector3f::Size();
		}
	};


	//-------------------------------------------------------------------
	class VertexTextureNormalTangentBoneWeight : public VertexTextureNormalTangent
	{
	protected:
		vector4f m_BoneIndex;
		vector4f m_Weight;
	public:
		VertexTextureNormalTangentBoneWeight();
		VertexTextureNormalTangentBoneWeight(const vector4f& position, const vector2f& coordinate, const vector3f& normal, const vector4f& tangent, const vector4f& index, const vector4f& weight);
		VertexTextureNormalTangentBoneWeight(const VertexTextureNormalTangentBoneWeight& vertex);
		VertexTextureNormalTangentBoneWeight& operator= (const VertexTextureNormalTangentBoneWeight& rhs);
	public:
		static VertexLayoutList Layout();
	public:
		static BOHGE_FORCEINLINE uint GetElementCount()
		{
			return VertexTextureNormalTangent::GetElementCount() + 2 * vector4f::Size();
		}
	};



	//-------------------------------------------------------------------
	class VertexTextureNormalTangentBinormalBoneWeight : public VertexTextureNormalTangentBinormal
	{
	protected:
		vector4f m_BoneIndex;
		vector4f m_Weight;
	public:
		VertexTextureNormalTangentBinormalBoneWeight();
		VertexTextureNormalTangentBinormalBoneWeight(const vector4f& position, const vector2f& coordinate, const vector3f& normal, const vector4f& tangent, const vector3f& binormal, const vector4f& index, const vector4f& weight);
		VertexTextureNormalTangentBinormalBoneWeight(const VertexTextureNormalTangentBinormalBoneWeight& vertex);
		VertexTextureNormalTangentBinormalBoneWeight& operator= (const VertexTextureNormalTangentBinormalBoneWeight& rhs);
	public:
		static VertexLayoutList Layout();
	public:
		static BOHGE_FORCEINLINE uint GetElementCount()
		{
			return VertexTextureNormalTangentBinormal::GetElementCount() + 2 * vector4f::Size();
		}
	};







	//-------------------------------------------------------------------
#pragma pack(push,1)
	class VertexLayout	//顶点的布局方式，使得Vertex数组可以直接输出
	{
	private:
		Attributes::ShaderAttributes	m_eAttributes;//顶点的类型
		uint							m_uSize;//每个定点数据的个数
		Device::DataType				m_eType;//顶点数据的类型
		bool							m_isNormalize;//是否规范化的，一般为false
		uint							m_uStride;//每个数据段的bite偏移量
	public:
		VertexLayout(Attributes::ShaderAttributes attr, uint size, Device::DataType type, uint stride, bool isnormal = false);
		VertexLayout(const VertexLayout& input);
	public:
		BOHGE_FORCEINLINE Attributes::ShaderAttributes GetAttributes()
		{
			return m_eAttributes;
		}
		BOHGE_FORCEINLINE uint Size()
		{
			return m_uSize;
		}
		BOHGE_FORCEINLINE Device::DataType DataType()
		{
			return m_eType;
		}
		BOHGE_FORCEINLINE bool IsNormalize()
		{
			return m_isNormalize;
		}
		BOHGE_FORCEINLINE uint Stride()
		{
			return m_uStride;
		}
	};
#pragma pack(pop)

}
