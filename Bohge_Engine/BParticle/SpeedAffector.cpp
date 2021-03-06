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
#include "Framework/OrientationComponent.h"
#include "Framework/VertexStream.h"
#include "SpeedAffector.h"



using namespace BohgeEngine;
namespace ParticleSystemPlugin
{
	RTTI_TYPE_IMPLEMENT( SpeedAffector );
	//---------------------------------------------------------------------------------------------------------
	SpeedAffector::SpeedAffector(void)
		:m_rpVolume( NULL ),
		m_rpOrientationComponent(NULL)
	{

	}
	//---------------------------------------------------------------------------------------------------------
	SpeedAffector::~SpeedAffector(void)
	{
	}
	//---------------------------------------------------------------------------------------------------------
	const BohgeEngine::Volume& SpeedAffector::GetAffectorVolume()
	{
		const vector3f& forward = m_rpOrientationComponent->GetWorldForward();
		const Volume& volume = m_rpVolume->GetRandomVolume();
		ASSERT( volume.GetTypeID() == vector1f::RTTI().GetTypeID() );
		m_Speed = forward * static_cast<const vector1f&>(volume);
		return m_Speed;
	}
	//---------------------------------------------------------------------------------------------------------
	BohgeEngine::Attributes::ShaderAttributes SpeedAffector::GetAttributes()
	{
		return Attributes::ATTRIBUTE_EMITTER_SPEED;
	}

}