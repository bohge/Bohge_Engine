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

#include "VolumeLightPS.h"
#include "Device.h"
#include "Engine.h"
#include "RenderTarget.h"
#include "ShaderManager.h"
#include "VolumeLightShaders.h"
#include "UtilityShaders.h"


namespace BohgeEngine
{
	//-------------------------------------------------------------------------------------------------------
	VolumeLightPS::VolumeLightPS(bool useSobel)
		:IPostEffect( 1000 ),
		m_HightEffect(useSobel),
		m_pDownSimple(NULL),
		m_pSimple1(NULL),
		m_pSimple2(NULL)
	{
		vector2d size = Engine::Instance().GetDevice()->GetResolutionSize();
		m_pDownSimple = NEW RenderTarget(size / 4, Device::FBO_TCOLOR);

		if( m_HightEffect )
		{
			m_pSimple1 = NEW RenderTarget(size / 4, Device::FBO_TCOLOR);
			m_pSimple2 = NEW RenderTarget(size / 4, Device::FBO_TCOLOR);
		}
	}
	//-------------------------------------------------------------------------------------------------------
	VolumeLightPS::~VolumeLightPS()
	{
		SAFE_DELETE( m_pDownSimple );
		if( m_HightEffect )
		{
			SAFE_DELETE( m_pSimple1 );
			SAFE_DELETE( m_pSimple2 );
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void VolumeLightPS::Process(Engine& engine, const RenderTarget* const org, const RenderTarget* const scene, RenderTarget* const out )
	{
		engine.GetDevice()->DisableDepthTest();
		engine.GetDevice()->DisableAlpha();

		engine.GetDevice()->PushRenderTarget(m_pDownSimple);
#ifdef DirectVolumeLight
		engine.GetDevice()->Clear( Device::COLOR_BUFFER );
		VolumeLight& vl = engine.GetShaderManager()->GetShader<VolumeLight>(ShaderManager::VolumeLight);
		vl.SetParamOrginial(org->GetDepthBuffer());
		vl.SetParamLight(env->GetDepthBuffer());
		engine.GetDevice()->Draw( *m_pRendBuffer, vl, NULL );
#else
		//light scating
		engine.GetDevice()->Clear( Device::COLOR_BUFFER );
		VolumeLightPost& vlp = engine.GetShaderManager()->GetShader<VolumeLightPost>(ShaderManager::VolumeLightPost);
		vlp.SetParamOrginial(org->GetColorBuffer());
		engine.GetDevice()->Draw( *m_pRendBuffer, vlp, NULL );
#endif
		engine.GetDevice()->PopRenderTarget();

		if( m_HightEffect )
		{
			{
				engine.GetDevice()->PushRenderTarget(m_pSimple1); //sobel 边缘检测
				engine.GetDevice()->Clear( Device::COLOR_BUFFER );
				SobelShader& ss = engine.GetShaderManager()->GetShader<SobelShader>(ShaderManager::SobelShader);
				ss.SetParamTextureSize(m_pDownSimple->GetSize());
				engine.GetDevice()->Draw( *m_pRendBuffer, ss, m_pDownSimple->GetColorBuffer() );
				engine.GetDevice()->PopRenderTarget();
			}

			{
				engine.GetDevice()->PushRenderTarget(m_pSimple2); //边缘模糊
				engine.GetDevice()->Clear( Device::COLOR_BUFFER );
				BlurEdgaShader& bes = engine.GetShaderManager()->GetShader<BlurEdgaShader>(ShaderManager::BlurEdgaShader);
				bes.SetParamTextureSize(m_pSimple1->GetSize());
				bes.SetParamScaleTexture( m_pDownSimple->GetColorBuffer() );
				bes.SetParamEdgaTexture( m_pSimple1->GetColorBuffer() );
				engine.GetDevice()->Draw( *m_pRendBuffer, bes, NULL );
				engine.GetDevice()->PopRenderTarget();
			}

			{
				engine.GetDevice()->PushRenderTarget(m_pDownSimple);//卷积模糊
				engine.GetDevice()->Clear( Device::COLOR_BUFFER );
				BlurImageShader& bis = engine.GetShaderManager()->GetShader<BlurImageShader>(ShaderManager::BlurImageShader);
				bis.SetParamTextureSize(m_pSimple2->GetSize());
				engine.GetDevice()->Draw( *m_pRendBuffer, bis, m_pSimple2->GetColorBuffer() );
				engine.GetDevice()->PopRenderTarget();
			}
		}

		engine.GetDevice()->PushRenderTarget(out);//卷积模糊
		VolumeLightCombine& vc = engine.GetShaderManager()->GetShader<VolumeLightCombine>(ShaderManager::VolumeLightCombine);
		vc.SetParamOrginial( scene->GetColorBuffer() );
		vc.SetParamSecen( m_pDownSimple->GetColorBuffer() );
		engine.GetDevice()->Draw( *m_pRendBuffer, vc, NULL );
		engine.GetDevice()->PopRenderTarget();
	}

	void VolumeLightPS::OnSizeChange( const vector2d& size )
	{
		m_pDownSimple->Resize( size / 4 );
		m_pSimple1->Resize( size / 4 );
		m_pSimple2->Resize( size / 4 );
	}

}