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
//			State Manage class		   //
/////////////////////////////////////////

#include "ShaderManage.h"
#include "ConcisePEShaders.h"
#include "EnvironmentShaders.h"
#include "HDRShaders.h"
#include "ModelShaders.h"
#include "ParticleShaders.h"
#include "PlantShaders.h"
#include "PrePixelLightShaders.h"
#include "Shaders2D.h"
#include "ShadowShader.h"
#include "TerrainShaders.h"
#include "UtilityShaders.h"
#include "VolumeLightShaders.h"
#include "WaterShaders.h"
#include "ModelSkeleton.h"



#include "AssembledMaterialShader.h"



using namespace std;

namespace BohgeEngine
{

	/////////////////////////
	//		ShaderManage   //
	/////////////////////////
	ShaderManage::ShaderManage()
		:m_AssembledShaderIndex(SC_MAX_COUNT)
	{
		m_mpShaderVector.resize( SC_MAX_COUNT );
		memset( &m_mpShaderVector[0], 0, m_mpShaderVector.size() );
		//m_mpShaderVector[PrePixelLight] = NEW class PrePixelLight();
		//m_mpShaderVector[PrePixelLightTransform] = NEW class PrePixelLightTransform();
		//m_mpShaderVector[ShadowVolume] = NEW class ShadowVolume();
		//m_mpShaderVector[ShadowVolumeInstance] = NEW class ShadowVolumeInstance();
		//m_mpShaderVector[Constant] = NEW class Constant();
		m_mpShaderVector[UIShader] = NEW class UIShader();
		m_mpShaderVector[ScreenShapes] = NEW class ScreenShapes();
		m_mpShaderVector[WorldShapes] = NEW class WorldShapes();
		m_mpShaderVector[BillboardShader] = NEW class BillboardShader();
		m_mpShaderVector[Luminance] = NEW class Luminance();
		m_mpShaderVector[Gaussian] = NEW class Gaussian();
		m_mpShaderVector[Combine] = NEW class Combine();
		//m_mpShaderVector[VolumeLight] = NEW class VolumeLight();
		m_mpShaderVector[VolumeLightPost] = NEW class VolumeLightPost();
		m_mpShaderVector[FontShader] = NEW class FontShader();
		m_mpShaderVector[ParticleShader] = NEW class ParticleShader();
		m_mpShaderVector[PipelineCombine] = NEW class PipelineCombine();
		m_mpShaderVector[QuadShader] = NEW class QuadShader();
		m_mpShaderVector[SkyBoxShader] = NEW class SkyBoxShader();
		m_mpShaderVector[SobelShader] = NEW class SobelShader();
		m_mpShaderVector[BlurEdgaShader] = NEW class BlurEdgaShader();
		m_mpShaderVector[VolumeLightCombine] = NEW class VolumeLightCombine();
		m_mpShaderVector[BlurImageShader] = NEW class BlurImageShader();
		//m_mpShaderVector[PrePixelLightInstance] = NEW class PrePixelLightInstance();
		m_mpShaderVector[SkeletonModel1B] = NEW class SkeletonModel1B();
		m_mpShaderVector[SkeletonModel2B] = NEW class SkeletonModel2B();
		m_mpShaderVector[SkeletonModel3B] = NEW class SkeletonModel3B();
		m_mpShaderVector[SkeletonModel4B] = NEW class SkeletonModel4B();
		m_mpShaderVector[SkeletonModelBump1B] = NEW class SkeletonModelBump1B();
		m_mpShaderVector[SkeletonModelBump2B] = NEW class SkeletonModelBump2B();
		m_mpShaderVector[SkeletonModelBump3B] = NEW class SkeletonModelBump3B();
		m_mpShaderVector[SkeletonModelBump4B] = NEW class SkeletonModelBump4B();
		m_mpShaderVector[ModelPrePixelLight] = NEW class ModelPrePixelLight();
		m_mpShaderVector[ModelSpotLightPerpixel] = NEW class ModelSpotLightPerpixel();
		m_mpShaderVector[ModelPointLightPerpixel] = NEW class ModelPointLightPerpixel();
		m_mpShaderVector[ModelLightMaped] = NEW class ModelLightMaped();
		m_mpShaderVector[ModelBump] = NEW class ModelBump();


		m_mpShaderVector[TerrainCDLOD] = NEW class TerrainCDLOD();
		m_mpShaderVector[TerrainCDLODNoShadow] = NEW class TerrainCDLODNoShadow();
		m_mpShaderVector[TerrainCDLODLow] = NEW class TerrainCDLODLow();
		//m_mpShaderVector[TerrainLineCDLOD] = NEW class TerrainLineCDLOD();
		m_mpShaderVector[WaterShader] = NEW class WaterShader();
		m_mpShaderVector[WaterShaderMedium] = NEW class WaterShaderMedium();
		m_mpShaderVector[WaterShaderNoShadow] = NEW class WaterShaderNoShadow();
		m_mpShaderVector[WaterShaderNoShadowMedium] = NEW class WaterShaderNoShadowMedium();
		//m_mpShaderVector[HighmapGenerateShader] = NEW class HighmapGenerateShader();
		//m_mpShaderVector[NormalmapGenerateShader] = NEW class NormalmapGenerateShader();
		m_mpShaderVector[CopyDepthToRBGAShader] = NEW class CopyDepthToRBGAShader();
		m_mpShaderVector[PlantTrunk] = NEW class PlantTrunk();
		m_mpShaderVector[PlantLeaf] = NEW class PlantLeaf();
		m_mpShaderVector[GaussianDepth] = NEW class GaussianDepth();
		m_mpShaderVector[SkeletonModelDepth1B] = NEW class SkeletonModelDepth1B();
		m_mpShaderVector[SkeletonModelDepth2B] = NEW class SkeletonModelDepth2B();
		m_mpShaderVector[SkeletonModelDepth3B] = NEW class SkeletonModelDepth3B();
		m_mpShaderVector[SkeletonModelDepth4B] = NEW class SkeletonModelDepth4B();
		m_mpShaderVector[ModelDepth] = NEW class ModelDepth();
		m_mpShaderVector[GaussianDepthLow] = NEW class GaussianDepthLow();
		m_mpShaderVector[HDRGenerateLuminance] = NEW class HDRGenerateLuminance();
		m_mpShaderVector[HDRAverageLuminance] = NEW class HDRAverageLuminance();
		m_mpShaderVector[HDRAdaptLuminance] = NEW class HDRAdaptLuminance();
		m_mpShaderVector[HDRBrightPass] = NEW class HDRBrightPass();
		m_mpShaderVector[Blur2x2Shader] = NEW class Blur2x2Shader();
		m_mpShaderVector[HDRCombine] = NEW class HDRCombine();
		m_mpShaderVector[DOFCombine] = NEW class DOFCombine();
		m_mpShaderVector[SSAOShader] = NEW class SSAOShader();
		m_mpShaderVector[SSAOCombine] = NEW class SSAOCombine();
		m_mpShaderVector[FXAAShder] = NEW class FXAAShder();
		m_mpShaderVector[SkeletonModelNormal1B] = NEW class SkeletonModelNormal1B();
		m_mpShaderVector[SkeletonModelNormal2B] = NEW class SkeletonModelNormal2B();
		m_mpShaderVector[SkeletonModelNormal3B] = NEW class SkeletonModelNormal3B();
		m_mpShaderVector[SkeletonModelNormal4B] = NEW class SkeletonModelNormal4B();
		m_mpShaderVector[ModelNormal] = NEW class ModelNormal();
		m_mpShaderVector[TerrainCDLODNormal] = NEW class TerrainCDLODNormal();
		m_mpShaderVector[PlantTrunkNormal] = NEW class PlantTrunkNormal();
		m_mpShaderVector[PlantLeafNormal] = NEW class PlantLeafNormal();
		m_mpShaderVector[WaterShaderNormal] = NEW class WaterShaderNormal();
		m_mpShaderVector[HaloShader] = NEW class HaloShader();
		m_mpShaderVector[FakeLightShader] = NEW class FakeLightShader();


		for ( ShaderVector::iterator it = m_mpShaderVector.begin() ;
			it != m_mpShaderVector.end();
			it ++ )
		{
			if ( NULL != (*it) )
			{
				(*it)->Initialization();
			}
		}
	}
	//----------------------------------------------------------------------------------------------------------
	ShaderManage::~ShaderManage()
	{
		for ( ShaderVector::iterator it = m_mpShaderVector.begin() ;
			it != m_mpShaderVector.end();
			it ++ )
		{
			SAFE_DELETE( *it );
		}
	}
	//----------------------------------------------------------------------------------------------------------
	void ShaderManage::InsertShader( int index, Shader* s )
	{
		ASSERT( SC_MAX_COUNT > index );
		m_mpShaderVector[index] = s;
		s->Initialization();
	}
	//----------------------------------------------------------------------------------------------------------
	int ShaderManage::InsertAssembledShader( AssembledMaterialShader* s )
	{
		if ( m_mpShaderVector.size() <= m_AssembledShaderIndex )
		{
			m_mpShaderVector.resize( m_AssembledShaderIndex + SC_INCRES );
		}
		m_mpShaderVector[m_AssembledShaderIndex] = dynamic_cast<Shader*>(s);
		int res = m_AssembledShaderIndex++;
		return res;
	}
	//----------------------------------------------------------------------------------------------------------
	MaterialShader* ShaderManage::GetMaterialShader( int index )
	{
#ifdef _DEBUG
		ASSERT( NULL != dynamic_cast<MaterialShader*>( m_mpShaderVector[index]) );//如果要求转换的指针不是目标指针，此处会报错
#endif
		return static_cast<MaterialShader*>(m_mpShaderVector[index]); 
	}
//----------------------------------------------------------------------------------------------------------

}