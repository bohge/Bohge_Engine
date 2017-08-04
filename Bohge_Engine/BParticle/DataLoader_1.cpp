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
#include "DataLoader_1.h"


using namespace BohgeEngine;
namespace ParticleSystemPlugin
{
	//------------------------------------------------------------------------------
	JSON_DATA_STRUCT_IMPLEMENT_4( Affector_1,
		"Affect", JsonProperty::JDT_INT, &m_Affect,
		"DataType", JsonProperty::JDT_INT, &m_DataType,
		"Min", JsonProperty::JDT_OBJECT_PTR, &m_pMin,
		"Max", JsonProperty::JDT_OBJECT_PTR, &m_pMax);
	Affector_1::~Affector_1()
	{
		SAFE_DELETE( m_pMin );
		SAFE_DELETE( m_pMax )
	}
	void Affector_1::_OnDataLoadded_2()
	{
		switch( m_DataType )
		{
		case BohgeEngine::Math::DST_VECTOR1:
			{
				m_pMin = NEW BohgeEngine::JsonFloat;
				m_pMax = NEW BohgeEngine::JsonFloat;
			}break;
		case BohgeEngine::Math::DST_VECTOR3:
			{
				m_pMin = NEW BohgeEngine::JsonVector3f;
				m_pMax = NEW BohgeEngine::JsonVector3f;
			}break;
		default:ASSERT(false);
		}
	}

	//------------------------------------------------------------------------------
	JSON_DATA_STRUCT_IMPLEMENT_3( EmitterLoader_1,
		"Transform", JsonProperty::JDT_OBJECT, &m_Transform,
		"Material", JsonProperty::JDT_OBJECT, &m_Material,
		"Affector", JsonProperty::JDT_ARRAY_OBJECT, &m_AffectorArray );
}