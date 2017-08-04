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
#include "RandVolume.h"


using namespace BohgeEngine;
namespace ParticleSystemPlugin
{
	RTTI_TYPE_IMPLEMENT( RandVolume );
	//---------------------------------------------------------------------------------------------------------
	RandVolume::RandVolume(void)
		:m_pMin(NULL),
		m_pMax(NULL),
		m_pResult(NULL),
		m_pRandomFunc(NULL),
		m_eDataStructType( (BohgeEngine::Math::DataStructType)-1 ),
		m_Attributes( (BohgeEngine::Attributes::ShaderAttributes)-1 )
	{

	}
	//---------------------------------------------------------------------------------------------------------
	RandVolume::~RandVolume(void)
	{
		SAFE_DELETE( m_pMin );
		SAFE_DELETE( m_pMax );
		SAFE_DELETE( m_pResult );
	}
	//---------------------------------------------------------------------------------------------------------
	void RandVolume::_Vector1fRandom()
	{
		*(static_cast<vector1f*>( m_pResult )) =
			Math::Random( 
			*(static_cast<vector1f*>( m_pMin )), 
			*(static_cast<vector1f*>( m_pMax )) );
	}
	//---------------------------------------------------------------------------------------------------------
	void RandVolume::_Vector2fRandom()
	{
		*(static_cast<vector2f*>( m_pResult )) =
			Math::Random( 
			*(static_cast<vector2f*>( m_pMin )), 
			*(static_cast<vector2f*>( m_pMax )) );
	}
	//---------------------------------------------------------------------------------------------------------
	void RandVolume::_Vector3fRandom()
	{
		*(static_cast<vector3f*>( m_pResult )) =
			Math::Random( 
			*(static_cast<vector3f*>( m_pMin )), 
			*(static_cast<vector3f*>( m_pMax )) );
	}
	//---------------------------------------------------------------------------------------------------------
	void RandVolume::_Vector4fRandom()
	{
		*(static_cast<vector4f*>( m_pResult )) =
			Math::Random( 
			*(static_cast<vector4f*>( m_pMin )), 
			*(static_cast<vector4f*>( m_pMax )) );
	}
	//---------------------------------------------------------------------------------------------------------
	void RandVolume::SetRange( Math::DataStructType dst, const BohgeEngine::Volume& min, const BohgeEngine::Volume& max )
	{
		m_eDataStructType = dst;
		m_pMin = static_cast<Volume*>( min.Duplicate() );
		m_pMax = static_cast<Volume*>( max.Duplicate() );
		m_pResult = static_cast<Volume*>( min.Duplicate() );
		switch( dst )
		{
		case Math::DST_VECTOR1: m_pRandomFunc = &RandVolume::_Vector1fRandom; break;
		case Math::DST_VECTOR2: m_pRandomFunc = &RandVolume::_Vector2fRandom; break;
		case Math::DST_VECTOR3: m_pRandomFunc = &RandVolume::_Vector3fRandom; break;
		case Math::DST_VECTOR4: m_pRandomFunc = &RandVolume::_Vector4fRandom; break;
		default:ASSERT(false);
		}
	}
}