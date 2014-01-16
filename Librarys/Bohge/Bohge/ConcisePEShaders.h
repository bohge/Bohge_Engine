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
//		dofºÍssvoµÄshader		   //
////////////////////////////////////
#pragma once
#include "3DMath.h"
#include "BaseShader.h"


namespace BohgeEngine
{
	class Texture;
	/////////////////////////
	//		DOFCombine		//
	/////////////////////////
	//---------------------------------------------------------------------
	class DOFCombine : public Shader
	{
	private:
		handle				m_hOgiTexture;
		handle				m_hDepthTexture;
		handle				m_hBloomTexture;
		handle				m_hNearQ;
		handle				m_hFocus;
		const Texture*		m_pOgiTexture;
		const Texture*		m_pBloomTexture;
		const Texture*		m_pDepthTexture;
		vector2f			m_Focus;
	protected:
		virtual void _SetParameters();
	public:
		DOFCombine();
		virtual ~DOFCombine();
		virtual bool Initialization();
	public:
		BOHGE_FORCEINLINE void SetParamSceneTex(const Texture* input)
		{
			m_pOgiTexture = input;
		}
		BOHGE_FORCEINLINE void SetParamBloomTex(const Texture* input)
		{
			m_pBloomTexture = input;
		}
		BOHGE_FORCEINLINE void SetParamDepthTex(const Texture* input)
		{
			m_pDepthTexture = input;
		}
		BOHGE_FORCEINLINE void SetParamFocus( const vector2f& input)
		{
			m_Focus = input;
		}
	};



	/////////////////////////
	//		SSAOShader		//
	/////////////////////////
	//---------------------------------------------------------------------
	class SSAOShader : public Shader
	{
	private:
		handle				m_hNormalTexture;
		handle				m_hDepthTexture;
		handle				m_hJitterTexture;
		handle				m_hProjInv;
		handle				m_hStepSize;
		handle				m_hAOEdgeFinder;
		handle				m_hAOIntensity;
		handle				m_hJitterAmount;
		const Texture*		m_pNormalTexture;
		const Texture*		m_pJitterTexture;
		const Texture*		m_pDepthTexture;
		vector2f			m_Step;
		float				m_AOEdgeFinder;
		float				m_AOIntensity;
		float				m_JitterAmount;
	protected:
		virtual void _SetParameters();
	public:
		SSAOShader();
		virtual ~SSAOShader();
		virtual bool Initialization();
	public:
		BOHGE_FORCEINLINE void SetParamNormalTex(const Texture* input)
		{
			m_pNormalTexture = input;
		}
		BOHGE_FORCEINLINE void SetParamJitterTex(const Texture* input)
		{
			m_pJitterTexture = input;
		}
		BOHGE_FORCEINLINE void SetParamDepthTex(const Texture* input)
		{
			m_pDepthTexture = input;
		}
		BOHGE_FORCEINLINE void SetParamStepSize( const vector2f& input)
		{
			m_Step = input;
		}
		BOHGE_FORCEINLINE void SetParamAOEdgeFinder( float input)
		{
			m_AOEdgeFinder = input;
		}
		BOHGE_FORCEINLINE void SetParamAOIntensity( float input)
		{
			m_AOIntensity = input;
		}
		BOHGE_FORCEINLINE void SetParamJitterAmount( float input)
		{
			m_JitterAmount = input;
		}
	};


	//---------------------------------------------------------------------
	class SSAOCombine : public Shader
	{
	private:
		handle				m_hSceneTexture;
		handle				m_hSSVOTexture;
		const Texture*		m_pSceneTexture;
		const Texture*		m_pSSVOTexture;
	protected:
		virtual void _SetParameters();
	public:
		SSAOCombine();
		virtual ~SSAOCombine();
		virtual bool Initialization();
	public:
		BOHGE_FORCEINLINE void SetParamSceneTexture(const Texture* input)
		{
			m_pSceneTexture = input;
		}
		BOHGE_FORCEINLINE void SetParamSSVOTexture(const Texture* input)
		{
			m_pSSVOTexture = input;
		}
	};








	//---------------------------------------------------------------------
	class FXAAShder : public Shader
	{
	private:
		handle				m_hSceneTexture;
		handle				m_hStepSize;
		const Texture*		m_pSceneTexture;
		vector2f			m_StepSize;
	protected:
		virtual void _SetParameters();
	public:
		FXAAShder();
		virtual bool Initialization();
	public:
		BOHGE_FORCEINLINE void SetParamSceneTexture(const Texture* input)
		{
			m_pSceneTexture = input;
		}
		BOHGE_FORCEINLINE void SetParamStepSize(const vector2f& input)
		{
			m_StepSize = input;
		}
	};

}