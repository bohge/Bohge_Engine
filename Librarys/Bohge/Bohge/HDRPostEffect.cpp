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

#include "HDRPostEffect.h"
#include "Device.h"
#include "Engine.h"
#include "RenderTarget.h"
#include "ShaderManager.h"
#include "HDRShaders.h"
#include "UtilityShaders.h"

#include "Vertex.h"
#include "RendBuffer.h"

namespace BohgeEngine
{
	static float GaussSpread;
	static float GaussRho;
	static float MiddleGray;
	static float White;
	static float BrightPassWhite;
	static float BrightPassThreshold;
	static float BrightPassOffset;
	static float Lambda;
	static float ExposureLimits;
	static std::vector<float> GaussWeight;


	//-------------------------------------------------------------------------------------------------------
	HDRPostEffect::HDRPostEffect(void)
		:IPostEffect( 500 )//不能和bloom共存
	{
		vector2d size = Engine::Instance().GetDevice()->GetResolutionSize();
		m_QuarterRT = NEW RenderTarget(size / 4 , Device::FBO_TCOLOR);
		m_BlurredRT = NEW RenderTarget(size / 4 , Device::FBO_TCOLOR);
		m_ScratchRT = NEW RenderTarget(size / 4 , Device::FBO_TCOLOR);
		m_BrightPassRT = NEW RenderTarget(size / 4 , Device::FBO_TCOLOR);

		m_64x64RT = NEW RenderTarget( vector2d(64, 64), Device::FBO_TCOLOR);
		m_16x16RT = NEW RenderTarget( vector2d(16, 16), Device::FBO_TCOLOR);
		m_4x4RT = NEW RenderTarget( vector2d(4, 4), Device::FBO_TCOLOR);
		m_1x1RT = NEW RenderTarget( vector2d(1, 1), Device::FBO_TCOLOR);
		m_PreviousAverageLumRT = NEW RenderTarget( vector2d(1, 1), Device::FBO_TCOLOR);
		m_LastAverageLumRT = NEW RenderTarget( vector2d(1, 1), Device::FBO_TCOLOR);

		{//初始化数值
			GaussSpread = 3;
			GaussRho = 2;//1.2;
			ExposureLimits = 0.35;//曝光色彩的上限
			MiddleGray = 0.125;//曝光色彩的下限
			White = 0.101;//白平衡
			BrightPassWhite = 0.2f;//6.0f;//0.9f;
			BrightPassThreshold = 2.0f;//2.41;//0.6f;
			BrightPassOffset = 0.5;//0.8f;
			Lambda = 0.05f;
			GaussWeight.resize(4);
			{//初始化模糊数组
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
		}
		Engine::Instance().GetActionManager()->OnKeyDownEvent( MEMBER_FUNC_PTR( &HDRPostEffect::OnKeyDown ) );


		//ushort Indies[4]={0,1,2,3};
		//m_pDebugRendBuffer = NEW RendBuffer( Device::RM_TRIANGLE_STRIP );
		//m_pDebugIndicesBuffer = NEW IndicesBuffer( Device::MU_STATIC );
		//m_pDebugRendBuffer->SetIndicesBuffer( m_pIndicesBuffer );
		//m_pDebugIndicesBuffer->FlushIndiesBuffer( 4, &Indies[0] );

		//VertexTexture Vertex[4];
		//Vertex[0] = VertexTexture(vector4f(-1,1,0), vector2f(0,1));
		//Vertex[1] = VertexTexture(vector4f(-1,0.75,0), vector2f(0,0));
		//Vertex[2] = VertexTexture(vector4f(-0.7,1,0), vector2f(1,1));
		//Vertex[3] = VertexTexture(vector4f(-0.7,0.75,0), vector2f(1,0));
		//m_pDebugVertexBuffer1 = NEW VertexBuffer( Device::MU_STATIC );
		//m_pDebugVertexBuffer1->FlushVertexBuffer( 4 , &Vertex[0] );


		//Vertex[0] = VertexTexture(vector4f(-1,0.7,0), vector2f(0,1));
		//Vertex[1] = VertexTexture(vector4f(-1,0.45,0), vector2f(0,0));
		//Vertex[2] = VertexTexture(vector4f(-0.7,0.7,0), vector2f(1,1));
		//Vertex[3] = VertexTexture(vector4f(-0.7,0.45,0), vector2f(1,0));
		//m_pDebugVertexBuffer2 = NEW VertexBuffer( Device::MU_STATIC );
		//m_pDebugVertexBuffer2->FlushVertexBuffer( 4 , &Vertex[0] );

		//Vertex[0] = VertexTexture(vector4f(-1,0.4,0), vector2f(0,1));
		//Vertex[1] = VertexTexture(vector4f(-1,0.15,0), vector2f(0,0));
		//Vertex[2] = VertexTexture(vector4f(-0.7,0.4,0), vector2f(1,1));
		//Vertex[3] = VertexTexture(vector4f(-0.7,0.15,0), vector2f(1,0));
		//m_pDebugVertexBuffer3 = NEW VertexBuffer( Device::MU_STATIC );
		//m_pDebugVertexBuffer3->FlushVertexBuffer( 4 , &Vertex[0] );

		//Vertex[0] = VertexTexture(vector4f(-1,0.1,0), vector2f(0,1));
		//Vertex[1] = VertexTexture(vector4f(-1,-0.15,0), vector2f(0,0));
		//Vertex[2] = VertexTexture(vector4f(-0.7,0.1,0), vector2f(1,1));
		//Vertex[3] = VertexTexture(vector4f(-0.7,-0.15,0), vector2f(1,0));
		//m_pDebugVertexBuffer4 = NEW VertexBuffer( Device::MU_STATIC );
		//m_pDebugVertexBuffer4->FlushVertexBuffer( 4 , &Vertex[0] );
	}
	//-------------------------------------------------------------------------------------------------------
	HDRPostEffect::~HDRPostEffect(void)
	{
		SAFE_DELETE( m_QuarterRT );
		SAFE_DELETE( m_BlurredRT );
		SAFE_DELETE( m_ScratchRT );
		SAFE_DELETE( m_BrightPassRT );
		SAFE_DELETE( m_64x64RT );
		SAFE_DELETE( m_16x16RT );
		SAFE_DELETE( m_4x4RT );
		SAFE_DELETE( m_1x1RT );
		SAFE_DELETE( m_PreviousAverageLumRT );
		SAFE_DELETE( m_LastAverageLumRT );
	}
	//-------------------------------------------------------------------------------------------------------
	bool HDRPostEffect::OnKeyDown( ActionManager::Key_Down k )
	{
		switch( k )
		{
		case ActionManager::KD_KEY1: ExposureLimits += 0.001; break;
		case ActionManager::KD_KEY2: ExposureLimits -= 0.001; break;
		case ActionManager::KD_KEY3: MiddleGray += 0.001; break;
		case ActionManager::KD_KEY4: MiddleGray -= 0.001; break;
		case ActionManager::KD_KEY5: White += 0.001; break;
		case ActionManager::KD_KEY6: White -= 0.001; break;
		case ActionManager::KD_KEY7: BrightPassWhite += 0.001; break;
		case ActionManager::KD_KEY8: BrightPassWhite -= 0.001; break;
		case ActionManager::KD_KEY9: BrightPassThreshold += 0.005; break;
		case ActionManager::KD_KEY0: BrightPassThreshold -= 0.005; break;
		case ActionManager::KD_KEYQ: BrightPassOffset += 0.005; break;
		case ActionManager::KD_KEYE: BrightPassOffset -= 0.005; break;
		}
		DEBUGLOG( "EL=%f, MG=%f, W=%f, BPW=%f BPT=%f, BPO=%f\n", ExposureLimits, MiddleGray, White, BrightPassWhite, BrightPassThreshold, BrightPassOffset );
		return true;
	}
	//-------------------------------------------------------------------------------------------------------
	void HDRPostEffect::_MakeBlur( Engine& engine, const RenderTarget* const InputTargetRT, RenderTarget* BlurredTargetRT )
	{
		ASSERT( m_ScratchRT->GetSize() == BlurredTargetRT->GetSize() );
			//x
		engine.GetDevice()->PushRenderTarget(m_ScratchRT);
		engine.GetDevice()->Clear( Device::COLOR_BUFFER );
		Blur2x2Shader& hdrB = engine.GetShaderManager()->GetShader<Blur2x2Shader>(ShaderManager::Blur2x2Shader);
		hdrB.SetParamColorTexture( InputTargetRT->GetColorBuffer() );
		hdrB.SetParamStepSize( vector2f( (1.0f / m_ScratchRT->GetSize().m_x) * GaussSpread, 0.0 ) );
		hdrB.SetParamGaussWeight( GaussWeight );
		engine.GetDevice()->Draw( *m_pRendBuffer, hdrB, NULL );
		engine.GetDevice()->PopRenderTarget();

		//y
		engine.GetDevice()->PushRenderTarget(BlurredTargetRT);
		engine.GetDevice()->Clear( Device::COLOR_BUFFER );
		hdrB.SetParamColorTexture( m_ScratchRT->GetColorBuffer() );
		hdrB.SetParamStepSize( vector2f( 0.0, (1.0f / m_ScratchRT->GetSize().m_y) * GaussSpread ) );
		hdrB.SetParamGaussWeight( GaussWeight );
		engine.GetDevice()->Draw( *m_pRendBuffer, hdrB, NULL );
		engine.GetDevice()->PopRenderTarget();
	}
	//-------------------------------------------------------------------------------------------------------
	void HDRPostEffect::Process( Engine& engine, const RenderTarget* const org, const RenderTarget* const scene, RenderTarget* const out )
	{
		engine.GetDevice()->DisableDepthTest();
		engine.GetDevice()->DisableAlpha();

		//下列实际可以去掉
		//{//画到小的图上
		//	engine.GetDevice()->PushRenderTarget(m_QuarterRT);
		//	engine.GetDevice()->Clear( Device::COLOR_BUFFER );
		//	QuadShader& qs = engine.GetShaderManage()->GetShader<QuadShader>(ShaderManage::QuadShader);
		//	engine.GetDevice()->Draw( *m_pRendBuffer, qs, scene->GetColorBuffer() );
		//	engine.GetDevice()->PopRenderTarget();
		//}
		//{//模糊
		//	_MakeBlur( engine, m_QuarterRT, m_BlurredRT );
		//}

		{//模糊
			_MakeBlur( engine, scene, m_BlurredRT );
		}

		{//缩小等一系列操作

			//生成流明
			engine.GetDevice()->PushRenderTarget(m_64x64RT);
			engine.GetDevice()->Clear( Device::COLOR_BUFFER );
			HDRGenerateLuminance& gl = engine.GetShaderManager()->GetShader<HDRGenerateLuminance>(ShaderManager::HDRGenerateLuminance);
			gl.SetParamColorTexture( m_BlurredRT->GetColorBuffer() );
			gl.SetParamStepSize( vector2f( 1.0 / m_BlurredRT->GetSize().m_x,  1.0 / m_BlurredRT->GetSize().m_y ) );
			engine.GetDevice()->Draw( *m_pRendBuffer, gl, NULL );
			engine.GetDevice()->PopRenderTarget();

			//缩小
			engine.GetDevice()->PushRenderTarget(m_16x16RT);
			engine.GetDevice()->Clear( Device::COLOR_BUFFER );
			HDRAverageLuminance& al = engine.GetShaderManager()->GetShader<HDRAverageLuminance>(ShaderManager::HDRAverageLuminance);
			al.SetParamColorTexture( m_64x64RT->GetColorBuffer() );
			al.SetParamStepSize( vector2f( 1.0 / m_64x64RT->GetSize().m_x,  1.0 / m_64x64RT->GetSize().m_y ) );
			engine.GetDevice()->Draw( *m_pRendBuffer, al, NULL );
			engine.GetDevice()->PopRenderTarget();

			engine.GetDevice()->PushRenderTarget(m_4x4RT);
			engine.GetDevice()->Clear( Device::COLOR_BUFFER );
			al.SetParamColorTexture( m_16x16RT->GetColorBuffer() );
			al.SetParamStepSize( vector2f( 1.0 / m_16x16RT->GetSize().m_x,  1.0 / m_16x16RT->GetSize().m_y ) );
			engine.GetDevice()->Draw( *m_pRendBuffer, al, NULL );
			engine.GetDevice()->PopRenderTarget();

			engine.GetDevice()->PushRenderTarget(m_1x1RT);
			engine.GetDevice()->Clear( Device::COLOR_BUFFER );
			al.SetParamColorTexture( m_4x4RT->GetColorBuffer() );
			al.SetParamStepSize( vector2f( 1.0 / m_4x4RT->GetSize().m_x,  1.0 / m_4x4RT->GetSize().m_y ) );
			engine.GetDevice()->Draw( *m_pRendBuffer, al, NULL );
			engine.GetDevice()->PopRenderTarget();

			//生成上输入的流明
			engine.GetDevice()->PushRenderTarget(m_LastAverageLumRT);
			engine.GetDevice()->Clear( Device::COLOR_BUFFER );
			HDRAdaptLuminance& adapt = engine.GetShaderManager()->GetShader<HDRAdaptLuminance>(ShaderManager::HDRAdaptLuminance);
			adapt.SetParamCurrentLuminance( m_1x1RT->GetColorBuffer() );
			adapt.SetParamPreviousLuminance( m_PreviousAverageLumRT->GetColorBuffer() );
			adapt.SetParamLambdaElapsedTime( Lambda * engine.GetTimeSpan() );		
			adapt.SetParamExposureLimits( ExposureLimits );
			engine.GetDevice()->Draw( *m_pRendBuffer, adapt, NULL );
			engine.GetDevice()->PopRenderTarget();
		}
		{//拷贝
			engine.GetDevice()->PushRenderTarget(m_PreviousAverageLumRT);
			engine.GetDevice()->Clear( Device::COLOR_BUFFER );
			QuadShader& qs = engine.GetShaderManager()->GetShader<QuadShader>(ShaderManager::QuadShader);
			engine.GetDevice()->Draw( *m_pRendBuffer, qs, m_LastAverageLumRT->GetColorBuffer() );
			engine.GetDevice()->PopRenderTarget();
		}

		{//Bright pass
			engine.GetDevice()->PushRenderTarget(m_BrightPassRT);
			engine.GetDevice()->Clear( Device::COLOR_BUFFER );
			HDRBrightPass& bp = engine.GetShaderManager()->GetShader<HDRBrightPass>(ShaderManager::HDRBrightPass);
			bp.SetParamCurrentLuminance( m_LastAverageLumRT->GetColorBuffer() );
			bp.SetParamScratchTexture( scene->GetColorBuffer() );
			bp.SetParamMiddleGray( MiddleGray );
			bp.SetParamBrightPassWhite( BrightPassWhite );
			bp.SetParamBrightPassThreshold( BrightPassThreshold );
			bp.SetParamBrightPassOffset( BrightPassOffset );
			engine.GetDevice()->Draw( *m_pRendBuffer, bp, NULL );
			engine.GetDevice()->PopRenderTarget();
		}
		_MakeBlur( engine, m_BrightPassRT, m_QuarterRT );
		_MakeBlur( engine, m_QuarterRT, m_BlurredRT );


		engine.GetDevice()->PushRenderTarget( out );//如果画在了输出上面，则需要更换输出，下一个pe就用另外一张rt
		engine.GetDevice()->Clear( Device::COLOR_BUFFER );
		//if( 1 )
		//{
		HDRCombine& ce = engine.GetShaderManager()->GetShader<HDRCombine>(ShaderManager::HDRCombine);
		ce.SetParamCurrentLuminance( m_LastAverageLumRT->GetColorBuffer() );
		ce.SetParamSharpTexture( scene->GetColorBuffer() );
		ce.SetParamBloomTexture( m_BlurredRT->GetColorBuffer() );
		ce.SetParamMiddleGray( MiddleGray );
		ce.SetParamWhite( White );
		engine.GetDevice()->Draw( *m_pRendBuffer, ce, NULL );
		//}
		//else
		//{
		//	QuadShader& qs = engine.GetShaderManage()->GetShader<QuadShader>(ShaderManage::QuadShader);
		//	engine.GetDevice()->Draw( *m_pRendBuffer, qs, m_BlurredRT->GetColorBuffer() );
		//}

		//QuadShader& qs = engine.GetShaderManage()->GetShader<QuadShader>(ShaderManage::QuadShader);
		//m_pDebugRendBuffer->SetVertexBuffer( m_pDebugVertexBuffer1 );
		//engine.GetDevice()->Draw( *m_pDebugRendBuffer, qs, m_4x4RT->GetColorBuffer() );
		//m_pDebugRendBuffer->SetVertexBuffer( m_pDebugVertexBuffer2 );
		//engine.GetDevice()->Draw( *m_pDebugRendBuffer, qs, m_1x1RT->GetColorBuffer() );
		//m_pDebugRendBuffer->SetVertexBuffer( m_pDebugVertexBuffer3 );
		//engine.GetDevice()->Draw( *m_pDebugRendBuffer, qs, m_LastAverageLumRT->GetColorBuffer() );
		//m_pDebugRendBuffer->SetVertexBuffer( m_pDebugVertexBuffer4 );
		//engine.GetDevice()->Draw( *m_pDebugRendBuffer, qs, m_PreviousAverageLumRT->GetColorBuffer() );
		engine.GetDevice()->PopRenderTarget( );
	}
	//-------------------------------------------------------------------------------------------------------
	void HDRPostEffect::OnSizeChange( const vector2d& size )
	{
		m_QuarterRT->Resize( size / 4 );
		m_BlurredRT->Resize( size / 4 );
		m_ScratchRT->Resize( size / 4 );
		m_BrightPassRT->Resize( size / 4 );
	}
}