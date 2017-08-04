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
#include "Framework/Delegate.hpp"
#include "WidgetComponent.h"
#include "Framework/Node.h"
#include "WidgetProperty.h"
#include "DataLoader_1.h"



#include <EASTL/map.h>
#include <EASTL/string.h>
namespace BohgeEngine
{
	class TouchInfo;
	class IAsyncSource;
	class VertexStream;
	class IJsonDataSet;
	class RenderComponent;
}
namespace BGUI
{
	class ImageSet;
	class IOptional;
	class UIWidget : public BohgeEngine::Node
	{
		RTTI_DRIVER_TYPE_DECLEAR( UIWidget, BohgeEngine::Node );
	private:
		BohgeEngine::Delegate1<void,uint>	m_ChangeEvent;
		WidgetComponent*					m_pUITransform;
		uint								m_MaterialHash;
	protected:
		WidgetProperty::WidgetStatus	m_Status;
	public:
		UIWidget(void);
		virtual ~UIWidget(void);
	private:
		virtual void _DoFlushMaterials( MaterialMap& mm ){};
		virtual void _DoFlushBuffer( BohgeEngine::VertexStream* buff ){};
		virtual void _DoFlushParameter( BohgeEngine::vector4arrayf& arr ){};
		virtual void _DoFlushLayout( BohgeEngine::VertexStream* buff ){};
		virtual uint _DoFlushGeometryIndex( uint index );
		virtual void _DoFlushTextureEntiy( BohgeEngine::RenderComponent* com ){};
		virtual void _DoFlushBind( BohgeEngine::Aabrectf& bind ){};
		virtual bool _DoOnActionEvent( const BohgeEngine::TouchInfo& touch );
		virtual bool _DoIsDelayFlush();
		virtual void _DoOnCreateResource( BohgeEngine::IAsyncSource* host ){};
	protected:
		virtual void _ReflushBuffer( uint hash );
		virtual void _ReflushTexture();
	public:
		virtual const Widgets_1_Array* Load_1( const BohgeEngine::IJsonDataSet& data );
	protected:
		void _SetMaterialHash( uint hash );
		void _NoticeStatusChange( uint id );
	public:
		void SetResolution( const BohgeEngine::vector2f& res );//屏幕分辨率
		void SetLocalPosition( const BohgeEngine::vector2d& pos );
		void SetDesignRectangle( const BohgeEngine::vector4d& size );//设置设计时的rect
		void SetActualRectangle( const BohgeEngine::vector4d& size );//设置实际的的rect
		void FlushMaterials( MaterialMap& mm );
		bool FlushLayout( uint id, BohgeEngine::VertexStream* buff );
		void FlushBuffer( uint id, BohgeEngine::VertexStream* buff );
		void FlushParameter( uint id, BohgeEngine::vector4arrayf& arr );
		bool FlushTextureEntiy( uint id, BohgeEngine::RenderComponent* com );
		uint FlushGeometryIndex( uint id, uint index );
		void FlushBind( BohgeEngine::Aabrectf& bind );
		uint DepthOffset( uint id, uint depth );
		bool isDelayFlush( uint id );
		void ReleaseChild();
		bool OnActionEvent( const BohgeEngine::TouchInfo& touch );
		void CreateResource( BohgeEngine::IAsyncSource* host );
		UIWidget* FindWidget( const eastl::string& name );
		void Enable();
		void Disable();
		void Hide( );
		void EnableWithChild();
		void DisableWithChild();
		void HideWithChild( );
	private:
		 BOHGE_FORCEINLINE bool _isMaterial( uint hash ) const
		 {
			 return m_MaterialHash == hash;
		 }
	protected:
		BOHGE_FORCEINLINE uint _GetMaterialHash( ) const
		{
			return m_MaterialHash;
		}
	public:
		BOHGE_FORCEINLINE void SetStatusChangeListener( BohgeEngine::Delegate1<void,uint>::FunctionType* const ptr )
		{
			m_ChangeEvent.Connect( ptr );
		}
		BOHGE_FORCEINLINE const BohgeEngine::vector4f& GetScreenActualRect() const
		{
			return m_pUITransform->GetScreenActualRect();
		}
		BOHGE_FORCEINLINE const BohgeEngine::vector4f& GetLocalDesignRect()
		{
			return m_pUITransform->GetLocalDesignRect();
		}
		BOHGE_FORCEINLINE void SetAdaptive( WidgetProperty::WidgetAdaptive wa )
		{
			m_pUITransform->SetWidgetAdaptive( wa );
		}
		BOHGE_FORCEINLINE WidgetProperty::WidgetAdaptive GetAdaptive()
		{
			return m_pUITransform->GetAdaptive();
		}
		BOHGE_FORCEINLINE const BohgeEngine::vector2f& GetResolution()
		{
			return m_pUITransform->GetResolution();
		}
		//BOHGE_FORCEINLINE const BohgeEngine::vector2f& GetWorldScale() const
		//{
		//	return m_pUITransform->GetWorldScale();
		//}
	};
}