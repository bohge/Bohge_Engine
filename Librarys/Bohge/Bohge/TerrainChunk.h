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



/////////////////////////
//		地形四叉树   //
/////////////////////////
#pragma once
#include "SecenProperty.h"
#include "ISceneNode.h"
#include "aabsphere.hpp"
#include "Frustum.hpp"
#include "ShaderManage.h"
#include <vector>


namespace BohgeEngine
{
	class MaterialShader;
	class Engine;
	class Material;
	class RendBuffer;
	class IndicesBuffer;
	class TerrainChunk : public IAttributeNode
	{
	private:
		enum DETAIL_LEVEL //细节等级，每一个节点包含一个地精度，与他的父节点精度相同，和一个高精度，他本身该有的精度等级，用于当节点分裂的为4个后判断干节点用于哪一个精度等级
		{
			DL_HIGHT	= 0,
			DL_LOW		= 1,
			DL_COUNT,
		};
	private:
		struct DrawData//绘制的数据
		{
			int					m_DetailIndex;//细节的等级，8对应0，等等，用于平滑过渡
			//下面数值要传递给shader,保存多个内容，分别是
			vector4f					m_MorphXY_SteperScaleInvers_DoubleChunkSize;//变形系数, 当前的递增距离与缩放的乘积的逆，用于确定当前是第几个mesh等级使用，将顶点还原到0,1,2,3,4,5这种1递增方式,当前chunk的尺寸
			ShaderManage::ShaderName	m_RenderShader;//保存绘制时候的材质
			IndicesBuffer*				m_IndicesBuffer;////当前精度的三角形索引
			//数据在chunk中删除
		};
	private:
		typedef std::vector< DrawData >	DrawDataVector;//绘制buffer索引的数组
	private:
		vector2f			m_DetailDisance_Tile;//细节纹利可见的细节,细节纹理翻倍绑定系数

		const bool			m_isFirstNode;	//是否是首结点
		const bool			m_isLastNode;	//是否是终结点

		bool				m_isCanSee;//当摄像机不能进入水下的时候，这个参数来剔除那些无法被显示的地皮
		bool				m_isSplit;//是否需要向下递归
		bool				m_isDraw;//是否需要绘制该节点

		vector2f			m_BeginEnd; //此chunk细节的可见区域
		vector2f			m_OffsetScale; //此chunk偏移
		aabbox3df			m_BindBox;	//节点绑定盒

		vector2f			m_SizeScaleInv_RepeatSizeInv;//地形尺寸*缩放的倒数———重复的单元大小的倒数,用于确定纹理坐标
		int					m_DrawLevel;//绘制的细节等级，有可能次节点分裂了，但是其中一般节点不满足高精度，还用他的父节点的精度绘制
		DrawDataVector		m_DrawData;

		RendBuffer*			m_Buffer;
		Material*			m_Material;

		//子节点
		TerrainChunk*		m_LeafChunk_UL;  //上左
		TerrainChunk*		m_LeafChunk_UR;  //上右
		TerrainChunk*		m_LeafChunk_DL;  //下左
		TerrainChunk*		m_LeafChunk_DR;  //下右
	public:
		TerrainChunk( const vector2d& begin, const vector2d& end, int iterDepth, const TerrainProperty& ppy );//size的起始于结束,你的绑定盒,已经迭代的深度,原始的地形属性
		~TerrainChunk( );
	public:
		void LoadBindBox( Serializer& loader );
		void OffsetScale( const vector2f& os );
		void Update( const Frustumf& view, aabspheref& sphere, std::multimap< float, TerrainChunk* >& tmap );
		void Render( Engine& engine) const;
		void SetBuffer( RendBuffer* buffer );
		void SetMaterial( Material* m );
		void AssignMaterial( const bool& isCast, const ICamera& camera );//通过阴影摄像机确定渲染每一块的材质
	public:
		const void *Attribute(const IAttributeNode::Node_Attribute &att) const;
	};
}