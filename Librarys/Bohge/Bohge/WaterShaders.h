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
//		水面相关shader		 //
///////////////////////////////
#pragma once

#include "BaseShader.h"


namespace BohgeEngine
{
	//---------------------------------------------------------------------
	class WaterShader : public MaterialShader//编辑器使用的shader
	{
	private:
		handle			m_hTime;
		handle			m_hNearQ_Q;
		handle			m_hReflrefr_offset;//反射，衍射强度
		handle			m_hViewProjMatrixLoc;
		handle			m_hLightDirectionLoc;
		handle			m_hCameraPositionLoc;
		handle			m_hNoiseTile;
		handle			m_hNoiseWaveSpeed;
		handle			m_hColorChangeDepthInv;
		handle			m_ShadowWorldViewProjBaisLoc;
		handle			m_hLightPositionLoc;
		handle			m_hCameraLineDepthParam;
		handle			m_hBumpScale;
		handle			m_hBumpReduic;
		handle			m_hBumpStrength;

		handle			m_hWaterColorTex;
		handle			m_hDepth;//深度图
		handle			m_hScene;//折射
		handle			m_hNoise;
		handle			m_hSkyBox;
		handle			m_hBump;
		handle			m_hShadowMap;


		handle			m_hCanSeeDepth;
		handle			m_hNoiseStrength;//噪点的强度
		handle			m_hNoiseColor;//噪点的颜色
		//handle			m_hFordWaterColor;//现在读图出颜色
		//handle			m_hDepthWaterColor;
		handle			m_hSpecular_Light;
		handle			m_hShininess;
		handle			m_hFogColor;
		handle			m_hFogParam;
	public:
		WaterShader();
		virtual ~WaterShader(){}
	protected:
		virtual void _SetParameters();
	private:
		virtual bool _CreateShader();
	public:
		virtual bool Initialization();
	};



	//---------------------------------------------------------------------
	class WaterShaderNoShadow : public MaterialShader//编辑器使用的shader
	{
	private:
		handle			m_hTime;
		handle			m_hNearQ_Q;
		handle			m_hReflrefr_offset;//反射，衍射强度
		handle			m_hViewProjMatrixLoc;
		handle			m_hLightDirectionLoc;
		handle			m_hCameraPositionLoc;
		handle			m_hNoiseTile;
		handle			m_hNoiseWaveSpeed;
		handle			m_hColorChangeDepthInv;
		handle			m_hBumpScale;
		handle			m_hBumpReduic;
		handle			m_hBumpStrength;

		handle			m_hWaterColorTex;
		handle			m_hDepth;//深度图
		handle			m_hScene;//折射
		handle			m_hNoise;
		handle			m_hSkyBox;
		handle			m_hBump;


		handle			m_hCanSeeDepth;
		handle			m_hNoiseStrength;//噪点的强度
		handle			m_hNoiseColor;//噪点的颜色

		handle			m_hSpecular_Light;
		handle			m_hShininess;
		handle			m_hFogColor;
		handle			m_hFogParam;
	public:
		WaterShaderNoShadow();
		virtual ~WaterShaderNoShadow(){}
	protected:
		virtual void _SetParameters();
	private:
		virtual bool _CreateShader();
	public:
		virtual bool Initialization();
	};


	////---------------------------------------------------------------------
	//class HighmapGenerateShader : public MaterialShader//编辑器使用的shader
	//{
	//private:
	//	handle			m_hScale;//临近点的纹理坐标增加值
	//	handle			m_hNoise1;//噪点图
	//	handle			m_hNoise2;
	//private:
	//	virtual void SetParameters();
	//public:
	//	HighmapGenerateShader();
	//	virtual bool Initialization();
	//};
	//
	//
	////---------------------------------------------------------------------
	//class NormalmapGenerateShader : public MaterialShader//编辑器使用的shader
	//{
	//private:
	//	handle			m_hStep;//临近点的纹理坐标增加值
	//	handle			m_hcorner00;
	//	handle			m_hcorner01;
	//	handle			m_hcorner10;
	//	handle			m_hcorner11;
	//	handle			m_hAmplitude;
	//
	//	handle			m_hHeightmap;//高度图
	//private:
	//	virtual void SetParameters();
	//public:
	//	NormalmapGenerateShader();
	//	virtual bool Initialization();
	//};



	//---------------------------------------------------------------------
	class WaterShaderNormal : public MaterialShader//编辑器使用的shader
	{
	private:
		handle			m_hViewProjMatrixLoc;
	protected:
		virtual void _SetParameters();
	public:
		WaterShaderNormal();
		virtual bool Initialization();
	};



	//---------------------------------------------------------------------
	class WaterShaderMedium : public WaterShader
	{
	private:
		virtual bool _CreateShader();
	};
	//---------------------------------------------------------------------
	class WaterShaderNoShadowMedium : public WaterShaderNoShadow
	{
	private:
		virtual bool _CreateShader();
	};
}