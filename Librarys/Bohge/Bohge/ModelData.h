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
//		模型基础数据	   //
/////////////////////////////
#pragma once
#include "3DMath.h"
#include "aabbox.hpp"

#include <string>
#include <vector>


namespace BohgeEngine
{
	class RendBuffer;
	class Material;
	class Serializer;

	typedef std::pair<std::vector<Quaternionf>, std::vector<Quaternionf> > AnimationDual;//最后要传入shader的数据

	struct Vertex_Attrs //读取模型时候，那些顶点有哪些没有
	{
		int Attrs;
		bool vertex, normal, tangent, binormal, tex, bone;
		Vertex_Attrs():vertex(false),normal(false),tangent(false),binormal(false),tex(false),bone(false),Attrs(0){}
	};



	class SkinningKeyFrame	//模型动画类
	{
		friend class Model;
	private:
		struct KeyFrame
		{
			std::string											m_Name;//对应骨骼名称
			std::vector< std::pair<Quaternionf, Quaternionf> >	m_QuaternionArray;//关键帧队列
		private:
			BOHGE_FORCEINLINE void _DoCopy( const KeyFrame& rhs )
			{
				m_Name = rhs.m_Name;
				m_QuaternionArray.resize( rhs.m_QuaternionArray.size() );
				for ( int i = 0 ; i < rhs.m_QuaternionArray.size(); i ++ )
				{
					m_QuaternionArray[i].first = rhs.m_QuaternionArray[i].first;
					m_QuaternionArray[i].second = rhs.m_QuaternionArray[i].second;
				}
			}
		public:
			KeyFrame()
			{

			}
			KeyFrame( const KeyFrame& rhs )
			{
				_DoCopy(rhs);
			}
			KeyFrame& operator = ( const KeyFrame& rhs )
			{
				_DoCopy(rhs);
				return *this;
			}
		};
	private://读入的数据
		int							m_FrameBging;	//开始帧
		int							m_FrameEnd;		//结束帧
		int							m_Rate;			//帧率
		std::vector<KeyFrame>		m_BoneKeyFrames;//有关键真的骨骼
	private:
		bool						m_isPlayAnimation;//是否播放动画
		bool						m_isLoop;//是否循环播放
		int							m_PlayBegin;	//需要播放的动画区间
		int							m_PlayEnd;	//需要播放的动画区间
		float						m_LastFrame;//前一帧
		float						m_CurrentFrame;//当前帧
	private:
		BOHGE_FORCEINLINE void _DoCopy( const SkinningKeyFrame& rhs )
		{
			m_FrameBging = rhs.m_FrameBging;
			m_FrameEnd = rhs.m_FrameEnd;
			m_Rate = rhs.m_Rate;
			m_isPlayAnimation = rhs.m_isPlayAnimation;
			m_isLoop = rhs.m_isLoop;
			m_PlayBegin = rhs.m_PlayBegin;
			m_PlayEnd = rhs.m_PlayEnd;
			m_LastFrame = rhs.m_LastFrame;
			m_CurrentFrame = rhs.m_CurrentFrame;
			m_BoneKeyFrames.resize( rhs.m_BoneKeyFrames.size() );
			for ( int i = 0 ; i < rhs.m_BoneKeyFrames.size(); i ++ )
			{
				m_BoneKeyFrames[i] = rhs.m_BoneKeyFrames[i];
			}
		}
	public:
		SkinningKeyFrame( const SkinningKeyFrame& rhs )
		{
			_DoCopy(rhs);
		}
		SkinningKeyFrame& operator = ( const SkinningKeyFrame& rhs )
		{
			_DoCopy(rhs);
			return *this;
		}
	private:
		BOHGE_FORCEINLINE bool isPlay()
		{
			return m_isPlayAnimation;
		}
	public:
		SkinningKeyFrame();
		void LoadKeyFrame(Serializer& file);//读入关键桢
		std::pair<Quaternionf, Quaternionf> GetFrame(int jointIndex);
		int	GetIndex(const std::string& jointName);
		void SetTime(uint timespan);
		std::vector< std::string > GetJointNameList();
	public:
		BOHGE_FORCEINLINE void SetAnimationInterval( int begin, int end)//设置需要播放的帧区间
		{
			ASSERT( begin >= m_FrameBging );
			ASSERT( end <= m_FrameEnd );
			m_PlayBegin = begin;
			m_PlayEnd = end;
		}
		BOHGE_FORCEINLINE void ResetAnimation()
		{
			m_LastFrame = m_CurrentFrame;
			m_CurrentFrame = m_PlayBegin;
		}
		BOHGE_FORCEINLINE void Play()
		{
			m_isPlayAnimation = true;
		}
		BOHGE_FORCEINLINE void Stop()
		{
			m_isPlayAnimation = false;
		}
		BOHGE_FORCEINLINE void Loop(bool b)
		{
			m_isLoop = b;
		}
	};


