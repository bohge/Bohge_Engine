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
#include "Framework/TextProperty.h"
#include "DataLoader_1.h"



using namespace eastl;
using namespace BohgeEngine;
namespace BTextSystemPlugin
{
	JSON_DATA_STRUCT_IMPLEMENT_2( Text_1,
		"ID", JsonProperty::JDT_INT, &m_ID,
		TextProperty::DEFAULT_LANGUANGE, JsonProperty::JDT_STRING, &m_Text);
	void Text_1::SetLanguage( const char* lang )
	{
		m_DataMarkArray[1].SetName( lang );
	}
	//------------------------------------------------------------------------------
	JSON_DATA_STRUCT_IMPLEMENT_2( TextSet_1,
		"Font", JsonProperty::JDT_STRING, &m_Font,
		"TextSet", JsonProperty::JDT_ARRAY_OBJECT, &m_Texts );
}