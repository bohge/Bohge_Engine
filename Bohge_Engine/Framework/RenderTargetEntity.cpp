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
#include "RenderTargetEntity.h"
#include "RenderTargetSource.h"




namespace BohgeEngine
{
	RTTI_TYPE_IMPLEMENT( RenderTargetProperty );
	RenderTargetEntity* RenderTargetEntity::s_CurrentRT = NULL;
	RTTI_TYPE_IMPLEMENT( RenderTargetEntity );
	//------------------------------------------------------------------------------------------------------
	RenderTargetEntity::RenderTargetEntity(void)
		:m_eTargetFace( RenderTargetProperty::TF_DEFAULT ),
		m_eTargetType( RenderTargetProperty::RT_RENDER_TARGET_2D ),
		m_isListenResizeview(false)
	{
	}
	//------------------------------------------------------------------------------------------------------
	RenderTargetEntity::~RenderTargetEntity(void)
	{		
	}
	//------------------------------------------------------------------------------------------------------
	void RenderTargetEntity::ListenResizeview()
	{
		m_isListenResizeview = true;
	}
	//------------------------------------------------------------------------------------------------------
	void RenderTargetEntity::_OnResourceCreated()
	{
		if ( m_isListenResizeview )
		{
			_GetResourcePtr()->ListenResizeview();
		}
	}
	//------------------------------------------------------------------------------------------------------
	void RenderTargetEntity::_OnCreateResource()
	{
		if ( _GetSourcePtr()->GetViewPort() == vector4d() )
		{
			vector2d size = _GetSourcePtr()->GetSize();
			_GetSourcePtr()->SetViewPort( vector4d(0,0,size.m_x,size.m_y) );
		}
	}
	////------------------------------------------------------------------------------------------------------
	//void RenderTargetEntity::SetSize( int x, int y )
	//{
	//	_GetSourcePtr()->SetSize( vector2d(x,y) );
	//}
	////------------------------------------------------------------------------------------------------------
	//void RenderTargetEntity::SetSize( const vector2d& size )
	//{
	//	_GetSourcePtr()->SetSize( size );
	//}
	////------------------------------------------------------------------------------------------------------
	//void RenderTargetEntity::SetViewPort( int bx, int by, int ex, int ey )
	//{
	//	_GetSourcePtr()->SetViewPort( vector4d( bx, by, ex, ey ) );
	//}
	////------------------------------------------------------------------------------------------------------
	//void RenderTargetEntity::SetViewPort( const vector4d& rect )
	//{
	//	_GetSourcePtr()->SetViewPort( rect );
	//}
	////------------------------------------------------------------------------------------------------------
	//void RenderTargetEntity::SetTargetType( RenderTargetProperty::TargetType tt )
	//{
	//	_GetSourcePtr()->SetTargetType( tt );
	//}
	//------------------------------------------------------------------------------------------------------
	void RenderTargetEntity::Resize( const vector2d& size )
	{
		ASSERT( 
			vector4d(0,0,_GetSourcePtr()->GetSize().m_x,_GetSourcePtr()->GetSize().m_y)
			== _GetSourcePtr()->GetViewPort()
			);
		_Recreate();
	}
	//------------------------------------------------------------------------------------------------------
	void RenderTargetEntity::MakeBufferAttachment( RenderTargetProperty::RenderTargetAttachment rta )
	{
		_GetSourcePtr()->MakeBufferAttachment(rta);
	}
	//------------------------------------------------------------------------------------------------------
	TextureEntity* RenderTargetEntity::MakeTextureAttachment( RenderTargetProperty::RenderTargetAttachment rta )
	{
		return _GetSourcePtr()->MakeTextureAttachment(rta);
	}


	////------------------------------------------------------------------------------------------------------
	//void RenderTargetEntity::SetSwapTarget( RenderTargetProperty::SwapTarget st )
	//{
	//	_GetSourcePtr()->SetSwapTarget( st );
	//}
	////------------------------------------------------------------------------------------------------------
	//void RenderTargetEntity::SetRenderTargetType( RenderTargetProperty::TargetType tt )
	//{
	//	m_eTargetType = tt;
	//	_GetSourcePtr()->SetTargetType( tt );
	//}
	////------------------------------------------------------------------------------------------------------
	//void RenderTargetEntity::SetClearColor( float r, float g, float b, float a )
	//{
	//	_GetSourcePtr()->SetClearColor( Color(r,g,b,a) );
	//}
	////------------------------------------------------------------------------------------------------------
	//void RenderTargetEntity::SetClearColor( const Color& clr )
	//{
	//	_GetSourcePtr()->SetClearColor( clr );
	//}



}
