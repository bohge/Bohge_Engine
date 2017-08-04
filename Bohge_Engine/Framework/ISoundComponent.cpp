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
#include "ISoundComponent.h"
#include "ISoundSystem.h"
#include "Node.h"


namespace BohgeEngine
{
	//-------------------------------------------------------------------------------------------------------
	ISoundComponent::ISoundComponent( void )
		:m_nFlag(0),
		m_fVolume(0.8f),
		m_fPitch(1.0f)
	{

	}
	//-------------------------------------------------------------------------------------------------------
	ISoundComponent::~ISoundComponent( void )
	{

	}
	//-------------------------------------------------------------------------------------------------------
	void ISoundComponent::_DoOnAttachNode()
	{
		//连接变更监听
		_HostNode()->RegisterMessageListener( NodeProperty::SA_POSITION, MEMBER_FUNC_PTR( &ISoundComponent::MessageRotation ) );
		_HostNode()->RegisterMessageListener( NodeProperty::SA_ROTATION, MEMBER_FUNC_PTR( &ISoundComponent::MessagePosition ) );
		_HostNode()->RegisterMessageListener( NodeProperty::SA_VELOCITY, MEMBER_FUNC_PTR( &ISoundComponent::MessageVelocity ) );
	}
	//-------------------------------------------------------------------------------------------------------
	void ISoundComponent::OnRootNodeChange()
	{
		const vector3f* pos = _HostNode()->Attribute<vector3f>( NodeProperty::GA_WORLD_POSITION );
		if ( pos )
		{
			_DoSetSoundPosition( *pos );
		}

		const vector3f* vel = _HostNode()->Attribute<vector3f>( NodeProperty::GA_WORLD_VELOCITY );
		if ( vel )
		{
			_DoSetSoundVelocity( *vel );
		}

		const Quaternionf* rot = _HostNode()->Attribute<Quaternionf>( NodeProperty::GA_WORLD_ROTATION );
		if ( rot )
		{
			vector3f dir = vector3f(0,0,1) * *rot;
			_DoSetSoundOrientation( dir );
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void ISoundComponent::MessageRotation( const NodeMessage& message )
	{
		const Quaternionf* rot = _HostNode()->Attribute<Quaternionf>( NodeProperty::GA_WORLD_ROTATION );
		if ( rot )
		{
			vector3f dir = vector3f(0,0,1) * *rot;
			_DoSetSoundOrientation( dir );
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void ISoundComponent::MessagePosition( const NodeMessage& message )
	{
		const vector3f* pos = _HostNode()->Attribute<vector3f>( NodeProperty::GA_WORLD_POSITION );
		if ( pos )
		{
			_DoSetSoundPosition( *pos );
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void ISoundComponent::MessageVelocity( const NodeMessage& message )
	{
		const vector3f* vel = _HostNode()->Attribute<vector3f>( NodeProperty::GA_WORLD_VELOCITY );
		if ( vel )
		{
			_DoSetSoundVelocity( *vel );
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void ISoundComponent::Release()
	{
		ISoundSystem::Instance()->UnloadSound( this );
	}
}