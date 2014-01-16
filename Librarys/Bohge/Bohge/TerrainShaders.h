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
//		地形相关shader		 //
///////////////////////////////
#pragma once

#include "BaseShader.h"




namespace BohgeEngine
{
	//---------------------------------------------------------------------
	class TerrainCDLOD : public MaterialShader//CDLOD地形
	{
	private:
		handle			m_hViewProjMatrixLoc;
		handle			m_hLightDirectionLoc;
		handle			m_hCameraPositionLoc;
		handle			m_hMorphXY_SteperScaleInvers_DoubleChunkSize;
		handle			m_hSizeScale_SizeScaleRepeat;
		handle			m_hDetailIndex;
		handle			m_hOffsetScale;
		handle			m_ShadowWorldViewProjBaisLoc;
		handle			m_hLightPositionLoc;
		handle			m_hCameraLineDepthParam;

		handle			m_hTextureBase;
		handle			m_hTextureDiffuse;
		handle			m_hTextureWeight;
		handle			m_hRTexture;
		handle			m_hGTexture;
		handle			m_hBTexture;
		handle			m_hDetalTexture;
		handle			m_hShadowMap;
		handle			m_hDetalDistanceInv;

		handle			m_hAmbient_AmbientLight;
		handle			m_hSpecular_Light;
		handle			m_hLight;
		handle			m_hShininess;

		handle			m_hFogColor;
		handle			m_hFogParam;
	protected:
		virtual void _SetParameters();
	public:
		TerrainCDLOD();
		virtual bool Initialization();
	};


	//---------------------------------------------------------------------
	class TerrainCDLODNoShadow : public MaterialShader//CDLOD地形
	{
	private:
		handle			m_hViewProjMatrixLoc;
		handle			m_hLightDirectionLoc;
		handle			m_hCameraPositionLoc;
		handle			m_hMorphXY_SteperScaleInvers_DoubleChunkSize;
		handle			m_hSizeScale_SizeScaleRepeat;
		handle			m_hDetailIndex;
		handle			m_hOffsetScale;

		handle			m_hTextureBase;
		handle			m_hTextureDiffuse;
		handle			m_hTextureWeight;
		handle			m_hRTexture;
		handle			m_hGTexture;
		handle			m_hBTexture;
		handle			m_hDetalTexture;
		handle			m_hDetalDistanceInv;

		handle			m_hAmbient_AmbientLight;
		handle			m_hSpecular_Light;
		handle			m_hLight;
		handle			m_hShininess;

		handle			m_hFogColor;
		handle			m_hFogParam;
	protected:
		virtual void _SetParameters();
	public:
		TerrainCDLODNoShadow();
		virtual bool Initialization();
	};



	//---------------------------------------------------------------------
	class TerrainCDLODLow : public MaterialShader//CDLOD地形
	{
	private:
		handle			m_hViewProjMatrixLoc;
		handle			m_hCameraPositionLoc;
		handle			m_hMorphXY_SteperScaleInvers_DoubleChunkSize;
		handle			m_hSizeScale_SizeScaleRepeat;
		handle			m_hDetailIndex;
		handle			m_hOffsetScale;

		handle			m_hFogColor;
		handle			m_hFogParam;
		handle			m_hTextureDiffuse;
	protected:
		virtual void _SetParameters();
	public:
		TerrainCDLODLow();
		virtual bool Initialization();
	};




	//---------------------------------------------------------------------
	class TerrainCDLODNormal : public MaterialShader//CDLOD地形
	{
	private:
		handle			m_hViewProjMatrixLoc;
		handle			m_hCameraPositionLoc;
		handle			m_hMorphXY_SteperScaleInvers_DoubleChunkSize;
		handle			m_hDetailIndex;
		handle			m_hOffsetScale;
	protected:
		virtual void _SetParameters();
	public:
		TerrainCDLODNormal();
		virtual bool Initialization();
	};
}