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
#include "JsonData.h"
#include "3DMath.h"



#include <EASTL/string.h>
namespace BohgeEngine
{
	typedef eastl::vector< eastl::string > JsonStringArray;
	typedef eastl::vector< int > JsonIntArray;
	typedef eastl::vector< float > JsonFloatArray;
	
	struct BOHGE_FRAMEWORK JsonRectangle : public IJsonDataSet
	{
		JSON_DATA_STRUCT_DECLEAR( JsonRectangle, 4 );
		vector4d			m_Rect;
	};

	struct BOHGE_FRAMEWORK JsonSize : public IJsonDataSet
	{
		JSON_DATA_STRUCT_DECLEAR( JsonSize, 2 );
		vector2d			m_Size;
	};

	struct BOHGE_FRAMEWORK JsonUV : public IJsonDataSet
	{
		JSON_DATA_STRUCT_DECLEAR( JsonUV, 4 );
		vector4f			m_UVStartEnd;
	};
	JSON_ARRAY_DATA_STRUCT( JsonUV );

	struct BOHGE_FRAMEWORK JsonInt : public IJsonDataSet
	{
		JSON_DATA_STRUCT_DECLEAR( JsonInt, 1 );
		int		m_Data;
	};

	struct BOHGE_FRAMEWORK JsonFloat : public IJsonDataSet
	{
		JSON_DATA_STRUCT_DECLEAR( JsonFloat, 1 );
		float		m_Data;
	};

	struct BOHGE_FRAMEWORK JsonVector2d : public IJsonDataSet
	{
		JSON_DATA_STRUCT_DECLEAR( JsonVector2d, 2 );
		vector2d			m_Position;
	};

	struct BOHGE_FRAMEWORK JsonVector2f : public IJsonDataSet
	{
		JSON_DATA_STRUCT_DECLEAR( JsonVector2f, 2 );
		vector2f			m_Vector2f;
	};

	struct BOHGE_FRAMEWORK JsonVector3f : public IJsonDataSet
	{
		JSON_DATA_STRUCT_DECLEAR( JsonVector3f, 3 );
		vector3f			m_Vector3f;
	};

	struct BOHGE_FRAMEWORK JsonVector4f : public IJsonDataSet
	{
		JSON_DATA_STRUCT_DECLEAR( JsonVector4f, 4 );
		vector4f			m_Vector4f;
	};

	struct BOHGE_FRAMEWORK JsonQuaternionf : public IJsonDataSet
	{
		JSON_DATA_STRUCT_DECLEAR( JsonQuaternionf, 4 );
		Quaternionf			m_Quaternionf;
	};

	struct BOHGE_FRAMEWORK JsonColor : public IJsonDataSet
	{
		JSON_DATA_STRUCT_DECLEAR( JsonColor, 4 );
		Color			m_Color;
	};
}

