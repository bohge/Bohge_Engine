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

#include "Vertex.h"
#include "Predefine.h"
#include "Log.h"





namespace BohgeEngine
{
	/////////////////////////////
	//			Vertex		   //
	/////////////////////////////
	//-------------------------------------------------------------------
	Vertex::Vertex()
	{
	}
	//-------------------------------------------------------------------
	Vertex::Vertex(const vector4f& position)
	{
		m_Vertex = position;
	}
	//-------------------------------------------------------------------
	Vertex::Vertex(const Vertex& vertex)
	{
		m_Vertex = vertex.m_Vertex;
	}
	//-------------------------------------------------------------------
	VertexLayoutList Vertex::Layout()
	{
		VertexLayoutList list;
		static VertexLayout vl(Attributes::ATTRIBUTE_POSITION, vector4f::Size(), Device::DT_FLOAT, vector4f::BitSize(), false );
		list.push_back(&vl);
		return list;

	}
	//-------------------------------------------------------------------
	Vertex& Vertex::operator = (const Vertex& rhs)
	{
		m_Vertex = rhs.m_Vertex;
		return *this;
	}
	//-------------------------------------------------------------------







	/////////////////////////////////
	//			VertexColor		   //
	/////////////////////////////////
	//-------------------------------------------------------------------
	VertexColor::VertexColor()
		:Vertex()
	{
	}
	//-------------------------------------------------------------------
	VertexColor::VertexColor(const vector4f& position, const Color& color)
		:Vertex(position),
		m_Color(color)
	{
	}
	//-------------------------------------------------------------------
	VertexColor::VertexColor(const VertexColor& vertex)
		:Vertex(vertex.m_Vertex),
		m_Color(vertex.m_Color)
	{
	}
	//-------------------------------------------------------------------
	VertexLayoutList VertexColor::Layout()
	{
		VertexLayoutList list = Vertex::Layout();
		static VertexLayout vl(Attributes::ATTRIBUTE_COLOR0, Color::Size(), Device::DT_FLOAT, Color::BitSize(), false );
		list.push_back(&vl);
		return list;
	}
	//-------------------------------------------------------------------
	VertexColor& VertexColor::operator = (const VertexColor& rhs)
	{
		Vertex::operator =(rhs);
		m_Color = rhs.m_Color;
		return *this;
	}
	//-------------------------------------------------------------------





	/////////////////////////////////
	//			VertexTexture	   //
	/////////////////////////////////
	//-------------------------------------------------------------------
	VertexTexture::VertexTexture()
		:Vertex()
	{
	}
	//-------------------------------------------------------------------
	VertexTexture::VertexTexture(const vector4f& position, const vector2f& coordinate)
		:Vertex(position),
		m_Coordinate(coordinate)
	{
	}
	//-------------------------------------------------------------------
	VertexTexture::VertexTexture(const VertexTexture& vertex)
		:Vertex(vertex.m_Vertex),
		m_Coordinate(vertex.m_Coordinate)
	{
	}
	//-------------------------------------------------------------------
	VertexLayoutList VertexTexture::Layout()
	{
		VertexLayoutList list = Vertex::Layout();
		static VertexLayout vl(Attributes::ATTRIBUTE_TEXCOORD0, vector2f::Size(), Device::DT_FLOAT, vector2f::BitSize(), false );
		list.push_back(&vl);
		return list;
	}
	//-------------------------------------------------------------------
	VertexTexture& VertexTexture::operator = (const VertexTexture& rhs)
	{
		Vertex::operator =(rhs);
		m_Coordinate = rhs.m_Coordinate;
		return *this;
	}
	//-------------------------------------------------------------------


