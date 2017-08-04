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
#include "Framework/NodeMessage.h"
#include "WidgetComponent.h"
#include "Framework/Node.h"


using namespace BohgeEngine;

namespace BGUI
{
	RTTI_TYPE_IMPLEMENT( WidgetComponent );
	//------------------------------------------------------------------------------
	WidgetComponent::WidgetComponent( void )
		:m_isRectChange(true),
		m_WidgetAdaptive( WidgetProperty::WA_NARROW )
	{

	}
	//------------------------------------------------------------------------------
	WidgetComponent::~WidgetComponent( void )
	{

	}
	//------------------------------------------------------------------------------
	void WidgetComponent::_DoOnAttachNode()
	{
		//连接数据源提供者
		_HostNode()->RegisterAttributeProvider( NodeProperty::GA_USER_0, MEMBER_FUNC_PTR( &WidgetComponent::AttributeDesignRect ) );
		_HostNode()->RegisterAttributeProvider( NodeProperty::GA_USER_1, MEMBER_FUNC_PTR( &WidgetComponent::AttributeActualRect ) );
	}
	//------------------------------------------------------------------------------
	void WidgetComponent::OnRootNodeChange()
	{
		m_isRectChange = true;
	}
	//------------------------------------------------------------------------------
	void WidgetComponent::Release()
	{

	}
	//------------------------------------------------------------------------------
	void WidgetComponent::SetLocalPosition( const BohgeEngine::vector2f& p )
	{
		m_isRectChange = true;
		m_LocalDesignRect.m_x = p.m_x;
		m_LocalDesignRect.m_y = p.m_y;
	}
	//------------------------------------------------------------------------------
	void WidgetComponent::SetLocalDesignRect( const BohgeEngine::vector4f& p )
	{
		m_isRectChange = true;
		m_LocalDesignRect = p;
	}
	//------------------------------------------------------------------------------
	void WidgetComponent::SetWorldActualRect( const BohgeEngine::vector4f& rs )
	{
		m_WorldActualRect = rs;
	}
	//------------------------------------------------------------------------------
	const Object* WidgetComponent::AttributeDesignRect() const
	{
		return &GetWorldDesignRect();
	}
	//------------------------------------------------------------------------------
	const Object* WidgetComponent::AttributeActualRect() const
	{
		return &GetWorldActualRect();
	}
	//------------------------------------------------------------------------------
	void WidgetComponent::_NarrowCaculate( const BohgeEngine::vector4f& wd, const BohgeEngine::vector4f& wa ) const
	{
		m_WorldDesignRect = vector4f(
			m_LocalDesignRect.m_x + wd.m_x,
			m_LocalDesignRect.m_y + wd.m_y,
			m_LocalDesignRect.m_z,
			m_LocalDesignRect.m_w  );
		vector2f scale = vector2f( wa.m_z, wa.m_w ) / vector2f( wd.m_z, wd.m_w );
		float mins = Math::GetMin( scale );
		if ( scale.m_x > scale.m_y )
		{
			m_WorldActualRect.m_x = wa.m_x + m_LocalDesignRect.m_x * scale.m_x + m_LocalDesignRect.m_z * ( scale.m_x - scale.m_y ) * 0.5f;
			m_WorldActualRect.m_y = wa.m_y + m_LocalDesignRect.m_y * scale.m_y;
		}
		else
		{
			m_WorldActualRect.m_x = wa.m_x + m_LocalDesignRect.m_x * scale.m_x;
			m_WorldActualRect.m_y = wa.m_y + m_LocalDesignRect.m_y * scale.m_y + m_LocalDesignRect.m_w * ( scale.m_y - scale.m_x ) * 0.5f;
		}
		m_WorldActualRect.m_z = m_LocalDesignRect.m_z * mins;
		m_WorldActualRect.m_w = m_LocalDesignRect.m_w * mins;
	}
	//------------------------------------------------------------------------------
	void WidgetComponent::_EnlargeCaculate( const BohgeEngine::vector4f& wd, const BohgeEngine::vector4f& wa ) const
	{
		m_WorldDesignRect = vector4f(
			m_LocalDesignRect.m_x + wd.m_x,
			m_LocalDesignRect.m_y + wd.m_y,
			m_LocalDesignRect.m_z,
			m_LocalDesignRect.m_w  );
		vector2f scale = vector2f( wa.m_z, wa.m_w ) / vector2f( wd.m_z, wd.m_w );
		float maxs = Math::GetMax( scale );
		if ( scale.m_x < scale.m_y )
		{
			m_WorldActualRect.m_x = wa.m_x + m_LocalDesignRect.m_x * scale.m_x + m_LocalDesignRect.m_z * ( scale.m_x - scale.m_y ) * 0.5f;
			m_WorldActualRect.m_y = wa.m_y + m_LocalDesignRect.m_y * scale.m_y;
		}
		else
		{
			m_WorldActualRect.m_x = wa.m_x + m_LocalDesignRect.m_x * scale.m_x;
			m_WorldActualRect.m_y = wa.m_y + m_LocalDesignRect.m_y * scale.m_y + m_LocalDesignRect.m_w * ( scale.m_y - scale.m_x ) * 0.5f;
		}
		m_WorldActualRect.m_z = m_LocalDesignRect.m_z * maxs;
		m_WorldActualRect.m_w = m_LocalDesignRect.m_w * maxs;
	}
	//------------------------------------------------------------------------------
	void WidgetComponent::_TensileCaculate( const BohgeEngine::vector4f& wd, const BohgeEngine::vector4f& wa ) const
	{
		m_WorldDesignRect = vector4f(
			m_LocalDesignRect.m_x + wd.m_x,
			m_LocalDesignRect.m_y + wd.m_y,
			m_LocalDesignRect.m_z,
			m_LocalDesignRect.m_w  );
		vector2f scale = vector2f( wa.m_z, wa.m_w ) / vector2f( wd.m_z, wd.m_w );
		m_WorldActualRect = m_WorldDesignRect * vector4f( scale, scale );
	}
	//------------------------------------------------------------------------------
	void WidgetComponent::_DoCaculate() const
	{
		if ( NULL == _HostNode()->RootAttribute( NodeProperty::GA_USER_0 ) )
		{
			m_WorldDesignRect = m_LocalDesignRect;
		}
		else
		{
			{//计算真实的世界rect
				const void* fd = _HostNode()->RootAttribute( NodeProperty::GA_USER_0 );
				const void* fa = _HostNode()->RootAttribute( NodeProperty::GA_USER_1 );
				switch( m_WidgetAdaptive )
				{
				case WidgetProperty::WA_NARROW:_NarrowCaculate( *(const vector4f*)fd, *(const vector4f*)fa );break;
				case WidgetProperty::WA_ENLARGE:_EnlargeCaculate( *(const vector4f*)fd, *(const vector4f*)fa );break;
				case WidgetProperty::WA_TENSILE:_TensileCaculate( *(const vector4f*)fd, *(const vector4f*)fa );break;
				default:ASSERT(false);
				}
			}
		}
		{//计算屏幕分辨率
			m_ScreenActualRect = m_WorldActualRect / vector4f( m_Resolution, m_Resolution );
		}
	}
	//------------------------------------------------------------------------------
	const BohgeEngine::vector4f& WidgetComponent::GetWorldDesignRect() const
	{
		if ( m_isRectChange )
		{
			m_isRectChange = false;
			_DoCaculate();
		}
		return m_WorldDesignRect;
	}
	//------------------------------------------------------------------------------
	const BohgeEngine::vector4f& WidgetComponent::GetWorldActualRect() const
	{
		if ( m_isRectChange )
		{
			m_isRectChange = false;
			_DoCaculate();
		}
		return m_WorldActualRect;
	}
	//------------------------------------------------------------------------------
	const BohgeEngine::vector4f& WidgetComponent::GetScreenActualRect() const
	{
		if ( m_isRectChange )
		{
			m_isRectChange = false;
			_DoCaculate();
		}
		return m_ScreenActualRect;
	}

}