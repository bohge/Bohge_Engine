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
#pragma once
#include "Framework/ShaderAttributes.h"
#include "Framework/ShaderParameter.h"
#include "Framework/LimitedIndex.h"
#include "Framework/JsonReadable.h"
#include "EmitterProperty.h"
#include "Framework/Node.h"



#include <EASTL/list.h>

namespace BohgeEngine
{	
	class IAsyncSource;
	class VertexStream;
	class IndicesStream;
	class RenderComponent;
	class TransformComponent;
	class OrientationComponent;
}
namespace ParticleSystemPlugin
{
	class IAffector;
	class RandVolume;
	class GeneralEmitter : public BohgeEngine::Node, public BohgeEngine::JsonReadable
	{
		RTTI_TYPE_DECLEAR( GeneralEmitter );
	private:
		enum Constant
		{
			GEC_VERTEX_COUNT_EACH_PARTICLE = 4,//每个粒子顶点个数
			GEC_PARTICLE_OFFSET = 1,
		};
	private:
		typedef eastl::list< IAffector* > IAffectorList;
		typedef eastl::list< eastl::pair< EmitterProperty::EmitterAffector, RandVolume* > > AffectorVolumePairList;
	private:
		IAffectorList						m_AffectorList;
		BohgeEngine::LimitedIndex			m_LimitedIndex;
		AffectorVolumePairList				m_AffectorVolumePairList;
		RandVolume*							m_pDensityVolume;
		BohgeEngine::OrientationComponent*	m_pOrientation;
		BohgeEngine::TransformComponent*	m_pTransform;
		BohgeEngine::RenderComponent*		m_pRender;
		BohgeEngine::VertexStream*			m_pVertexStream;
		BohgeEngine::IndicesStream*			m_pIndicesStream;
		float								m_TimeSpan;//没有生成粒子的时间间隔
	public:
		GeneralEmitter(void);
		virtual ~GeneralEmitter(void);
	private:
		void _Load_1( BohgeEngine::JsonReader& reader );
	public:
		void MessageLoaclRotation( const BohgeEngine::NodeMessage& message );
	private:
		void _ParseAffector();
		void _InitializeStream();
		void _CaculateMaxParticleCount( BohgeEngine::Math::DataStructType dst, const BohgeEngine::Object& max );
		BohgeEngine::Attributes::ShaderAttributes _AttributesMapping( EmitterProperty::EmitterAffector aff );
		void _SettingVertexStream( EmitterProperty::EmitterAffector aff, BohgeEngine::Math::DataStructType dst );
		uint _CaculateEmitteCount( float span );
		void _SetAffectParameter( EmitterProperty::EmitterAffector aff, BohgeEngine::Math::DataStructType dst, const BohgeEngine::Volume& min, const BohgeEngine::Volume& max );//插入一个影响数据
	public:
		void DepenedSource( BohgeEngine::IAsyncSource* source );
		void CreateResource( BohgeEngine::IAsyncSource* host );
	public:
		void UpdataEmitter( float span );
	};
}