	/////////////////////////////////
	//			VertexNormal	   //
	/////////////////////////////////
	//-------------------------------------------------------------------
	VertexNormal::VertexNormal()
		:Vertex()
	{
	}
	//-------------------------------------------------------------------
	VertexNormal::VertexNormal(const vector4f& position, const vector3f& normal)
		:Vertex(position),
		m_Normal(normal)
	{
	}
	//-------------------------------------------------------------------
	VertexNormal::VertexNormal(const VertexNormal &vertex)
		:Vertex(vertex.m_Vertex),
		m_Normal(vertex.m_Normal)
	{
	}
	//-------------------------------------------------------------------
	VertexLayoutList VertexNormal::Layout()
	{
		VertexLayoutList list = Vertex::Layout();
		static VertexLayout vl(Attributes::ATTRIBUTE_NORMAL, vector3f::Size(), Device::DT_FLOAT, vector3f::BitSize(), false );
		list.push_back(&vl);
		return list;
	}
	//-------------------------------------------------------------------
	VertexNormal& VertexNormal::operator = (const VertexNormal& rhs)
	{
		Vertex::operator =(rhs);
		m_Normal = rhs.m_Normal;
		return *this;
	}
	//-------------------------------------------------------------------



	/////////////////////////////////
	//			Vertex2Texture	   //
	/////////////////////////////////
	//-------------------------------------------------------------------
	Vertex2Texture::Vertex2Texture()
		:Vertex()
	{
	}
	//-------------------------------------------------------------------
	Vertex2Texture::Vertex2Texture(const vector4f& position, const vector4f& coordinate)
		:Vertex(position),
		m_Coordinate(coordinate)
	{
	}
	//-------------------------------------------------------------------
	Vertex2Texture::Vertex2Texture(const Vertex2Texture& vertex)
		:Vertex(vertex.m_Vertex),
		m_Coordinate(vertex.m_Coordinate)
	{
	}
	//-------------------------------------------------------------------
	VertexLayoutList Vertex2Texture::Layout()
	{
		VertexLayoutList list = Vertex::Layout();
		static VertexLayout vl(Attributes::ATTRIBUTE_TEXCOORD0, vector4f::Size(), Device::DT_FLOAT, vector4f::BitSize(), false );
		list.push_back(&vl);
		return list;
	}
	//-------------------------------------------------------------------
	Vertex2Texture& Vertex2Texture::operator = (const Vertex2Texture& rhs)
	{
		Vertex::operator =(rhs);
		m_Coordinate = rhs.m_Coordinate;
		return *this;
	}
	//-------------------------------------------------------------------





	/////////////////////////////////
	//		VertexTextureNormal	   //
	/////////////////////////////////
	//-------------------------------------------------------------------
	VertexTextureNormal::VertexTextureNormal()
		:VertexTexture()
	{
	}
	//-------------------------------------------------------------------
	VertexTextureNormal::VertexTextureNormal(const vector4f &position, const vector2f &coordinate, const vector3f &normal)
		:VertexTexture(position, coordinate),
		m_Normal(normal)
	{
	}
	//-------------------------------------------------------------------
	VertexTextureNormal::VertexTextureNormal(const VertexTextureNormal &vertex)
		:VertexTexture(vertex.m_Vertex, vertex.m_Coordinate),
		m_Normal(vertex.m_Normal)
	{
	}
	//-------------------------------------------------------------------
	VertexLayoutList VertexTextureNormal::Layout()
	{
		VertexLayoutList list = VertexTexture::Layout();
		static VertexLayout vl(Attributes::ATTRIBUTE_NORMAL, vector3f::Size(), Device::DT_FLOAT, vector3f::BitSize(), false );
		list.push_back(&vl);
		return list;
	}
	//-------------------------------------------------------------------
	VertexTextureNormal& VertexTextureNormal::operator = (const VertexTextureNormal& rhs)
	{
		VertexTexture::operator =(rhs);
		m_Normal = rhs.m_Normal;
		return *this;
	}
	//-------------------------------------------------------------------



