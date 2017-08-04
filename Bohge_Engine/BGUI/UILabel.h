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
#include "Framework/IAsyncSource.h"
#include "Framework/SmartPtr.hpp"
#include "UIWidget.h"



namespace BGUI
{
	class UIGeometry;
	class UILabel : public UIWidget
	{
		RTTI_DRIVER_TYPE_DECLEAR( UILabel, UIWidget );
	private:
		BohgeEngine::TextureEntity*		m_pSwitch;
		BohgeEngine::TextureEntity*		m_pTexture;
		UIGeometry*						m_pGeometry;
	public:
		UILabel(void);
		~UILabel(void);
	public:
		void OnReplaceTextureLoaded( const BohgeEngine::SmartPtr<BohgeEngine::IAsyncSource>& res );
	private:
		virtual void _DoFlushMaterials( MaterialMap& mm );
		virtual void _DoFlushBuffer( BohgeEngine::VertexStream* buff );
		virtual void _DoFlushParameter( BohgeEngine::vector4arrayf& arr );
		virtual void _DoFlushLayout( BohgeEngine::VertexStream* buff );
		virtual uint _DoFlushGeometryIndex( uint index );
		virtual void _DoFlushTextureEntiy( BohgeEngine::RenderComponent* com );
		virtual void _DoOnCreateResource( BohgeEngine::IAsyncSource* host );
	public:
		virtual const Widgets_1_Array* Load_1( const BohgeEngine::IJsonDataSet& data );
	public:
		void ReplaceTexture( const eastl::string& name );//这个接口其效果必须保证独立纹理
	};
}