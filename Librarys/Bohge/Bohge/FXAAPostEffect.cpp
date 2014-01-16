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
#include "FXAAPostEffect.h"
#include "Device.h"
#include "Engine.h"
#include "RenderTarget.h"
#include "ShaderManage.h"
#include "ConcisePEShaders.h"


namespace BohgeEngine
{
	//-------------------------------------------------------------------------------------------------------
	FXAAPostEffcet::FXAAPostEffcet()
		:IPostEffect( 100000 )//最低优先级，最后渲染
	{
	}
	//-------------------------------------------------------------------------------------------------------
	void FXAAPostEffcet::Process( Engine& engine, const RenderTarget* const org, const RenderTarget* const scene, RenderTarget* const out )
	{
		engine.GetDevice()->DisableAlpha();
		engine.GetDevice()->PushRenderTarget( out );
		engine.GetDevice()->Clear( Device::COLOR_BUFFER | Device::DEPTH_BUFFER | Device::STENCIL_BUFFER ); //清理屏幕
		vector2d size = engine.GetDevice()->GetResolutionSize();
		FXAAShder& fxaa = engine.GetShaderManage()->GetShader<FXAAShder>(ShaderManage::FXAAShder);
		fxaa.SetParamSceneTexture( scene->GetColorBuffer() );
		fxaa.SetParamStepSize( vector2f( 1.0 / size.m_x, 1.0 / size.m_y ) );
		engine.GetDevice()->Draw( *m_pRendBuffer, fxaa, NULL );
		engine.GetDevice()->PopRenderTarget();
	}
	//-------------------------------------------------------------------------------------------------------
	void FXAAPostEffcet::OnSizeChange( const vector2d& size )
	{

	}

}