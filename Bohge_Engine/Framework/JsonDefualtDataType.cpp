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
#include "JsonDefualtDataType.h"




namespace BohgeEngine
{
	JSON_DATA_STRUCT_IMPLEMENT_4( JsonRectangle,
		"x", JsonProperty::JDT_INT, &m_Rect.m_x,
		"y", JsonProperty::JDT_INT, &m_Rect.m_y,
		"w", JsonProperty::JDT_INT, &m_Rect.m_z,
		"h", JsonProperty::JDT_INT, &m_Rect.m_w );

	JSON_DATA_STRUCT_IMPLEMENT_2( JsonSize,
		"w", JsonProperty::JDT_INT, &m_Size.m_x,
		"h", JsonProperty::JDT_INT, &m_Size.m_y );

	JSON_DATA_STRUCT_IMPLEMENT_4( JsonUV,
		"us", JsonProperty::JDT_FLOAT, &m_UVStartEnd.m_x,
		"vs", JsonProperty::JDT_FLOAT, &m_UVStartEnd.m_y,
		"ue", JsonProperty::JDT_FLOAT, &m_UVStartEnd.m_z,
		"ve", JsonProperty::JDT_FLOAT, &m_UVStartEnd.m_w );

	JSON_DATA_STRUCT_IMPLEMENT_1( JsonInt,
		"n", JsonProperty::JDT_INT, &m_Data );

	JSON_DATA_STRUCT_IMPLEMENT_1( JsonFloat,
		"n", JsonProperty::JDT_FLOAT, &m_Data );

	JSON_DATA_STRUCT_IMPLEMENT_2( JsonVector2d,
		"x", JsonProperty::JDT_INT, &m_Position.m_x,
		"y", JsonProperty::JDT_INT, &m_Position.m_y );

	JSON_DATA_STRUCT_IMPLEMENT_2( JsonVector2f,
		"x", JsonProperty::JDT_FLOAT, &m_Vector2f.m_x,
		"y", JsonProperty::JDT_FLOAT, &m_Vector2f.m_y );

	JSON_DATA_STRUCT_IMPLEMENT_3( JsonVector3f,
		"x", JsonProperty::JDT_FLOAT, &m_Vector3f.m_x,
		"y", JsonProperty::JDT_FLOAT, &m_Vector3f.m_y,
		"z", JsonProperty::JDT_FLOAT, &m_Vector3f.m_z );

	JSON_DATA_STRUCT_IMPLEMENT_4( JsonVector4f,
		"x", JsonProperty::JDT_FLOAT, &m_Vector4f.m_x,
		"y", JsonProperty::JDT_FLOAT, &m_Vector4f.m_y,
		"z", JsonProperty::JDT_FLOAT, &m_Vector4f.m_z,
		"w", JsonProperty::JDT_FLOAT, &m_Vector4f.m_w );

	JSON_DATA_STRUCT_IMPLEMENT_4( JsonQuaternionf,
		"x", JsonProperty::JDT_FLOAT, &m_Quaternionf.m_x,
		"y", JsonProperty::JDT_FLOAT, &m_Quaternionf.m_y,
		"z", JsonProperty::JDT_FLOAT, &m_Quaternionf.m_z,
		"w", JsonProperty::JDT_FLOAT, &m_Quaternionf.m_w );

	JSON_DATA_STRUCT_IMPLEMENT_4( JsonColor,
		"r", JsonProperty::JDT_FLOAT, &m_Color.m_x,
		"g", JsonProperty::JDT_FLOAT, &m_Color.m_y,
		"b", JsonProperty::JDT_FLOAT, &m_Color.m_z,
		"a", JsonProperty::JDT_FLOAT, &m_Color.m_w );
}