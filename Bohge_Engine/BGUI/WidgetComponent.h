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
#include "Framework/IComponent.h"
#include "Framework/3DMath.h"
#include "WidgetProperty.h"


#include <EASTL/string.h>


namespace BGUI
{
	class WidgetComponent : public BohgeEngine::IComponent
	{
		RTTI_DRIVER_TYPE_DECLEAR( WidgetComponent, BohgeEngine::IComponent );
	private:
		BohgeEngine::vector4f			m_LocalDesignRect;//设计
		mutable BohgeEngine::vector4f	m_WorldDesignRect;
		mutable BohgeEngine::vector4f	m_WorldActualRect;
		mutable BohgeEngine::vector4f	m_ScreenActualRect;
		BohgeEngine::vector2f			m_Resolution;//分辨率
		mutable bool					m_isRectChange;
		WidgetProperty::WidgetAdaptive	m_WidgetAdaptive;
	public:
		WidgetComponent(void);
		virtual ~WidgetComponent(void);
	private:
		virtual void _DoOnAttachNode( );
	public:
		virtual void OnRootNodeChange( );
		virtual void Release( );
	private:
		void _NarrowCaculate( const BohgeEngine::vector4f& wd, const BohgeEngine::vector4f& wa ) const;
		void _EnlargeCaculate( const BohgeEngine::vector4f& wd, const BohgeEngine::vector4f& wa ) const;
		void _TensileCaculate( const BohgeEngine::vector4f& wd, const BohgeEngine::vector4f& wa ) const;
		void _DoCaculate() const;
	public:
		const Object* AttributeDesignRect() const;
		const Object* AttributeActualRect() const;
	public:
		void SetLocalPosition( const BohgeEngine::vector2f& p );
		void SetLocalDesignRect( const BohgeEngine::vector4f& p );
		void SetWorldActualRect( const BohgeEngine::vector4f& rs );
		const BohgeEngine::vector4f& GetWorldDesignRect() const;
		const BohgeEngine::vector4f& GetWorldActualRect() const;
		const BohgeEngine::vector4f& GetScreenActualRect() const;
	public:
		BOHGE_FORCEINLINE const BohgeEngine::vector4f& GetLocalDesignRect()
		{
			return m_LocalDesignRect;
		}
		BOHGE_FORCEINLINE void SetResolution( const BohgeEngine::vector2f& r )
		{
			m_isRectChange = true;
			m_Resolution = r;
		}
		BOHGE_FORCEINLINE void SetWidgetAdaptive( WidgetProperty::WidgetAdaptive wa )
		{
			m_WidgetAdaptive = wa;
		}
		BOHGE_FORCEINLINE WidgetProperty::WidgetAdaptive GetAdaptive()
		{
			return m_WidgetAdaptive;
		}
		BOHGE_FORCEINLINE const BohgeEngine::vector2f& GetResolution()
		{
			return m_Resolution;
		}
	};
}