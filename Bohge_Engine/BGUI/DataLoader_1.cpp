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
#include "WidgetProperty.h"
#include "DataLoader_1.h"


using namespace eastl;
using namespace BohgeEngine;
namespace BGUI
{
	//------------------------------------------------------------------------------
	JSON_DATA_STRUCT_IMPLEMENT_2( Widgets_1,
		"Type", JsonProperty::JDT_INT, &m_Type,
		"Property", JsonProperty::JDT_OBJECT_PTR, &m_pProperty);
	Widgets_1::~Widgets_1(){ SAFE_DELETE( m_pProperty ) }
	void Widgets_1::_OnDataLoadded_1()
	{
		switch( m_Type )
		{
		case WidgetProperty::WT_PANEL:m_pProperty = NEW Panel_1;break;
		case WidgetProperty::WT_LABEL:m_pProperty = NEW Label_1;break;
		case WidgetProperty::WT_BUTTON:m_pProperty = NEW Button_1;break;
		case WidgetProperty::WT_STRING:m_pProperty = NEW Text_1;break;
		default:ASSERT(false);
		}
	}
	//------------------------------------------------------------------------------
	JSON_DATA_STRUCT_IMPLEMENT_4( Panel_1,
		"Name", JsonProperty::JDT_STRING, &m_Name,
		"Adaptive", JsonProperty::JDT_INT, &m_Adaptive,
		"Rectangle", JsonProperty::JDT_OBJECT, &m_Rectangle,
		"Child", JsonProperty::JDT_ARRAY_OBJECT, &m_Child);
	//------------------------------------------------------------------------------
	JSON_DATA_STRUCT_IMPLEMENT_6( Label_1,
		"Name", JsonProperty::JDT_STRING, &m_Name,
		"Adaptive", JsonProperty::JDT_INT, &m_Adaptive,
		"Rectangle", JsonProperty::JDT_OBJECT, &m_Rectangle,
		"ImageSource", JsonProperty::JDT_STRING, &m_ImageSource,
		"ImageUV", JsonProperty::JDT_OBJECT, &m_ImageUV,
		"Child", JsonProperty::JDT_ARRAY_OBJECT, &m_Child);
	//------------------------------------------------------------------------------
	JSON_DATA_STRUCT_IMPLEMENT_7( Button_1,
		"Name", JsonProperty::JDT_STRING, &m_Name,
		"Adaptive", JsonProperty::JDT_INT, &m_Adaptive,
		"Rectangle", JsonProperty::JDT_OBJECT, &m_Rectangle,
		"ImageSource", JsonProperty::JDT_STRING, &m_ImageSource,
		"ImageUVArray", JsonProperty::JDT_ARRAY_OBJECT, &m_ImageUVArray,
		"Script", JsonProperty::JDT_STRING, &m_Script,
		"Child", JsonProperty::JDT_ARRAY_OBJECT, &m_Child);
	//------------------------------------------------------------------------------
	JSON_DATA_STRUCT_IMPLEMENT_9( Text_1,
		"Name", JsonProperty::JDT_STRING, &m_Name,
		"StringID", JsonProperty::JDT_INT, &m_ID,
		"FontSize", JsonProperty::JDT_INT, &m_Size,
		"Align", JsonProperty::JDT_INT, &m_Align,
		"Adaptive", JsonProperty::JDT_INT, &m_Adaptive,
		"Rectangle", JsonProperty::JDT_OBJECT, &m_Rectangle,
		"Color", JsonProperty::JDT_OBJECT, &m_Color,
		"Text", JsonProperty::JDT_STRING, &m_Path,
		"Child", JsonProperty::JDT_ARRAY_OBJECT, &m_Child);
	//------------------------------------------------------------------------------
	JSON_DATA_STRUCT_IMPLEMENT_5( Layout_1,
		"Name", JsonProperty::JDT_STRING, &m_Name,
		"Widgets", JsonProperty::JDT_OBJECT, &m_Widget,
		"Depth", JsonProperty::JDT_INT, &m_Depth,
		"Adaptive", JsonProperty::JDT_INT, &m_Adaptive,
		"Rectangle", JsonProperty::JDT_OBJECT, &m_Rectangle
		);
	//------------------------------------------------------------------------------
	JSON_DATA_STRUCT_IMPLEMENT_2( Windows_1,
		"Widget", JsonProperty::JDT_STRING, &m_WidgetName,
		"Position", JsonProperty::JDT_OBJECT, &m_Position );
	//------------------------------------------------------------------------------
	JSON_DATA_STRUCT_IMPLEMENT_3( UIWindowsData_1,
		"Design", JsonProperty::JDT_OBJECT, &m_Design,
		"Source", JsonProperty::JDT_ARRAY_STRING, &m_Source,
		"Layout", JsonProperty::JDT_ARRAY_OBJECT, &m_WindowsArray );
}