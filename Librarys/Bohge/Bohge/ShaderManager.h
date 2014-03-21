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



/////////////////////////////////////////
//			enter point of game		   //
/////////////////////////////////////////
#pragma once
#include "Predefine.h"


#include <vector>



namespace BohgeEngine
{

	class Shader;
	class MaterialShader;
	class AssembledMaterialShader;
	//---------------------------------------------------------------------
	class ShaderManager
	{
	public:
		enum ShaderName
		{
			//PrePixelLight,
			//PrePixelLightTransform,
			//ShadowVolume,
			//ShadowVolumeInstance,
			//Constant,
			UIShader = 0,
			ScreenShapes,
			WorldShapes,
			BillboardShader,
			Luminance,
			Gaussian,
			Combine,
			//VolumeLight,
			VolumeLightPost,
			FontShader,
			ParticleShader,
			PipelineCombine,
			QuadShader,
			SkyBoxShader,
			SobelShader,
			VolumeLightCombine,
			BlurEdgaShader,
			BlurImageShader,

			//模型
			//PrePixelLightInstance,
			SkeletonModel1B,
			SkeletonModel2B,
			SkeletonModel3B,
			SkeletonModel4B,
			SkeletonModelBump1B,
			SkeletonModelBump2B,
			SkeletonModelBump3B,
			SkeletonModelBump4B,
			ModelPrePixelLight,
			ModelLightMaped,
			ModelPointLightPerpixel,
			ModelSpotLightPerpixel,
			ModelBump,

			//地形
			TerrainCDLOD,
			TerrainCDLODNoShadow,
			TerrainCDLODLow,
			//TerrainLineCDLOD,
			WaterShader,
			WaterShaderMedium,
			WaterShaderNoShadow,
			WaterShaderNoShadowMedium,
			//HighmapGenerateShader,
			//NormalmapGenerateShader,
			CopyDepthToRBGAShader,
			PlantTrunk,
			PlantLeaf,
			GaussianDepth,
			SkeletonModelDepth1B,
			SkeletonModelDepth2B,
			SkeletonModelDepth3B,
			SkeletonModelDepth4B,
			ModelDepth,
			GaussianDepthLow,

			//HDR的
			HDRGenerateLuminance,
			HDRAverageLuminance,
			HDRAdaptLuminance,
			HDRBrightPass,
			Blur2x2Shader,
			HDRCombine,

			DOFCombine,

			FXAAShder,

			SSAOShader,
			SSAOCombine,

			//发现输出
			SkeletonModelNormal1B,
			SkeletonModelNormal2B,
			SkeletonModelNormal3B,
			SkeletonModelNormal4B,
			ModelNormal,
			TerrainCDLODNormal,
			PlantTrunkNormal,
			PlantLeafNormal,
			WaterShaderNormal,


			//环境的
			HaloShader,
			FakeLightShader,

			CountShader,
		};
	private:
		enum Constant
		{
			SC_MAX_COUNT = 100,//老旧形式的shader最多100个
			SC_INCRES = 20,
		};
	private:
		typedef std::vector<Shader*>	ShaderVector;//初始化加载慢，高速访问
	private:
		int				m_AssembledShaderIndex;
		ShaderVector	m_mpShaderVector;
	public:
		ShaderManager();
		~ShaderManager();
		void InsertShader( int index, Shader* s );
		int InsertAssembledShader( AssembledMaterialShader* s );
		MaterialShader* GetMaterialShader(int index);
	public:
		template<typename T>
		BOHGE_FORCEINLINE T& GetShader(int index)
		{
#ifdef _DEBUG
			ASSERT( NULL != dynamic_cast<T*>( m_mpShaderVector[index] ) );//如果要求转换的指针不是目标指针，此处会报错
#endif
			return static_cast<T&>(*( m_mpShaderVector[index] )); 
		}
	};

}