	/////////////////////////////////
	//	Vertex2TextureNormal	   //
	/////////////////////////////////
	//-------------------------------------------------------------------
	Vertex2TextureNormal::Vertex2TextureNormal()
		:Vertex2Texture()
	{
	}
	//-------------------------------------------------------------------
	Vertex2TextureNormal::Vertex2TextureNormal(const vector4f &position, const vector4f &coordinate, const vector3f &normal)
		:Vertex2Texture(position, coordinate),
		m_Normal(normal)
	{
	}
	//-------------------------------------------------------------------
	Vertex2TextureNormal::Vertex2TextureNormal(const Vertex2TextureNormal &vertex)
		:Vertex2Texture(vertex.m_Vertex, vertex.m_Coordinate),
		m_Normal(vertex.m_Normal)
	{
	}
	//-------------------------------------------------------------------
	VertexLayoutList Vertex2TextureNormal::Layout()
	{
		VertexLayoutList list = Vertex2Texture::Layout();
		static VertexLayout vl(Attributes::ATTRIBUTE_NORMAL, vector3f::Size(), Device::DT_FLOAT, vector3f::BitSize(), false );
		list.push_back(&vl);
		return list;
	}
	//-------------------------------------------------------------------
	Vertex2TextureNormal& Vertex2TextureNormal::operator = (const Vertex2TextureNormal& rhs)
	{
		Vertex2Texture::operator =(rhs);
		m_Normal = rhs.m_Normal;
		return *this;
	}
	//-------------------------------------------------------------------



	/////////////////////////////////
	//	Vertex2TextureNormalTangent//
	/////////////////////////////////
	//-------------------------------------------------------------------
	Vertex2TextureNormalTangent::Vertex2TextureNormalTangent()
		:Vertex2TextureNormal()
	{
	}
	//-------------------------------------------------------------------
	Vertex2TextureNormalTangent::Vertex2TextureNormalTangent(const vector4f& position, const vector4f& coordinate, const vector3f& normal, const vector3f& tangent)
		:Vertex2TextureNormal(position, coordinate, normal),
		m_Tangent(tangent)
	{
	}
	//-------------------------------------------------------------------
	Vertex2TextureNormalTangent::Vertex2TextureNormalTangent(const Vertex2TextureNormalTangent &vertex)
		:Vertex2TextureNormal(vertex.m_Vertex, vertex.m_Coordinate, vertex.m_Normal),
		m_Tangent(vertex.m_Tangent)
	{
	}
	//-------------------------------------------------------------------
	VertexLayoutList Vertex2TextureNormalTangent::Layout()
	{
		VertexLayoutList list = Vertex2TextureNormal::Layout();
		static VertexLayout vl(Attributes::ATTRIBUTE_TANGENT, vector3f::Size(), Device::DT_FLOAT, vector3f::BitSize(), false );
		list.push_back(&vl);
		return list;
	}
	//-------------------------------------------------------------------
	Vertex2TextureNormalTangent& Vertex2TextureNormalTangent::operator = (const Vertex2TextureNormalTangent& rhs)
	{
		Vertex2TextureNormal::operator =(rhs);
		m_Tangent = rhs.m_Tangent;
		return *this;
	}
	//-------------------------------------------------------------------



	/////////////////////////////////
	//		VertexTextureColor	   //
	/////////////////////////////////
	//-------------------------------------------------------------------
	VertexTextureColor::VertexTextureColor()
		:VertexTexture()
	{
	}
	//-------------------------------------------------------------------
	VertexTextureColor::VertexTextureColor(const vector4f &position, const vector2f &coordinate, const Color& color)
		:VertexTexture(position, coordinate),
		m_Color(color)
	{
	}
	//-------------------------------------------------------------------
	VertexLayoutList VertexTextureColor::Layout()
	{
		VertexLayoutList list = VertexTexture::Layout();
		static VertexLayout vl(Attributes::ATTRIBUTE_COLOR0, Color::Size(), Device::DT_FLOAT, Color::BitSize(), false );
		list.push_back(&vl);
		return list;
	}
	//-------------------------------------------------------------------
	VertexTextureColor& VertexTextureColor::operator = (const VertexTextureColor& rhs)
	{
		VertexTexture::operator =(rhs);
		m_Color = rhs.m_Color;
		return *this;
	}
	//-------------------------------------------------------------------





