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
#include "Framework/JsonDefualtDataType.h"




namespace BGUI
{
	//------------------------------------------------------------------------------
	struct Widgets_1 : public BohgeEngine::IJsonDataSet
	{
		JSON_DATA_STRUCT_DECLEAR( Widgets_1, 2 );
		int								m_Type;
		BohgeEngine::IJsonDataSet*		m_pProperty;
	public:
		virtual ~Widgets_1();
	protected:
		virtual void _OnDataLoadded_1();
	};
	JSON_ARRAY_DATA_STRUCT( Widgets_1 );
	//------------------------------------------------------------------------------
	struct Panel_1 : public BohgeEngine::IJsonDataSet
	{
		JSON_DATA_STRUCT_DECLEAR( Panel_1, 4 );
		eastl::string					m_Name;
		uint							m_Adaptive;
		BohgeEngine::JsonRectangle		m_Rectangle;
		Widgets_1_Array					m_Child;
	};
	//------------------------------------------------------------------------------
	struct Label_1 : public BohgeEngine::IJsonDataSet
	{
		JSON_DATA_STRUCT_DECLEAR( Label_1, 6 );
		eastl::string					m_Name;
		uint							m_Adaptive;
		BohgeEngine::JsonRectangle		m_Rectangle;
		eastl::string					m_ImageSource;
		BohgeEngine::JsonUV				m_ImageUV;
		Widgets_1_Array					m_Child;
	};
	//------------------------------------------------------------------------------
	struct Button_1 : public BohgeEngine::IJsonDataSet
	{
		JSON_DATA_STRUCT_DECLEAR( Button_1, 7 );
		eastl::string					m_Name;
		uint							m_Adaptive;
		BohgeEngine::JsonRectangle		m_Rectangle;
		eastl::string					m_ImageSource;
		BohgeEngine::JsonUV_Array		m_ImageUVArray;
		eastl::string					m_Script;
		Widgets_1_Array					m_Child;
	};
	//------------------------------------------------------------------------------
	struct Text_1 : public BohgeEngine::IJsonDataSet
	{
		JSON_DATA_STRUCT_DECLEAR( Text_1, 9 );
		eastl::string				m_Name;
		eastl::string				m_Path;
		BohgeEngine::JsonRectangle	m_Rectangle;
		uint						m_Adaptive;
		BohgeEngine::JsonColor		m_Color;
		int							m_Size;
		int							m_ID;
		int							m_Align;
		Widgets_1_Array				m_Child;
	};
	//------------------------------------------------------------------------------
	struct Layout_1 : public BohgeEngine::IJsonDataSet
	{
		JSON_DATA_STRUCT_DECLEAR( Layout_1, 5 );
		eastl::string	m_Name;
		Widgets_1		m_Widget;
		uint			m_Depth;
		uint			m_Adaptive;
		BohgeEngine::JsonRectangle	m_Rectangle;
	};
	//------------------------------------------------------------------------------
	struct Windows_1 : public BohgeEngine::IJsonDataSet
	{
		JSON_DATA_STRUCT_DECLEAR( Windows_1, 2 );
		eastl::string				m_WidgetName;
		BohgeEngine::JsonVector2d	m_Position;
	};
	//------------------------------------------------------------------------------
	struct UIWindowsData_1 : public BohgeEngine::IJsonDataSet
	{
		JSON_DATA_STRUCT_DECLEAR( UIWindowsData_1, 3 );
		JSON_ARRAY_DATA_STRUCT( Windows_1 );
		BohgeEngine::JsonSize				m_Design;//…Ëº∆≥ﬂ¥Á
		BohgeEngine::JsonStringArray		m_Source;
		Windows_1_Array						m_WindowsArray;
	};

}