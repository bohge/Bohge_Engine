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



//////////////////////////////
//			Ðý×ª»·		   //
/////////////////////////////

#include "RotatingRing.h"
#include "Vertex.h"
#include "ShaderManage.h"
#include "RendBuffer.h"
#include "Shaders2D.h"

namespace BohgeEngine
{
	//------------------------------------------------------------------------------------------------------
	RotatingRing::RotatingRing()
		:LocatableObject( this ),
		m_isMaked(false),
		m_pRingBuffer(NULL),
		m_fRadius(0)
	{
		float bs = 1.5 * 1;
		m_BindBox = aabbox3df( vector3f( -bs, -bs, -bs ), vector3f( bs, bs, bs ) );
		m_isMaked = true;
		m_fRadius = 1;
		ASSERT( 3 == RC_AXIS );
		VertexColor LineVertex[ RC_AXIS * RC_POINT ];
		static ushort Indies[RC_AXIS*RC_POINT*2];
		for ( int i = 0 ; i < RC_AXIS*RC_POINT*2; i += 2 )
		{
			Indies[i] = i/2;
			//Indies[i+1] = (i/2)%RC_POINT==0&&0!=i ? i-RC_POINT*2 : i/2+1 ;
			if ( i/2 == RC_POINT-1 || i/2 == RC_POINT*2-1 || i/2 == RC_POINT*3-1 )
			{
				Indies[i+1] = i/2-RC_POINT+1;
			}
			else
			{
				Indies[i+1] = i/2+1;
			}
		}
		m_pRingBuffer = NEW RendBuffer( Device::RM_LINES );
		m_pVertexBuffer = NEW VertexBuffer( Device::MU_DYNAMIC );
		m_pIndicesBuffer = NEW IndicesBuffer( Device::MU_STATIC );
		m_pRingBuffer->SetVertexBuffer( m_pVertexBuffer );
		m_pRingBuffer->SetIndicesBuffer( m_pIndicesBuffer );
		m_pIndicesBuffer->FlushIndiesBuffer( RC_AXIS*RC_POINT*2, Indies );
	}
	//------------------------------------------------------------------------------------------------------
	RotatingRing::~RotatingRing()
	{
		SAFE_DELETE( m_pRingBuffer );
		SAFE_DELETE( m_pVertexBuffer );
		SAFE_DELETE( m_pIndicesBuffer );
	}
	//------------------------------------------------------------------------------------------------------
	void RotatingRing::CreateRing( const float& r )
	{
		float bs = 1.5 * r;
		m_BindBox = aabbox3df( vector3f( -bs, -bs, -bs ), vector3f( bs, bs, bs ) );
		m_isMaked = true;
		m_fRadius = r;
		ASSERT( 3 == RC_AXIS );
		VertexColor LineVertex[ RC_AXIS * RC_POINT ];
		for ( int i = 0; i < RC_POINT; i ++ )
		{
			Matrix22f m;
			m.FromRadian( (PI / 180) * i );
			vector2f v( r, 0 );
			v *= m;
			LineVertex[i].SetVertex( vector4f( v.m_x, 0, v.m_y, 1 ) );//Î§ÈÆyÖá
			LineVertex[i].SetColor( Color( 1, 0, 0, 1) );

			LineVertex[i+RC_POINT].SetVertex( vector4f( 0, v.m_x, v.m_y, 1 ) );//Î§ÈÆxÖá
			LineVertex[i+RC_POINT].SetColor( Color( 0, 1, 0, 1) );

			LineVertex[i+2*RC_POINT].SetVertex( vector4f( v.m_x, v.m_y, 0, 1 ) );//Î§ÈÆxÖá
			LineVertex[i+2*RC_POINT].SetColor( Color( 0, 0, 1, 1) );
		}
		m_pVertexBuffer->FlushVertexBuffer( RC_AXIS * RC_POINT, LineVertex );
	}
	//------------------------------------------------------------------------------------------------------
	void RotatingRing::Update( uint milisecon )
	{
		LocatableObject::Update( milisecon );
		m_Transform = Math::Transformation<float>( NULL, NULL, NULL, NULL, NULL, &GetWorldPosition() );
	}
	//------------------------------------------------------------------------------------------------------
	void RotatingRing::EndRender( Engine& engine )
	{
		if ( m_isMaked )
		{
			engine.GetDevice()->DisableDepthTest();
			engine.GetDevice()->DepthMask( true );
			WorldShapes& ss = engine.GetShaderManage()->GetShader<WorldShapes>(ShaderManage::WorldShapes);
			ss.SetParamTransform( &m_Transform );
			engine.GetDevice()->Draw( *m_pRingBuffer, ss, NULL );
		}
	}
	//------------------------------------------------------------------------------------------------------
	bool RotatingRing::isInview( const Frustumf& cFru )
	{
		return IntersectedType::IT_NO != cFru.Intersect( m_BindBox * m_Transform );
	}
	//------------------------------------------------------------------------------------------------------
	const void* RotatingRing::Attribute( const Node_Attribute& name ) const
	{
		switch( name )
		{
		case IAttributeNode::ATT_WORLDTRANSFORM: return &m_Transform;
		}
		return NULL;
	}














	//////////////////////////////
	//			Ðý×ª»·		   //
	/////////////////////////////
	//------------------------------------------------------------------------------------------------------
	void SelectableRotatingRing::CreateRing( const float& r )
	{
		float bs = 1.5 * r;
		float sbs = r * 0.1;
		m_XBind = aabbox3df( vector3f(sbs,bs,bs), vector3f(-sbs,-bs,-bs) );
		m_YBind = aabbox3df( vector3f(bs,sbs,bs), vector3f(-bs,-sbs,-bs) );
		m_ZBind = aabbox3df( vector3f(bs,bs,sbs), vector3f(-bs,-bs,-sbs) );
		RotatingRing::CreateRing( r );
	}
	//------------------------------------------------------------------------------------------------------
	void SelectableRotatingRing::EndRender( Engine& engine )
	{
		RotatingRing::EndRender( engine );
#ifdef DRAWAABBOX
		m_XBind.Render( engine );
		m_YBind.Render( engine );
		m_ZBind.Render( engine );
#endif
	}
	//------------------------------------------------------------------------------------------------------
	SelectableRotatingRing::Selected SelectableRotatingRing::IsSelected( const Rayf& r )
	{
		aabbox3df local = m_BindBox;
		local.SetCenter( GetWorldPosition() );
		m_XBind.SetCenter( GetWorldPosition() );
		m_YBind.SetCenter( GetWorldPosition() );
		m_ZBind.SetCenter( GetWorldPosition() );
		if ( ( local ).Intersection( r ) )
		{
			std::map< float, SelectableRotatingRing::Selected > rm;
			vector3f point;
			if(( m_XBind ).IntersectPointWithRay( r, point ) ) rm.insert( std::make_pair( (point - r.GetRayOrigin()).LengthPow(), SR_BY_X_AXIS ) );
			if(( m_YBind ).IntersectPointWithRay( r, point ) ) rm.insert( std::make_pair( (point - r.GetRayOrigin()).LengthPow(), SR_BY_Y_AXIS ) );
			if(( m_ZBind ).IntersectPointWithRay( r, point ) ) rm.insert( std::make_pair( (point - r.GetRayOrigin()).LengthPow(), SR_BY_Z_AXIS ) );
			if ( rm.empty() )
			{
				return SR_NON;
			}
			return rm.begin()->second;
		}
		return SR_NON;
	}

}