	/////////////////////////////////
	//		VertexCubeTexture	   //
	/////////////////////////////////
	//-------------------------------------------------------------------
	VertexCubeTexture::VertexCubeTexture()
		:Vertex()
	{
	}
	//-------------------------------------------------------------------
	VertexCubeTexture::VertexCubeTexture(const vector4f& position, const vector3f& coordinate)
		:Vertex(position),
		m_Coordinate( coordinate )
	{
	}
	//-------------------------------------------------------------------
	VertexCubeTexture::VertexCubeTexture(const VertexCubeTexture& vertex)
		:Vertex(vertex.m_Vertex),
		m_Coordinate(vertex.m_Coordinate)
	{
	}
	//-------------------------------------------------------------------
	VertexLayoutList VertexCubeTexture::Layout()
	{
		VertexLayoutList list = Vertex::Layout();
		static VertexLayout vl(Attributes::ATTRIBUTE_TEXCOORD0, vector3f::Size(), Device::DT_FLOAT, vector3f::BitSize(), false );
		list.push_back(&vl);
		return list;
	}
	//-------------------------------------------------------------------
	VertexCubeTexture& VertexCubeTexture::operator = (const VertexCubeTexture& rhs)
	{
		Vertex::operator =(rhs);
		m_Coordinate = rhs.m_Coordinate;
		return *this;
	}
	//-------------------------------------------------------------------




	/////////////////////////////////////
	//			VertexLayout		   //
	/////////////////////////////////////
	//-------------------------------------------------------------------
	VertexTextureNormalTangent::VertexTextureNormalTangent()
		:VertexTextureNormal()
	{

	}
	//-------------------------------------------------------------------
	VertexTextureNormalTangent::VertexTextureNormalTangent(const vector4f& position, const vector2f& coordinate, const vector3f& normal, const vector4f& tangent)
		:VertexTextureNormal( position, coordinate, normal),
		m_Tangent(tangent)
	{

	}
	//-------------------------------------------------------------------
	VertexTextureNormalTangent::VertexTextureNormalTangent(const VertexTextureNormalTangent& vertex)
		:VertexTextureNormal( vertex ),
		m_Tangent(vertex.m_Tangent)
	{

	}
	//-------------------------------------------------------------------
	VertexTextureNormalTangent& VertexTextureNormalTangent::operator= (const VertexTextureNormalTangent& rhs)
	{
		VertexTextureNormal::operator =( rhs );
		m_Tangent = rhs.m_Tangent;
		return *this;
	}
	//-------------------------------------------------------------------
	VertexLayoutList VertexTextureNormalTangent::Layout()
	{
		VertexLayoutList list = VertexTextureNormal::Layout();
		static VertexLayout vl1(Attributes::ATTRIBUTE_TANGENT, vector4f::Size(), Device::DT_FLOAT, vector4f::BitSize(), false );
		list.push_back(&vl1);
		return list;
	}


	/////////////////////////////////////
	//			VertexLayout		   //
	/////////////////////////////////////
	//-------------------------------------------------------------------
	VertexTextureNormalTangentBinormal::VertexTextureNormalTangentBinormal()
		:VertexTextureNormalTangent()
	{

	}
	//-------------------------------------------------------------------
	VertexTextureNormalTangentBinormal::VertexTextureNormalTangentBinormal(const vector4f& position, const vector2f& coordinate, const vector3f& normal, const vector4f& tangent, const vector3f& binormal)
		:VertexTextureNormalTangent( position, coordinate, normal, tangent),
		m_Binormal(binormal)
	{

	}
	//-------------------------------------------------------------------
	VertexTextureNormalTangentBinormal::VertexTextureNormalTangentBinormal(const VertexTextureNormalTangentBinormal& vertex)
		:VertexTextureNormalTangent( vertex ),
		m_Binormal(vertex.m_Binormal)
	{

	}
	//-------------------------------------------------------------------
	VertexTextureNormalTangentBinormal& VertexTextureNormalTangentBinormal::operator= (const VertexTextureNormalTangentBinormal& rhs)
	{
		VertexTextureNormalTangent::operator =( rhs );
		m_Binormal = rhs.m_Binormal;
		return *this;
	}
	//-------------------------------------------------------------------
	VertexLayoutList VertexTextureNormalTangentBinormal::Layout()
	{
		VertexLayoutList list = VertexTextureNormalTangent::Layout();
		static VertexLayout vl1(Attributes::ATTRIBUTE_BINORMAL, vector3f::Size(), Device::DT_FLOAT, vector3f::BitSize(), false );
		list.push_back(&vl1);
		return list;
	}




