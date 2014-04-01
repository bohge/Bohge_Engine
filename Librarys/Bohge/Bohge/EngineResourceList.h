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
//			引擎中资源的列表	   //
/////////////////////////////////////
#pragma once
#include "Predefine.h"
#include "IOSystem.h"

#include <map>
#include <string>



namespace BohgeEngine
{
	//字体资源
	static const char* FontName = "font.bf";
#define BS(x) IOSystem::Instance().ShaderFolder()+(x)

	//引擎的shader的索引必须是唯一的
	class ShaderResource
	{
	public:
		enum ShaderName
		{
			rPrePixelLight,
			rPrePixelLightTransform,
			rShadowVolume,
			rShadowVolumeInstance,
			rConstant,
			rUIShader,
			rScreenShapes,
			rWorldShapes,
			rBillboardShader,
			rLuminance,
			rGaussian,
			rCombine,
			rVolumeLight,
			rVolumeLightPost,
			rVolumeLightPostLow,
			rFontShader,
			rParticleShader,
			rPipelineCombine,
			rQuadShader,
			rBlur3x3Shader,
			rPrePixelLightInstance,

			//环境
			rHaloShader,
			rSkyBox,
			rSobel,
			rBlurEdge,
			rVolumeLightCombine,
			rFakeLightShader,

			//模型
			rSkeletonModel1B,
			rSkeletonModel2B,
			rSkeletonModel3B,
			rSkeletonModel4B,
			rSkeletonModelBump1B,
			rSkeletonModelBump2B,
			rSkeletonModelBump3B,
			rSkeletonModelBump4B,
			rModelPrePixelLight,
			rModelPointLightPerpixel,
			rModelSpotLightPerpixel,
			rModelBump,
			rModelLightMaped,

			//地形
			rTerrainCDLOD,
			rTerrainCDLODNoShadow,
			rTerrainCDLODLow,
			rTerrainLineCDLOD,

			//水面
			rWaterGrid,
			rWaterGridMedium,
			rWaterGridNoShadow,
			rWaterGridMediumNoShadow,
			rHighmapGenerateShader,
			rHNormalmapGenerateShader,

			//植被
			rPlantTrunkShader,
			rPlantLeafShader,

			//阴影，深度
			rDepthLinearShader,
			rCopyDepthToRBGAShader,
			rGaussianDepth,
			rSkeletonModelDepth1B,
			rSkeletonModelDepth2B,
			rSkeletonModelDepth3B,
			rSkeletonModelDepth4B,
			rModelDepth,
			rModelDepthHigh,
			rGaussianDepthLow,

			//HDR的
			rHDRGenerateLuminance,
			rHDRAverageLuminance,
			rHDRAdaptLuminance,
			rHDRBrightPass,
			rBlur2x2Shader,
			rHDRCombine,

			//DOF
			rDOFCombine,

			//FSAA
			rFXAAShader,

			//ao
			rSSVOshader,
			rSSVOCombine,

			//法显
			rSkeletonModelNormal1B,
			rSkeletonModelNormal2B,
			rSkeletonModelNormal3B,
			rSkeletonModelNormal4B,
			rModelNormal,
			rTerrainCDLODNormal,
			rWaterNormal,
			rPlantTrunkNormal,
			rPlantLeafNormal,

