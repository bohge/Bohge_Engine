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



//////////////////////////
//		拷贝深度	   //
/////////////////////////
#include "CopyCurrentScreen.h"
#include "Engine.h"
#include "ShaderManage.h"
#include "Vertex.h"
#include "RendBuffer.h"
#include "SceneManage.h"
#include "Texture.h"
#include "UtilityShaders.h"



namespace BohgeEngine
{
	//-------------------------------------------------------------------------------------------------------
	CopyCurrentScreen::CopyCurrentScreen( RenderTarget* rt1, RenderTarget* rt2 )
		:m_Depth( rt1 ),
		m_Color( rt2 )
	{
		ASSERT( rt1 != rt2 );
		VertexTexture Vertex[4];
		Vertex[0] = VertexTexture(vector4f(-1,1,0), vector2f(0,1));
		Vertex[1] = VertexTexture(vector4f(-1,-1,0), vector2f(0,0));
		Vertex[2] = VertexTexture(vector4f(1,1,0), vector2f(1,1));
		Vertex[3] = VertexTexture(vector4f(1,-1,0), vector2f(1,0));
		ushort Indies[4]={0,1,2,3};
		m_pRendBuffer = NEW RendBuffer( Device::RM_TRIANGLE_STRIP);
		m_pVertexBuffer = NEW VertexBuffer( Device::MU_STATIC );
		m_pIndicesBuffer = NEW IndicesBuffer( Device::MU_STATIC );
		m_pRendBuffer->SetVertexBuffer( m_pVertexBuffer );
		m_pRendBuffer->SetIndicesBuffer( m_pIndicesBuffer );
		m_pVertexBuffer->FlushVertexBuffer( 4, &Vertex[0] );
		m_pIndicesBuffer->FlushIndiesBuffer( 4, &Indies[0] );
		//m_Depth->GetColorBuffer()->SetTextureWarp( Device::TW_REPEAT, Device::TW_REPEAT );
		//m_Color->GetColorBuffer()->SetTextureWarp( Device::TW_REPEAT, Device::TW_REPEAT );
	}
	//-------------------------------------------------------------------------------------------------------
	CopyCurrentScreen::~CopyCurrentScreen(void)
	{
		SAFE_DELETE(m_pRendBuffer);
		SAFE_DELETE(m_pVertexBuffer);
		SAFE_DELETE(m_pIndicesBuffer);
	}
	//-------------------------------------------------------------------------------------------------------
	void CopyCurrentScreen::CopyScreenColor( Engine& engine, const RenderTarget* rt )
	{
		RenderTarget* now;
		now = engine.GetDevice()->PopRenderTarget( );
		ASSERT( now != m_Color && now != m_Depth );//需要拷贝的不是是这几个传递进来的公共rt

		engine.GetDevice()->PushRenderTarget( m_Color );
		engine.GetDevice()->Clear( Device::COLOR_BUFFER );
		QuadShader& qs = engine.GetShaderManage()->GetShader<QuadShader>( ShaderManage::QuadShader );
		engine.GetDevice()->Draw( *m_pRendBuffer, qs, rt->GetColorBuffer() );
		engine.GetDevice()->PopRenderTarget( );


		engine.GetDevice()->PushRenderTarget( now );
	}
	//-------------------------------------------------------------------------------------------------------
	void CopyCurrentScreen::CopyScreenDepth( Engine& engine, const RenderTarget* rt )
	{
		RenderTarget* now;
		now = engine.GetDevice()->PopRenderTarget( );
		ASSERT( now != m_Color && now != m_Depth );//需要拷贝的不是是这几个传递进来的公共rt

		Device::AlphaState as = engine.GetDevice()->GetAlphaState();
		engine.GetDevice()->DisableAlpha();//切记，拷贝深度的时候一定要把alpha混合关掉，不然会将深度数据与清屏数据混合
		engine.GetDevice()->PushRenderTarget( m_Depth );
		engine.GetDevice()->Clear( Device::COLOR_BUFFER );
		CopyDepthToRBGAShader& dls = engine.GetShaderManage()->GetShader<CopyDepthToRBGAShader>( ShaderManage::CopyDepthToRBGAShader );
		engine.GetDevice()->Draw( *m_pRendBuffer, dls, rt->GetDepthBuffer() );
		engine.GetDevice()->PopRenderTarget( );
		engine.GetDevice()->EnableAlphaBlend( as );

		engine.GetDevice()->PushRenderTarget( now );
	}
}