	class VertexBuffer;
	class IndicesBuffer;
	struct RendMesh
	{
		bool			m_isCopyed;
		mutable bool	m_isRender;//测试是否在摄像机中，来判断是否绘制
		std::string		m_Name;
		aabbox3df		m_BindBox;//包围盒
		RendBuffer*		m_pBuffer;
		VertexBuffer*	m_pVertexBuffer;
		IndicesBuffer*	m_pIndicesBuffer;
		int				m_Materialindex;//材质的索引
		Material*		m_pMaterial;//索引指向材质的指针
	public:
		RendMesh():m_isCopyed(false),m_isRender(true),m_pBuffer(NULL),m_pMaterial(NULL),m_pVertexBuffer(NULL),m_pIndicesBuffer(NULL){}
		~RendMesh();
		RendMesh( const RendMesh& rm )
		{
			_DoCopy(rm);
		}
		const RendMesh& operator = ( const RendMesh& rm )
		{
			_DoCopy(rm);
			return *this;
		}
		BOHGE_FORCEINLINE void SetRend( const bool& b ) const
		{
			m_isRender = b;
		}
	private:
		void _DoCopy( const RendMesh& rm )
		{
			m_isCopyed = true;
			m_isRender = true;
			m_Name = rm.m_Name;
			m_BindBox = rm.m_BindBox;
			m_pBuffer = rm.m_pBuffer;
			m_pVertexBuffer = rm.m_pVertexBuffer;
			m_pIndicesBuffer = rm.m_pIndicesBuffer;
			m_Materialindex = rm.m_Materialindex;
			m_pMaterial = rm.m_pMaterial;
		}
	};



	struct Joint//骨骼信息
	{
		std::string								m_Name; //骨骼名称
		int										m_Parent;//骨骼的父
		int										m_KeyFrameIndex;//骨骼在关键帧动画中的索引
		Quaternionf								m_RealBind;
		Quaternionf								m_DaulBind;
		std::pair<Quaternionf, Quaternionf>		m_InversPair;//骨骼初始状态的逆
	private:
		BOHGE_FORCEINLINE void _DoCopy( const Joint& rhs )
		{
			//memcpy( this, &rhs, sizeof( Joint ) );//不能用会崩溃，字符串析沟，这里的std字符串不是实际复制构造的，共用了一个内存
			m_Name = rhs.m_Name;
			m_Parent = rhs.m_Parent;
			m_KeyFrameIndex = rhs.m_KeyFrameIndex;
			m_RealBind = rhs.m_RealBind;
			m_DaulBind = rhs.m_DaulBind;
			m_InversPair.first = rhs.m_InversPair.first;
			m_InversPair.second = rhs.m_InversPair.second;
		}
	public:
		Joint()
		{

		}
		Joint( const Joint& rhs )
		{
			_DoCopy(rhs);
		}
		Joint& operator = ( const Joint& rhs )
		{
			_DoCopy(rhs);
			return *this;
		}
	};




	class ModelData //资源管理器加载的模型数据
	{
		friend class Model;
	public:
		//enum中的起始数字不能修改，是输出需求的
		enum Model_Chunk	//包含的数据体这个要和模型导出的索引匹配
		{
			CHUNK_BONES = 0,//模型骨骼
			CHUNK_MATERIALS,//模型材质
			CHUNK_KEYFRAMES,//模型关键帧
			//添加新的chunk属性
			CHUNK_MESHES = 501,//模型网格
			MESHES_DATA,//网格数据
			MESHES_INDEX,//模型索引
			//添加新的mesh数据属性
			MESHES_BINDBOX,//网格的包围盒
			MESHES_END = 999,//网格结束
			MODEL_END = 1000,//模型读取完毕
		};
		enum Vertex_Elements //每个顶点包含的数据资料这个要和模型导出的索引匹配
		{
			VE_VERTEX = 1001,//顶点
			VE_NORMAL,//法线
			VE_BINORMAL,//副法线
			VE_TANGENT,//切线
			VE_TEX_COORD,//纹理坐标
			VE_BONEWEIGHT,//骨骼信息
			VE_END = 2000,//结束
		};
	private:
		//c++的默认的拷贝构造，与赋值=，是将指针全部赋值，即，多个类共用
		std::string					m_FileName;
		bool						m_isBone;//是否有骨骼
		uint						m_MaxBoneIndexCount;//最大索引数目，用于确定调用什么样子的shader
		std::vector<Material*>		m_MaterialVector;//这个值是全部同名模型公用的，目前觉得可以接受
		std::vector<RendMesh*>		m_RendMeshVector;//这个值是全部同名模型公用的，目前觉得可以接受
		std::vector<Joint>			m_JointVector;
		SkinningKeyFrame			m_KeyFrame;
		aabbox3df					m_BindBox;
	public:
		explicit ModelData();
		~ModelData();
	private:
		void _LoadBones(Serializer& file);
		void _LoadMaterials(Serializer& file, const std::string& folderpath);
		void _LoadMeshesData(Serializer& file, const Vertex_Attrs& att, vector2f& tex, vector3f& pos, vector3f& nor, vector3f& bin, vector4f& tat, vector4f& bone, vector4f& weight );
		void _LoadMeshes(Serializer& file);
		void _LoadKeyFrame(Serializer& file);
	public:
		void LoadModel(const std::string& name);
	};

}