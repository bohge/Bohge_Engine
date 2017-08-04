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
#include "Framework/ShaderAttributes.h"
#include "Framework/3DMath.h"


namespace ParticleSystemPlugin
{
	class RandVolume : public BohgeEngine::Object
	{
		RTTI_TYPE_DECLEAR( RandVolume );
	private:
		typedef void ( RandVolume::*RandomFunc )( void );
	private:
		RandomFunc				m_pRandomFunc;
		BohgeEngine::Volume*	m_pMin;
		BohgeEngine::Volume*	m_pMax;
		BohgeEngine::Volume*	m_pResult;
		BohgeEngine::Math::DataStructType		m_eDataStructType;
		BohgeEngine::Attributes::ShaderAttributes	m_Attributes;
	public:
		RandVolume(void);
		~RandVolume(void);
	private:
		void _Vector1fRandom();
		void _Vector2fRandom();
		void _Vector3fRandom();
		void _Vector4fRandom();
	public:
		void SetRange( BohgeEngine::Math::DataStructType dst, const BohgeEngine::Volume& min, const BohgeEngine::Volume& max );
	public:
		BOHGE_FORCEINLINE const BohgeEngine::Volume& GetRandomVolume()
		{
			(this->*m_pRandomFunc)();
			return *m_pResult;
		}
		BOHGE_FORCEINLINE void SetAttributes( BohgeEngine::Attributes::ShaderAttributes att )
		{
			m_Attributes = att;
		}
		BOHGE_FORCEINLINE BohgeEngine::Attributes::ShaderAttributes GetAttributes() const
		{
			return m_Attributes;
		}
		BOHGE_FORCEINLINE BohgeEngine::Math::DataStructType GetDataStructType() const
		{
			return m_eDataStructType;
		}
	};
}