	/////////////////////////////////////
	//	VertexTextureNormalBoneWeight	//
	/////////////////////////////////////
	//-------------------------------------------------------------------
	VertexTextureNormalBoneWeight::VertexTextureNormalBoneWeight()
		:VertexTextureNormal()
	{

	}
	//-------------------------------------------------------------------
	VertexTextureNormalBoneWeight::VertexTextureNormalBoneWeight(const vector4f& position, const vector2f& coordinate, const vector3f& normal, const vector4f& index, const vector4f& weight)
		:VertexTextureNormal( position, coordinate, normal),
		m_BoneIndex( index ),
		m_Weight(weight)
	{

	}
	//-------------------------------------------------------------------
	VertexTextureNormalBoneWeight::VertexTextureNormalBoneWeight(const VertexTextureNormalBoneWeight& vertex)
		:VertexTextureNormal( vertex ),
		m_BoneIndex( vertex.m_BoneIndex ),
		m_Weight(vertex.m_Weight)
	{

	}
	//-------------------------------------------------------------------
	VertexTextureNormalBoneWeight& VertexTextureNormalBoneWeight::operator= (const VertexTextureNormalBoneWeight& rhs)
	{
		VertexTextureNormal::operator =( rhs );
		m_Weight = rhs.m_Weight;
		m_BoneIndex = rhs.m_BoneIndex;
		return *this;
	}
	//-------------------------------------------------------------------
	VertexLayoutList VertexTextureNormalBoneWeight::Layout()
	{
		VertexLayoutList list = VertexTextureNormal::Layout();
		static VertexLayout vl1(Attributes::ATTRIBUTE_BONEINDICES, vector4f::Size(), Device::DT_FLOAT, vector4f::BitSize(), false );
		static VertexLayout vl2(Attributes::ATTRIBUTE_BONEWEIGHTS, vector4f::Size(), Device::DT_FLOAT, vector4f::BitSize(), false );
		list.push_back(&vl1);
		list.push_back(&vl2);
		return list;
	}



	/////////////////////////////////////////////////////////////
	//			VertexTextureNormalTangentBoneWeight		   //
	/////////////////////////////////////////////////////////////
	//-------------------------------------------------------------------
	VertexTextureNormalTangentBoneWeight::VertexTextureNormalTangentBoneWeight()
		:VertexTextureNormalTangent()
	{

	}
	//-------------------------------------------------------------------
	VertexTextureNormalTangentBoneWeight::VertexTextureNormalTangentBoneWeight(const vector4f& position, const vector2f& coordinate, const vector3f& normal, const vector4f& tangent, const vector4f& index, const vector4f& weight)
		:VertexTextureNormalTangent( position, coordinate, normal, tangent),
		m_BoneIndex(index),
		m_Weight(weight)
	{

	}
	//-------------------------------------------------------------------
	VertexTextureNormalTangentBoneWeight::VertexTextureNormalTangentBoneWeight(const VertexTextureNormalTangentBoneWeight& vertex)
		:VertexTextureNormalTangent( vertex ),
		m_BoneIndex(vertex.m_BoneIndex),
		m_Weight(vertex.m_Weight)
	{

	}
	//-------------------------------------------------------------------
	VertexTextureNormalTangentBoneWeight& VertexTextureNormalTangentBoneWeight::operator= (const VertexTextureNormalTangentBoneWeight& rhs)
	{
		VertexTextureNormalTangent::operator =( rhs );
		m_BoneIndex = rhs.m_BoneIndex;
		m_Weight = rhs.m_Weight;
		return *this;
	}
	//-------------------------------------------------------------------
	VertexLayoutList VertexTextureNormalTangentBoneWeight::Layout()
	{
		VertexLayoutList list = VertexTextureNormalTangent::Layout();
		static VertexLayout vl1(Attributes::ATTRIBUTE_BONEINDICES, vector4f::Size(), Device::DT_FLOAT, vector4f::BitSize(), false );
		static VertexLayout vl2(Attributes::ATTRIBUTE_BONEWEIGHTS, vector4f::Size(), Device::DT_FLOAT, vector4f::BitSize(), false );
		list.push_back(&vl1);
		list.push_back(&vl2);
		return list;
	}




