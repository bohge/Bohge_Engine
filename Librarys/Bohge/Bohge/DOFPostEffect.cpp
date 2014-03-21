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

#include "DOFPostEffect.h"
#include "Engine.h"
#include "Device.h"
#include "RenderTarget.h"
#include "ShaderManager.h"
#include "ConcisePEShaders.h"
#include "UtilityShaders.h"


namespace BohgeEngine
{
	static float GaussSpread;
	static float GaussRho;
	static std::vector<float> GaussWeight;
	//-------------------------------------------------------------------------------------------------------
	DOFPostEffect::DOFPostEffect(void)
		:IPostEffect( 700 )
	{
		vector2d size = Engine::Instance().GetDevice()->GetResolutionSize();
		m_pDownSimple_1 = NEW RenderTarget(size / 2 , Device::FBO_TCOLOR);
		m_pDownSimple_2 = NEW RenderTarget(size / 2 , Device::FBO_TCOLOR);

		{//初始化数值
			GaussSpread = 4;
			GaussRho = 4;
			GaussWeight.resize(4);
			_MakeupGuassParam();
		}
	}
	//-------------------------------------------------------------------------------------------------------
	DOFPostEffect::~DOFPostEffect(void)
	{
		SAFE_DELETE( m_pDownSimple_1 );
		SAFE_DELETE( m_pDownSimple_2 );
	}
	//-------------------------------------------------------------------------------------------------------
	void DOFPostEffect::_MakeupGuassParam()
	{
		float GaussSum = 0.0f;
		for( int i = 0; i < 4; i++ )
		{
			// normalize to -1..1
			float X = ( float( i ) / 3.0f );
			X = ( X - 0.5f ) * 2.0f;

			// spread is tunable
			X *= GaussSpread;

			float Gauss = Math::Invsqrt( 2.0f * PI * GaussRho * GaussRho );
			Gauss *= Math::Exp( -( X * X ) / ( 2.0f * GaussRho * GaussRho ) );
			GaussWeight[i] = Gauss;

			GaussSum += GaussWeight[i];
		}
		float GaussInvSum = ( 1.0f / GaussSum );
		for( int i = 0; i < 4; i++ )
		{
			GaussWeight[i] *= GaussInvSum;
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void DOFPostEffect::Process( Engine& engine, const RenderTarget* const org, const RenderTarget* const scene, RenderTarget* const out )
	{
		engine.GetDevice()->DisableDepthTest();
		engine.GetDevice()->DisableAlpha();
		{//模糊
			//x
			engine.GetDevice()->PushRenderTarget(m_pDownSimple_1);
			engine.GetDevice()->Clear( Device::COLOR_BUFFER );
			Blur2x2Shader& bs = engine.GetShaderManager()->GetShader<Blur2x2Shader>(ShaderManager::Blur2x2Shader);
			bs.SetParamColorTexture( scene->GetColorBuffer() );
			bs.SetParamStepSize( vector2f( (1.0f / scene->GetSize().m_x) * GaussSpread, 0.0 ) );
			bs.SetParamGaussWeight( GaussWeight );
			engine.GetDevice()->Draw( *m_pRendBuffer, bs, NULL );
			engine.GetDevice()->PopRenderTarget();

			//y
			engine.GetDevice()->PushRenderTarget(m_pDownSimple_2);
			engine.GetDevice()->Clear( Device::COLOR_BUFFER );
			bs.SetParamColorTexture( m_pDownSimple_1->GetColorBuffer() );
			bs.SetParamStepSize( vector2f( 0.0, (1.0f / m_pDownSimple_1->GetSize().m_y) * GaussSpread ) );
			bs.SetParamGaussWeight( GaussWeight );
			engine.GetDevice()->Draw( *m_pRendBuffer, bs, NULL );
			engine.GetDevice()->PopRenderTarget();
		}

		engine.GetDevice()->PushRenderTarget( out );//如果画在了输出上面，则需要更换输出，下一个pe就用另外一张rt
		engine.GetDevice()->Clear( Device::COLOR_BUFFER );
		//if( 1 )
		//{
		DOFCombine& dof = engine.GetShaderManager()->GetShader<DOFCombine>(ShaderManager::DOFCombine);
		dof.SetParamFocus( vector2f( 10, 1.0/ 1500.0 ) );
		dof.SetParamSceneTex( scene->GetColorBuffer() );
		dof.SetParamDepthTex( org->GetDepthBuffer() );
		dof.SetParamBloomTex( m_pDownSimple_2->GetColorBuffer() );
		engine.GetDevice()->Draw( *m_pRendBuffer, dof, NULL );
		//}
		//else
		//{
		//	QuadShader& qs = engine.GetShaderManage()->GetShader<QuadShader>(ShaderManage::QuadShader);
		//	engine.GetDevice()->Draw( *m_pRendBuffer, qs, m_pDownSimple_2->GetColorBuffer() );
		//}
		engine.GetDevice()->PopRenderTarget( );
	}
	//-------------------------------------------------------------------------------------------------------
	void DOFPostEffect::OnSizeChange( const vector2d& size )
	{
		m_pDownSimple_1->Resize( size / 2 );
		m_pDownSimple_2->Resize( size / 2 );
	}

}