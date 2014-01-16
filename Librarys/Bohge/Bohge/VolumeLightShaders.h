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



///////////////////////////////
//		体积光的shader		 //
///////////////////////////////
#pragma once
#include "Predefine.h"
#include "BaseShader.h"
#include "3DMath.h"

namespace BohgeEngine
{
	class Texture;
	//---------------------------------------------------------------------
	class VolumeLight : public Shader
	{
	private:
		handle				m_hTexOrginDepthHandle;
		handle				m_hTexLightDepthHandle;
		handle				m_hCameraPostionHandle;
		handle				m_hViewProjInversHandle;
		handle				m_hLightViewProjHandle;
		handle				m_hSenceNearHandle;
		handle				m_hSenceFarNearHandle;
		handle				m_hSenceFarNearQHandle;
		handle				m_hLightNearHandle;
		handle				m_hLightFarNearHandle;
		handle				m_hLightFarNearQHandle;
		const Texture*		m_pTexOrginial;
		const Texture*		m_pTexLight;
	protected:
		virtual void _SetParameters();
	public:
		VolumeLight();
		virtual ~VolumeLight();
		virtual bool Initialization();
	public:
		BOHGE_FORCEINLINE void SetParamOrginial(const Texture* input)
		{
			m_pTexOrginial = input;
		}
		BOHGE_FORCEINLINE void SetParamLight(const Texture* input)
		{
			m_pTexLight = input;
		}

	};


	//---------------------------------------------------------------------
	//后期处理的方法计算Volumelight
	class VolumeLightPost : public Shader
	{
	private:
		handle				m_hExposureHandle;
		handle				m_hDecayHandle;
		handle				m_hDensityHandle;
		handle				m_hWeightHandle;
		handle				m_hLightPositionOnScreenHandle;
		handle				m_hTextureHandle;
		const Texture*		m_pTexOrginial;
	protected:
		virtual void _SetParameters();
	public:
		VolumeLightPost();
		virtual ~VolumeLightPost();
		virtual bool Initialization();
	public:
		BOHGE_FORCEINLINE void SetParamOrginial(const Texture* input)
		{
			m_pTexOrginial = input;
		}
	};




	//---------------------------------------------------------------------
	class SobelShader : public Shader
	{
	private:
		handle					m_hTextureWidthInv;
		handle					m_hTextureHeightInv;
		float					m_fTextureWidthInv;
		float					m_fTextureHeightInv;
	protected:
		virtual void _SetParameters();
	public:
		SobelShader();
		virtual ~SobelShader();
		virtual bool Initialization();
		BOHGE_FORCEINLINE void SetParamTextureSize(const vector2d& size)
		{
			m_fTextureWidthInv = 1.0f / size.m_x;
			m_fTextureHeightInv = 1.0f / size.m_y;
		}
	};

	//---------------------------------------------------------------------
	class BlurEdgaShader : public Shader
	{
	private:
		handle					m_hTextureWidthInv;
		handle					m_hTextureHeightInv;
		float					m_fTextureWidthInv;
		float					m_fTextureHeightInv;

		handle					m_hTextureEdga;
		handle					m_hTextureScale;
		const Texture*			m_pTexEdga;
		const Texture*			m_pTexScale;
	protected:
		virtual void _SetParameters();
	public:
		virtual bool Initialization();
		BOHGE_FORCEINLINE void SetParamTextureSize(const vector2d& size)
		{
			m_fTextureWidthInv = 1.0f / size.m_x;
			m_fTextureHeightInv = 1.0f / size.m_y;
		}
		BOHGE_FORCEINLINE void SetParamEdgaTexture(const Texture* input)
		{
			m_pTexEdga = input;
		}
		BOHGE_FORCEINLINE void SetParamScaleTexture(const Texture* input)
		{
			m_pTexScale = input;
		}
	};


	//---------------------------------------------------------------------
	class BlurImageShader : public Shader
	{
	private:
		handle					m_hTextureWidthInv;
		handle					m_hTextureHeightInv;
		float					m_fTextureWidthInv;
		float					m_fTextureHeightInv;
	protected:
		virtual void _SetParameters();
	public:
		virtual bool Initialization();
		BOHGE_FORCEINLINE void SetParamTextureSize(const vector2d& size)
		{
			m_fTextureWidthInv = 1.0f / size.m_x;
			m_fTextureHeightInv = 1.0f / size.m_y;
		}
	};



	//---------------------------------------------------------------------
	class VolumeLightCombine : public Shader
	{
	private:
		handle				m_hTexOrginialHandle;
		handle				m_hTexSecenHandle;
		handle				m_hColor;
		const Texture*		m_pTexOrginial;
		const Texture*		m_pTexSecen;
	protected:
		virtual void _SetParameters();
	public:
		VolumeLightCombine();
		virtual ~VolumeLightCombine();
		virtual bool Initialization();
	public:
		BOHGE_FORCEINLINE void SetParamOrginial(const Texture* input)
		{
			m_pTexOrginial = input;
		}
		BOHGE_FORCEINLINE void SetParamSecen(const Texture* input)
		{
			m_pTexSecen = input;
		}
	};
}