	/////////////////////////////////////////////////////////////////////
	//			VertexTextureNormalTangentBinormalBoneWeight		   //
	/////////////////////////////////////////////////////////////////////
	//-------------------------------------------------------------------
	VertexTextureNormalTangentBinormalBoneWeight::VertexTextureNormalTangentBinormalBoneWeight()
		:VertexTextureNormalTangentBinormal()
	{

	}
	//-------------------------------------------------------------------
	VertexTextureNormalTangentBinormalBoneWeight::VertexTextureNormalTangentBinormalBoneWeight(const vector4f& position, const vector2f& coordinate, const vector3f& normal, const vector4f& tangent, const vector3f& binormal, const vector4f& index, const vector4f& weight)
		:VertexTextureNormalTangentBinormal( position, coordinate, normal, tangent, binormal),
		m_BoneIndex(index),
		m_Weight(weight)
	{

	}
	//-------------------------------------------------------------------
	VertexTextureNormalTangentBinormalBoneWeight::VertexTextureNormalTangentBinormalBoneWeight(const VertexTextureNormalTangentBinormalBoneWeight& vertex)
		:VertexTextureNormalTangentBinormal( vertex ),
		m_BoneIndex(vertex.m_BoneIndex),
		m_Weight(vertex.m_Weight)
	{

	}
	//-------------------------------------------------------------------
	VertexTextureNormalTangentBinormalBoneWeight& VertexTextureNormalTangentBinormalBoneWeight::operator= (const VertexTextureNormalTangentBinormalBoneWeight& rhs)
	{
		VertexTextureNormalTangentBinormal::operator =( rhs );
		m_BoneIndex = rhs.m_BoneIndex;
		m_Weight = rhs.m_Weight;
		return *this;
	}
	//-------------------------------------------------------------------
	VertexLayoutList VertexTextureNormalTangentBinormalBoneWeight::Layout()
	{
		VertexLayoutList list = VertexTextureNormalTangentBinormal::Layout();
		static VertexLayout vl1(Attributes::ATTRIBUTE_BONEINDICES, vector4f::Size(), Device::DT_FLOAT, vector4f::BitSize(), false );
		static VertexLayout vl2(Attributes::ATTRIBUTE_BONEWEIGHTS, vector4f::Size(), Device::DT_FLOAT, vector4f::BitSize(), false );
		list.push_back(&vl1);
		list.push_back(&vl2);
		return list;
	}






	/////////////////////////////////////
	//			VertexLayout		   //
	/////////////////////////////////////
	//-------------------------------------------------------------------
	VertexLayout::VertexLayout(Attributes::ShaderAttributes attr, uint size, Device::DataType type, uint stride, bool isnormal)
		:m_eAttributes(attr),
		m_uSize(size),
		m_eType(type),
		m_uStride(stride),
		m_isNormalize(isnormal)
	{
	}
	//-------------------------------------------------------------------
	VertexLayout::VertexLayout(const VertexLayout& input)
	{
		m_eAttributes = input.m_eAttributes;
		m_uSize = input.m_uSize;
		m_eType = input.m_eType;
		m_isNormalize = input.m_isNormalize;
		m_uStride = input.m_uStride;
	}
	//-------------------------------------------------------------------


}