			SHADER_COUNT,
		};
	private:
		std::map< int, std::pair<std::string, std::string> > m_ShaderFileName;
	private:
		ShaderResource()
		{	
			AddShader( rPrePixelLight,				BS("PerPixelLighting.vs"),			BS("PerPixelLighting.ps") );
			AddShader( rPrePixelLightTransform,		BS("PerPixelLightingTransform.vs"), BS("PerPixelLightingTransform.ps") );
			AddShader( rShadowVolume,				BS("ShadowVolume.vs"),				BS("ShadowVolume.ps") );
			AddShader( rShadowVolumeInstance,		BS("ShadowVolumeInstance.vs"),		BS("ShadowVolume.ps") );
			AddShader( rConstant,					BS("Constant.vs"),					BS("Constant.ps") );
			AddShader( rUIShader,					BS("UIShader.vs"),					BS("UIShader.ps") );
			AddShader( rScreenShapes,				BS("ScreenShapes.vs"),				BS("ShapesShader.ps") );
			AddShader( rWorldShapes,				BS("WorldShapes.vs"),				BS("ShapesShader.ps") );
			AddShader( rBillboardShader,			BS("BillboardShader.vs"),			BS("BillboardShader.ps") );
			AddShader( rHaloShader,					BS("HaloShader.vs"),				BS("HaloShader.ps") );
			AddShader( rFakeLightShader,			BS("FakeLightShader.vs"),			BS("FakeLightShader.ps") );
			AddShader( rLuminance,					BS("Luminance.vs"),					BS("Luminance.ps") );
			AddShader( rGaussian,					BS("Gaussian.vs"),					BS("Gaussian.ps") );
			AddShader( rCombine,					BS("Quad.vs"),						BS("Combine.ps") );
			AddShader( rVolumeLight,				BS("VolumeLight.vs"),				BS("VolumeLight.ps") );
			AddShader( rVolumeLightPost,			BS("VolumeLightPost.vs"),			BS("VolumeLightPost.ps") );
			AddShader( rVolumeLightPostLow,			BS("VolumeLightPostLow.vs"),		BS("VolumeLightPostLow.ps") );
			AddShader( rFontShader,					BS("Font.vs"),						BS("Font.ps") );
			AddShader( rParticleShader,				BS("Particle.vs"),					BS("Particle.ps") );
			AddShader( rPipelineCombine,			BS("Quad.vs"),						BS("PipelineCombine.ps") );
			AddShader( rQuadShader,					BS("Quad.vs"),						BS("Quad.ps") );
			AddShader( rSkyBox,						BS("SkyBox.vs"),					BS("SkyBox.ps") );
			AddShader( rSobel,						BS("Quad.vs"),						BS("Sobel.ps") );
			AddShader( rBlurEdge,					BS("Quad.vs"),						BS("GradientBlur.ps") );
			AddShader( rVolumeLightCombine,			BS("Quad.vs"),						BS("CombineVolumeLight.ps") );
			AddShader( rBlur3x3Shader,				BS("Blur3x3.vs"),					BS("Blur3x3.ps") );
			AddShader( rPrePixelLightInstance,		BS("PerPixelLightingInstance.vs"),	BS("PerPixelLighting.ps") );

			//模型
			AddShader( rSkeletonModel1B,			BS("SkeletonModel1B.vs"),			BS("SkeletalModel.ps") );
			AddShader( rSkeletonModel2B,			BS("SkeletonModel2B.vs"),			BS("SkeletalModel.ps") );
			AddShader( rSkeletonModel3B,			BS("SkeletonModel3B.vs"),			BS("SkeletalModel.ps") );
			AddShader( rSkeletonModel4B,			BS("SkeletonModel4B.vs"),			BS("SkeletalModel.ps") );
			AddShader( rSkeletonModelBump1B,		BS("SkeletonModelBump1B.vs"),		BS("SkeletalModelBump.ps") );
			AddShader( rSkeletonModelBump2B,		BS("SkeletonModelBump2B.vs"),		BS("SkeletalModelBump.ps") );
			AddShader( rSkeletonModelBump3B,		BS("SkeletonModelBump3B.vs"),		BS("SkeletalModelBump.ps") );
			AddShader( rSkeletonModelBump4B,		BS("SkeletonModelBump4B.vs"),		BS("SkeletalModelBump.ps") );

			AddShader( rModelPrePixelLight,			BS("ModelPerPixelLight.vs"),		BS("SkeletalModel.ps") );
			AddShader( rModelPointLightPerpixel,	BS("ModelPointLightPerPixel.vs"),	BS("ModelPointLightPerPixel.ps") );
			AddShader( rModelSpotLightPerpixel,		BS("ModelSpotLightPerPixel.vs"),	BS("ModelSpotLightPerPixel.ps") );
			AddShader( rModelBump,					BS("ModelBump.vs"),					BS("SkeletalModelBump.ps") );
			AddShader( rModelLightMaped,			BS("ModeLightMaped.vs"),			BS("LightMaped.ps") );


			//地形
			AddShader( rTerrainCDLOD,				BS("TerrainCDLOD.vs"),				BS("TerrainCDLOD.ps") );
			AddShader( rTerrainCDLODNoShadow,		BS("TerrainCDLOD.vs"),				BS("TerrainCDLOD_NoShadow.ps") );
			AddShader( rTerrainCDLODLow,			BS("TerrainCDLODLow.vs"),			BS("TerrainCDLODLow.ps") );
			AddShader( rTerrainLineCDLOD,			BS("TerrainLineCDLOD.vs"),			BS("TerrainLineCDLOD.ps") );

			//水面
			AddShader( rWaterGrid,					BS("WaterGrid.vs"),					BS("WaterGrid.ps") );
			AddShader( rWaterGridNoShadow,			BS("WaterGrid.vs"),					BS("WaterGrid_NoShadow.ps") );
			AddShader( rHighmapGenerateShader,		BS("HighmapGenerateShader.vs"),		BS("HighmapGenerateShader.ps") );
			AddShader( rHNormalmapGenerateShader,	BS("NormalmapGenerateShader.vs"),	BS("NormalmapGenerateShader.ps") );
			AddShader( rWaterGridMedium,			BS("WaterGrid.vs"),					BS("WaterGridMedium.ps") );
			AddShader( rWaterGridMediumNoShadow,	BS("WaterGrid.vs"),					BS("WaterGridMedium_NoShadow.ps") );


			//植被
			AddShader( rPlantTrunkShader,			BS("PlantTrunk.vs"),				BS("PlantTrunk.ps") );
			AddShader( rPlantLeafShader,			BS("PlantLeaf.vs"),					BS("PlantLeaf.ps") );

			//深度，阴影
			AddShader( rDepthLinearShader,			BS("Quad.vs"),						BS("DepthLinear.ps") );
			AddShader( rCopyDepthToRBGAShader,		BS("Quad.vs"),						BS("CopyDepthToRBGA.ps") );
			AddShader( rGaussianDepth,				BS("Gaussian.vs"),					BS("GaussianDepth.ps") );
			AddShader( rSkeletonModelDepth1B,		BS("SkeletonModelDepth1B.vs"),		BS("PackDepth.ps") );
			AddShader( rSkeletonModelDepth2B,		BS("SkeletonModelDepth2B.vs"),		BS("PackDepth.ps") );
			AddShader( rSkeletonModelDepth3B,		BS("SkeletonModelDepth3B.vs"),		BS("PackDepth.ps") );
			AddShader( rSkeletonModelDepth4B,		BS("SkeletonModelDepth4B.vs"),		BS("PackDepth.ps") );
			AddShader( rModelDepth,					BS("ModelDepth.vs"),				BS("PackDepth.ps") );
			AddShader( rModelDepthHigh,				BS("ModelDepthHigh.vs"),			BS("PackDepthHigh.ps") );			
			AddShader( rGaussianDepthLow,			BS("Gaussian.vs"),					BS("GaussianDepthLow.ps") );

			//hdr
			AddShader( rHDRGenerateLuminance,		BS("HDRGenerateLuminance.vs"),		BS("HDRGenerateLuminance.ps") );
			AddShader( rHDRAverageLuminance,		BS("Quad.vs"),						BS("HDRAverageLuminance.ps") );
			AddShader( rHDRAdaptLuminance,			BS("Quad.vs"),						BS("HDRAdaptLuminance.ps") );
			AddShader( rHDRBrightPass,				BS("Quad.vs"),						BS("HDRBrightPass.ps") );
			AddShader( rBlur2x2Shader,				BS("Blur2x2Shader.vs"),				BS("Blur2x2Shader.ps") );
			AddShader( rHDRCombine,					BS("Quad.vs"),						BS("HDRCombine.ps") );

			//dof
			AddShader( rDOFCombine,					BS("Quad.vs"),						BS("DOFCombine.ps") );
			AddShader( rFXAAShader,					BS("FXAA.vs"),						BS("FXAA.ps") );

			//ssvo
			AddShader( rSSVOshader,					BS("Quad.vs"),						BS("SSAO.ps") );
			AddShader( rSSVOCombine,				BS("Quad.vs"),						BS("SSAOCombine.ps") );

			//法线
			AddShader( rSkeletonModelNormal1B,		BS("SkeletonModelNormal1B.vs"),		BS("PackNormal.ps") );
			AddShader( rSkeletonModelNormal2B,		BS("SkeletonModelNormal2B.vs"),		BS("PackNormal.ps") );
			AddShader( rSkeletonModelNormal3B,		BS("SkeletonModelNormal3B.vs"),		BS("PackNormal.ps") );
			AddShader( rSkeletonModelNormal4B,		BS("SkeletonModelNormal4B.vs"),		BS("PackNormal.ps") );
			AddShader( rModelNormal,				BS("ModelNormal.vs"),				BS("PackNormal.ps") );
			AddShader( rTerrainCDLODNormal,			BS("TerrainCDLODNormal.vs"),		BS("PackNormal.ps") );
			AddShader( rWaterNormal,				BS("WaterNormal.vs"),				BS("PackNormal.ps") );
			AddShader( rPlantTrunkNormal,			BS("PlantTrunkNormal.vs"),			BS("PackNormal.ps") );
			AddShader( rPlantLeafNormal,			BS("PlantLeafNormal.vs"),			BS("PlantLeafNormal.ps") );

		}
	public:
		static BOHGE_FORCEINLINE ShaderResource& Instance()
		{
			static ShaderResource sr;
			return sr;
		}
		BOHGE_FORCEINLINE void AddShader( int sn, const std::string& vs, const std::string& ps )
		{
			m_ShaderFileName.insert( std::make_pair( sn, std::make_pair( vs, ps ) ) );
		}
		BOHGE_FORCEINLINE const std::string& GetShaderVS( int sn )
		{
			return m_ShaderFileName.find( sn )->second.first;
		}
		BOHGE_FORCEINLINE const std::string& GetShaderPS( int sn )
		{
			return m_ShaderFileName.find( sn )->second.second;
		}
	};


}