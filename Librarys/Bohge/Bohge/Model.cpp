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



/////////////////////////////
//			模型		   //
/////////////////////////////
#include "Model.h"
#include "Serializer.h"
#include "3DMath.h"
#include "Texture.h"
#include "Material.h"
#include "Vertex.h"
#include "RendBuffer.h"
#include "Log.h"
#include "ResourceManager.h"
#include "Camera.h"
#include "Utility.h"


#include <vector>

using namespace std;

namespace BohgeEngine
{
	//--------------------------------------------------------------------------------------------------------------------------------------
	Model::Model( )
		:m_isBone(false),
		m_isLoaded(false),
		m_isInview(false),
		m_RefreshLimit(1000),
		m_PassedTime(0),
		m_isUsingBasiMatrixOnDrawDepth(false)
	{
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	Model::~Model(void)
	{
		Engine::Instance().GetResourceManage()->ReleaseModel( m_FileName );
		for ( JointNode::iterator it = m_JointNodes.begin();
			it != m_JointNodes.end();
			it++ )
		{
			SAFE_DELETE( it->second );
		}
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	void Model::LoadModel( const std::string& name )
	{
		if ( m_isLoaded )
		{
			Engine::Instance().GetResourceManage()->ReleaseModel( m_FileName );
		}
		m_isLoaded = true;
		const ModelData& data = Engine::Instance().GetResourceManage()->LoadModel( name );
		m_FileName = name;
		m_isBone = data.m_isBone;
		m_MaxBoneIndexCount = data.m_MaxBoneIndexCount;
		m_MaterialVector.resize( data.m_MaterialVector.size() );
		for ( int i = 0 ; i < data.m_MaterialVector.size(); i ++ )
		{
			m_MaterialVector[i] = *data.m_MaterialVector[i];
		}
		m_RendMeshVector.resize( data.m_RendMeshVector.size() );
		for ( int i = 0 ; i < data.m_RendMeshVector.size(); i ++ )
		{
			m_RendMeshVector[i] = *data.m_RendMeshVector[i];
			m_RendMeshVector[i].m_pMaterial = &m_MaterialVector[m_RendMeshVector[i].m_Materialindex];
		}
		m_JointVector = data.m_JointVector;
		m_KeyFrame = data.m_KeyFrame;
		m_BindBox = data.m_BindBox;
		m_OgiBindBox = m_BindBox;
		m_AnimationDual.first.resize( m_JointVector.size() );
		m_AnimationDual.second.resize( m_JointVector.size() );
		if( m_isBone )
		{
			this->_BuildBones(0);
		}
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	void Model::_BuildBones(uint timespan)
	{
		m_KeyFrame.SetTime(timespan);
		for ( int i = 0 ; i < m_JointVector.size(); i++ )
		{
			std::pair<Quaternionf, Quaternionf> kf;
			kf = m_KeyFrame.GetFrame( m_JointVector[i].m_KeyFrameIndex );
			int parent =   m_JointVector[i].m_Parent;
			if( -1 != parent )
			{
				if( kf.first.Dot( m_JointVector[ parent ].m_RealBind ) < 0 )
				{
					kf.first = -kf.first;
					kf.second = -kf.second;
				}
				m_JointVector[i].m_RealBind = kf.first * m_JointVector[parent].m_RealBind;
				m_JointVector[i].m_DaulBind = kf.first * m_JointVector[parent].m_DaulBind + kf.second * m_JointVector[parent].m_RealBind;
			}
			else
			{
				m_JointVector[i].m_RealBind = kf.first;
				m_JointVector[i].m_DaulBind = kf.second;
			}
		}
		for ( int i = 0 ; i < m_JointVector.size(); i++ )//乘最原始的逆矩阵,构造渲染用四元数
		{
			m_AnimationDual.first[i] = m_JointVector[i].m_InversPair.first * m_JointVector[i].m_RealBind;
			m_AnimationDual.second[i] = m_JointVector[i].m_InversPair.first * m_JointVector[i].m_DaulBind + m_JointVector[i].m_InversPair.second * m_JointVector[i].m_RealBind;
		}
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	void Model::Render( Engine& engine )
	{
		//if( 0 < m_JointVector.size() )//采用自适应shader，不需要再绘制的时候判断使用什么shader了，移到其它地方去做
		//{
		for (int i = 0 ; i < m_RendMeshVector.size(); i ++ )
		{
			//if ( NULL != m_RendMeshVector[i]->m_pMaterial->Attribute(Material::ATT_TEX_DIFFUSE) )
			{
				if ( m_RendMeshVector[i].m_isRender )
				{
					if( NULL != this->Attribute( IAttributeNode::ATT_WORLDTRANSFORM ) )
					{
						if( IntersectedType::IT_NO != engine.GetCamera()->GetFrustum().Intersect( m_RendMeshVector[i].m_BindBox * *(Matrix44f*)(this->Attribute( IAttributeNode::ATT_WORLDTRANSFORM )) ) )
						{
							m_RendMeshVector[i].m_pMaterial->SetNode( (IAttributeNode*)this );
							engine.GetDevice()->Draw(*m_RendMeshVector[i].m_pBuffer,*m_RendMeshVector[i].m_pMaterial);
						}
					}
					else
					{
						if( m_RendMeshVector[i].m_isRender
							&& IntersectedType::IT_NO != engine.GetCamera()->GetFrustum().Intersect( m_RendMeshVector[i].m_BindBox ) )
						{
							m_RendMeshVector[i].m_pMaterial->SetNode( (IAttributeNode*)this );
							engine.GetDevice()->Draw(*m_RendMeshVector[i].m_pBuffer,*m_RendMeshVector[i].m_pMaterial);
						}
					}
				}
			}
		}
		//}
		//else
		//{
		//	for (int i = 0 ; i < m_RendMeshVector.size(); i ++ )
		//	{
		//		PrePixelLightTransform& plst = engine.GetShaderManage()->GetShader<PrePixelLightTransform>(ShaderManage::PrePixelLightTransform);
		//		plst.SetParamTransform( m_Transform );
		//		engine.GetDevice()->Draw(*m_RendMeshVector[i]->m_pBuffer, plst, m_RendMeshVector[i]->m_pMaterial->GetTextur(Material::ATT_TEX_DIFFUSE) );
		//	}
		//}
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	const void* Model::Attribute(const Node_Attribute& name) const
	{
		switch( name )
		{
		case IAttributeNode::ATT_USER_0: return m_isUsingBasiMatrixOnDrawDepth ? static_cast<const void*>(&m_isUsingBasiMatrixOnDrawDepth) : NULL;
		case  IAttributeNode::ATT_ANIMATION_REAL : return static_cast<const void*>(&m_AnimationDual.first);
		case  IAttributeNode::ATT_ANIMATION_DUAL : return static_cast<const void*>(&m_AnimationDual.second);
		}
		//ASSERT(false);
		return NULL;
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	void Model::_UpDataJoint()
	{
		//计算子节点新位置
		for ( JointNode::iterator it = m_JointNodes.begin();
			it != m_JointNodes.end();
			)
		{
			if ( 0 == it->second->SonCount() )//没有子节点了，删除自己
			{
				DetachNode( it->second );
				SAFE_DELETE( it->second );
				JointNode::iterator temp = it++;
				m_JointNodes.erase( temp );
			}
			else//更新矩阵
			{
				Quaternionf real = m_JointVector[ it->second->m_JointIndex ].m_RealBind;
				Quaternionf dual = m_JointVector[ it->second->m_JointIndex ].m_DaulBind;

				const void* data;
				data = it->second->FatherAttribute( IAttributeNode::ATT_WORLDSCALE );
				it->second->m_Scale = NULL == data ? vector3f(1,1,1) :  *((const vector3f*)data);

				data = it->second->FatherAttribute( IAttributeNode::ATT_WORLDTRANSFORM );
				it->second->m_Transfrom = NULL == data ? Math::DualQuaternionToMatrix44( real, dual ) : Math::DualQuaternionToMatrix44( real, dual ) * *((const Matrix44f*)data);

				it->second->m_Position = vector3f( it->second->m_Transfrom.a41, it->second->m_Transfrom.a42, it->second->m_Transfrom.a43 );

				it->second->m_Rotation = it->second->m_Transfrom.ToQuaternion();

				it->second->m_RotationMatrix = Matrix33f( it->second->m_Transfrom );

				it->second->CastChangedMessage();//通知子节点
				it ++;
			}
		}
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	void Model::Update( uint milisecon )
	{
		if( m_isBone && m_KeyFrame.isPlay() )
		{
			m_PassedTime += milisecon;
			m_isInview = IntersectedType::IT_NO != Engine::Instance().GetCamera()->GetFrustum().Intersect( m_BindBox );
			if ( _isInview() && m_PassedTime >= m_RefreshLimit )
			{
				//DEBUGLOG("Model reflash limit %d / second\n", 1000 / m_RefreshLimit );
				this->_BuildBones(m_PassedTime);
				m_PassedTime = 0;//m_PassedTime - m_RefreshLimit;
				_UpDataJoint();
			}
		}
		else if ( isFatherChange()
				|| m_isFroceUpdate )//父节点改变，改变包围盒和joint
		{
			_UpDataJoint();
		}
		m_isFroceUpdate = false;
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	bool Model::isInview( const Frustumf& cFru )
	{
		return IntersectedType::IT_NO != cFru.Intersect( m_BindBox );;
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	IAttributeNode* Model::GetNodeByIndex( int index )
	{
		if ( index <= 0)//0视作是根节点
		{
			return this;
		}
		index-=1;
		JointNode::iterator it = m_JointNodes.find( index );
		if ( m_JointNodes.end() == it )//没找到
		{
			JointObject* jo = NEW JointObject;
			AttachNode( jo );//挂在根节点上
			jo->m_JointIndex = index;
			m_JointNodes.insert( std::make_pair( index, jo ) );
			SetFatherChanged();
			return jo;
		}
		else
		{
			return it->second;
		}
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	void Model::LoadLightMap( int index, const std::string& path )
	{
		Material& m = m_MaterialVector[index];
		Texture2D* tex = m.LoadTexture( Material::ATT_TEX_LIGHT_MAP, path );
		tex->SetFilter( Device::TF_LINEAR, Device::TF_LINEAR );
		if( m_isBone ) //如果是有动画的
		{
			//现在都在初始阶段
			switch( m_MaxBoneIndexCount )
			{
			case 1:m.SetShader( Pipeline::PT_LIGHTING, ShaderManage::ModelLightMaped );break;
			case 2:m.SetShader( Pipeline::PT_LIGHTING, ShaderManage::ModelLightMaped );break;
			case 3:m.SetShader( Pipeline::PT_LIGHTING, ShaderManage::ModelLightMaped );break;
			case 4:m.SetShader( Pipeline::PT_LIGHTING, ShaderManage::ModelLightMaped );break;
			default: ASSERT(false);
			}
		}
		else
		{
			m.SetShader( Pipeline::PT_LIGHTING, ShaderManage::ModelLightMaped );
		}
	}






	/////////////////////////////
	//		可以定位的模型	   //
	/////////////////////////////
	//--------------------------------------------------------------------------------------------------------------------------------------
	LocatableModel::LocatableModel(  )
		:LocatableObject( this ),
		Model( )
	{

	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	void LocatableModel::Update( uint milisecon )
	{
		bool islocatablechange = isChange();
		LocatableObject::Update(milisecon);
		_SetForceUpdataJoint( islocatablechange );

		if ( _isBone() && _isInview() )
		{
			vector3f disdir = GetWorldPosition() - Engine::Instance().GetCamera()->GetPosition();
			vector3f boxdiag = ( m_BindBox.GetMax() - m_BindBox.GetMin() );
			float dispow = disdir.Dot( disdir );
			float boxsize = boxdiag.Dot( boxdiag );
			_SetRefreshLimit( Utility::CaculateRefreshLimit(dispow, boxsize) );
		}
		Model::Update( milisecon );
		if ( islocatablechange 
			|| isFatherChange() )
		{
			Matrix44f* trans = (Matrix44f*)(this->Attribute( IAttributeNode::ATT_WORLDTRANSFORM ));
			m_BindBox =  NULL == trans ? m_OgiBindBox : m_OgiBindBox * *trans;
		}
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	const void* LocatableModel::Attribute(const Node_Attribute& name) const
	{
		switch( name )
		{
		case IAttributeNode::ATT_USER_0:
		case IAttributeNode::ATT_ANIMATION_REAL:
		case IAttributeNode::ATT_ANIMATION_DUAL:
			{
				return Model::Attribute( name );
			}
		default: return LocatableAttribute( name );
		}
	}







	/////////////////////////////////////
	//		可以定位的物理模型	   //
	/////////////////////////////////////
	//--------------------------------------------------------------------------------------------------------------------------------------
	PhysicalModel::PhysicalModel( )
		:PhysicalObject(this),
		Model( )
	{

	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	void PhysicalModel::InitPhysical( float mass, const vector3f& localInertia )
	{
		this->_InitPhysical( m_BindBox, mass, localInertia );
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	void PhysicalModel::Update( uint milisecon )
	{
		PhysicalObject::Update(milisecon);
		Model::Update( milisecon );
		Matrix44f* trans = (Matrix44f*)(this->Attribute( IAttributeNode::ATT_WORLDTRANSFORM ));
		m_BindBox =  NULL == trans ? m_OgiBindBox : m_OgiBindBox * *trans;
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	const void* PhysicalModel::Attribute(const Node_Attribute& name) const
	{
		switch( name )
		{
		case IAttributeNode::ATT_USER_0:
		case IAttributeNode::ATT_ANIMATION_REAL:
		case IAttributeNode::ATT_ANIMATION_DUAL:
			{
				return Model::Attribute( name );
			}
		default: return LocatableAttribute( name );
		}
	}
}