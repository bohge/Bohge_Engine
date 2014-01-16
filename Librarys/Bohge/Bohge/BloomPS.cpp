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



/////////////////////////////////////
//			后期渲染特效		   //
/////////////////////////////////////


#include "BloomPS.h"
#include "Engine.h"
#include "Device.h"
#include "RenderTarget.h"
#include "ShaderManage.h"
#include "HDRShaders.h"
#include "UtilityShaders.h"



namespace BohgeEngine
{
	//-------------------------------------------------------------------------------------------------------
	BloomPS::BloomPS()
		:IPostEffect( 500 ),
		m_StrengthOriginal(1.0),
		m_StrengthScene(1.0)
	{

		vector2d size = Engine::Instance().GetDevice()->GetResolutionSize();
		m_pDownSimple_1 = NEW RenderTarget(size / 4 , Device::FBO_TCOLOR);
		m_pDownSimple_2 = NEW RenderTarget(size / 4 , Device::FBO_TCOLOR);
	}
	//-------------------------------------------------------------------------------------------------------
	BloomPS::~BloomPS()
	{
		SAFE_DELETE( m_pDownSimple_1 );
		SAFE_DELETE( m_pDownSimple_2 );
	}
	//-------------------------------------------------------------------------------------------------------
	void BloomPS::Process(Engine& engine, const RenderTarget* const org, const RenderTarget* const scene, RenderTarget* const out )
	{
		float Range = 2; //控制模糊度
		engine.GetDevice()->DisableDepthTest();
		engine.GetDevice()->DisableAlpha();

		//降采样
		//engine.GetDevice()->PushRenderTarget(m_pDownSimple_1);
		//engine.GetDevice()->Clear( Device::COLOR_BUFFER );
		//QuadShader& qs = engine.GetShaderManage()->GetShader<QuadShader>(ShaderManage::QuadShader);
		//engine.GetDevice()->Draw( *m_pRendBuffer, qs, scene->GetColorBuffer() );
		//engine.GetDevice()->PopRenderTarget();

		//流明化
		engine.GetDevice()->PushRenderTarget(m_pDownSimple_1);
		engine.GetDevice()->Clear( Device::COLOR_BUFFER );
		Luminance& le = engine.GetShaderManage()->GetShader<Luminance>(ShaderManage::Luminance);
		engine.GetDevice()->Draw( *m_pRendBuffer, le, scene->GetColorBuffer() );
		engine.GetDevice()->PopRenderTarget();

		//x方向模糊
		engine.GetDevice()->PushRenderTarget(m_pDownSimple_2);
		engine.GetDevice()->Clear( Device::COLOR_BUFFER );
		Gaussian& gauss = engine.GetShaderManage()->GetShader<Gaussian>(ShaderManage::Gaussian);
		gauss.SetParamStep( vector2f( Range / m_pDownSimple_1->GetSize().m_x, 0.0f ) );
		engine.GetDevice()->Draw( *m_pRendBuffer, gauss, m_pDownSimple_1->GetColorBuffer() );
		engine.GetDevice()->PopRenderTarget();

		//y方向模糊
		engine.GetDevice()->PushRenderTarget(m_pDownSimple_1);
		engine.GetDevice()->Clear( Device::COLOR_BUFFER );
		gauss.SetParamStep( vector2f( 0.0f, Range / m_pDownSimple_2->GetSize().m_y ) );
		engine.GetDevice()->Draw( *m_pRendBuffer, gauss, m_pDownSimple_2->GetColorBuffer() );
		engine.GetDevice()->PopRenderTarget();

		engine.GetDevice()->PushRenderTarget( out );//如果画在了输出上面，则需要更换输出，下一个pe就用另外一张rt
		engine.GetDevice()->Clear( Device::COLOR_BUFFER );
		//if( 0 )
		//{
		Combine& ce = engine.GetShaderManage()->GetShader<Combine>(ShaderManage::Combine);
		ce.SetParamOrginial( scene->GetColorBuffer() );
		ce.SetParamSecen( m_pDownSimple_1->GetColorBuffer() );
		ce.SetParamOrginialWeight(m_StrengthOriginal);
		ce.SetParamSecenWeight(m_StrengthScene);//was 0.75
		engine.GetDevice()->Draw( *m_pRendBuffer, ce, NULL );
		//}
		//else
		//{
		//	QuadShader& qs = engine.GetShaderManage()->GetShader<QuadShader>(ShaderManage::QuadShader);
		//	engine.GetDevice()->Draw( *m_pRendBuffer, qs, m_pDownSimple_1->GetColorBuffer() );
		//}
		engine.GetDevice()->PopRenderTarget( );
	}
	//-------------------------------------------------------------------------------------------------------
	void BloomPS::OnSizeChange( const vector2d& size )
	{
		m_pDownSimple_1->Resize( size / 4 );
		m_pDownSimple_2->Resize( size / 4 );
	}

	//-------------------------------------------------------------------------------------------------------
	//-------------------------------------------------------------------------------------------------------
	//-------------------------------------------------------------------------------------------------------
	//-------------------------------------------------------------------------------------------------------
	//-------------------------------------------------------------------------------------------------------
}