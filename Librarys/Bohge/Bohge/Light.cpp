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



/////////////////////
//		灯光	   //
/////////////////////

#include "Light.h"
#include "Environment.h"
#include "Engine.h"
#include "ShadowCaster.h"
#include "Billboard.h"
#include "Texture.h"
#include "Device.h"
#include "ShaderManager.h"
#include "RendBuffer.h"
#include "EnvironmentShaders.h"


namespace BohgeEngine
{
	//-------------------------------------------------------------------------------------------------------
	LightNode::LightNode( LightType lt, LightStyle la )
		:LocatableObject(this),
		m_pShadowCaster(NULL),
		m_eLightType( lt ),
		m_eFakeReal( la ),
		m_HaloSize( 0.12f, 0.25f ),
		m_isActive(true),
		m_LightRange(1.0f),
		m_pLighthalo(NULL),
		m_MaxScale(1.0),
		m_isCastShadow(false),
		m_Color(0.2,0.2,0.2),
		m_LightAngle(PI/4,PI/3),
		m_LightAttrib(0),
		m_isLightChange(true),
		m_Attenuation(1,0,0,0)
	{
		SetRender(false);
		Engine::Instance().GetEnvironment()->_AddLightNode( this );
		m_pLighthalo = NEW Billboard;
		m_pLighthalo->SetBillCoordnate( vector2f(0,0), vector2f(1,1) );
	}
	//-------------------------------------------------------------------------------------------------------
	LightNode::~LightNode()
	{
		SAFE_DELETE( m_pLighthalo );
		Engine::Instance().GetEnvironment()->_RemoveLightNode( this );
	}
	//-------------------------------------------------------------------------------------------------------
	void LightNode::Update( uint milisecon )
	{
		if( isChange() || isFatherChange() || m_isLightChange )
		{
			if ( isChange() || isFatherChange() )
			{
				LocatableObject::Update( milisecon );
				vector3f ws = GetWorldScale();
				m_MaxScale = Math::GetMin( ws.m_x, Math::GetMin( ws.m_y, ws.m_z ) );
				if ( m_eLightType != LT_POINT )
				{
					m_WorldDirection = LightDirectionRaw * GetWorldRotation();
					m_WorldUp = LightUpRaw * GetWorldRotation();
				}
				CastChangedMessage();//通知子节点
				_SetChangeFlag( false );
			}
			m_isLightChange = false;
			_OnLightChangeRecaculater();//告诉自己的继承类，灯光变了
		}
	}
	//-------------------------------------------------------------------------------------------------------
	const void* LightNode::Attribute( const Node_Attribute& name ) const
	{
		return LocatableObject::LocatableAttribute( name );
	}
	//-------------------------------------------------------------------------------------------------------
	void LightNode::_OnLightChangeRecaculater()
	{
		float size = GetRange() * GetHaloSize().m_x;
		m_pLighthalo->SetBillBorad( GetWorldPosition(), 
			vector2f( size, size * GetHaloSize().m_y ) );
	}
	//-------------------------------------------------------------------------------------------------------
	void LightNode::GetHalo( SharedBuffer& input )
	{
		m_pLighthalo->GetVertexBuffer( input );
	}
	//-------------------------------------------------------------------------------------------------------
	void LightNode::SetLocalRotation( const Quaternionf& r )
	{
		_SetChangeFlag( true );
		LocatableObject::SetLocalRotation( r );
	}
	//-------------------------------------------------------------------------------------------------------
	void LightNode::SetAttribute( const LightAttribute& att )
	{
		if ( !isAttribute( att) )
		{
			m_LightAttrib |= att;
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void LightNode::EraseAttribute( const LightAttribute& att )
	{
		if ( isAttribute( att) )
		{
			m_LightAttrib &= ( ~att );
		}
	}





	//-------------------------------------------------------------------------------------------------------
	RealLightNode::RealLightNode( LightType lt )
		:LightNode( lt, LS_REAL )
	{
	}
	//-------------------------------------------------------------------------------------------------------
	RealLightNode::~RealLightNode()
	{
		SAFE_DELETE( m_pShadowCaster );
	}
	//-------------------------------------------------------------------------------------------------------
	void RealLightNode::SetAttribute( const LightAttribute& att )
	{
		if ( LA_SHADOW == att && !isAttribute(att) )
		{
			_NewShadowCaster();
		}
		LightNode::SetAttribute( att );
	}
	//-------------------------------------------------------------------------------------------------------
	void RealLightNode::EraseAttribute( const LightAttribute& att )
	{
		LightNode::EraseAttribute( att );
		if ( LA_SHADOW == att )
		{
			SAFE_DELETE( m_pShadowCaster );
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void RealLightNode::Update( uint milisecon )
	{
		bool ischange = isChange();
		LightNode::Update( milisecon );
		m_isCastShadow = false;
		if( m_pShadowCaster )
		{
			m_pShadowCaster->Update( *this, *Engine::Instance().GetCamera(), ischange );
			m_isCastShadow = m_pShadowCaster->isCastShadow();
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void RealLightNode::CastDepth( Engine& engine )
	{
		if( m_pShadowCaster )
		{
			m_pShadowCaster->CastDepth( engine );
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void RealLightNode::Render( Engine& engine )
	{

	}
	//-------------------------------------------------------------------------------------------------------
	void RealLightNode::_OnLightChangeRecaculater()
	{
		if( m_pShadowCaster )
		{
			m_pShadowCaster->OnLightChange( *this );
		}
	}

	//-------------------------------------------------------------------------------------------------------






	//-------------------------------------------------------------------------------------------------------
	FakeLightNode::FakeLightNode( LightType type )
		:LightNode( type, LS_FAKE ),
		m_pTexture(NULL),
		m_pRendBuffer(NULL),
		m_pVertexBuffer(NULL),
		m_pIndicesBuffer(NULL)
	{
		m_pTexture = NEW Texture2D();
	}
	//-------------------------------------------------------------------------------------------------------
	FakeLightNode::~FakeLightNode()
	{
		SAFE_DELETE( m_pTexture );
		SAFE_DELETE( m_pRendBuffer );
		SAFE_DELETE( m_pVertexBuffer );
		SAFE_DELETE( m_pIndicesBuffer );
	}
	//-------------------------------------------------------------------------------------------------------
	void FakeLightNode::CastDepth( Engine& engine )
	{

	}
	//-------------------------------------------------------------------------------------------------------
	void FakeLightNode::Render( Engine& engine )
	{
		FakeLightShader& fls = engine.GetShaderManager()->GetShader<FakeLightShader>(ShaderManager::FakeLightShader);
		fls.SetColor( GetColor() );
		engine.GetDevice()->Draw( *m_pRendBuffer, fls, m_pTexture );
	}
	//-------------------------------------------------------------------------------------------------------






	/////////////////////////////
	//		预渲染后的光	   //
	/////////////////////////////
	//-------------------------------------------------------------------------------------------------------
	LightMapedNode::LightMapedNode( LightType lt )
		:LightNode( lt, LS_LIGHTMAPED )
	{

	}
	//-------------------------------------------------------------------------------------------------------
	LightMapedNode::~LightMapedNode()
	{

	}
	//-------------------------------------------------------------------------------------------------------
	void LightMapedNode::CastDepth( Engine& engine )
	{

	}
	//-------------------------------------------------------------------------------------------------------
	void LightMapedNode::Render( Engine& engine )
	{

	}
	//-------------------------------------------------------------------------------------------------------

}