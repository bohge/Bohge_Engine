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
#include "SpeedAxisAffector.h"



using namespace BohgeEngine;
namespace ParticleSystemPlugin
{
	RTTI_TYPE_IMPLEMENT( SpeedAxisAffector );
	//---------------------------------------------------------------------------------------------------------
	SpeedAxisAffector::SpeedAxisAffector(void)
		:m_rpAxis(NULL),
		m_rpSpeed(NULL),
		m_rpOrientationComponent(NULL)
	{

	}
	//---------------------------------------------------------------------------------------------------------
	SpeedAxisAffector::~SpeedAxisAffector(void)
	{
	}
	//---------------------------------------------------------------------------------------------------------
	const BohgeEngine::Volume& SpeedAxisAffector::GetAffectorVolume()
	{
		const vector3f& forward = m_rpOrientationComponent->GetWorldForward();
		{
			vector3f axis( forward );
			{
				Quaternionf qua;
				const Volume& axisVolume = m_rpAxis->GetRandomVolume();
				ASSERT( axisVolume.GetTypeID() == vector3f::RTTI().GetTypeID() );
				const vector3f& angle = static_cast<const vector3f&>(axisVolume);
				qua.RotateXYZ( angle.m_x, angle.m_y, angle.m_z );
				axis *= qua;
			}
			const Volume& speedVolume = m_rpSpeed->GetRandomVolume();
			ASSERT( speedVolume.GetTypeID() == vector1f::RTTI().GetTypeID() );
			m_Speed = axis * static_cast<const vector1f&>(speedVolume);
		}
		return m_Speed;
	}
	//---------------------------------------------------------------------------------------------------------
	BohgeEngine::Attributes::ShaderAttributes SpeedAxisAffector::GetAttributes()
	{
		return Attributes::ATTRIBUTE_EMITTER_SPEED;
	}

}