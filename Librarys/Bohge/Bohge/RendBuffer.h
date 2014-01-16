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
//			绘制时Buffer		   //
/////////////////////////////////////

#pragma once
#include "Device.h"
#include "Vertex.h"
#include "IndicesBuffer.h"
#include "VertexBuffer.h"

namespace BohgeEngine
{


	//----------------------------------------------------------------------------------------------------------
	class RendBuffer //新的可绘制的buffer类型
	{
	private:
		Device::RenderMode	m_eMode;//绘制方式
		VertexBuffer*		m_pVertex;
		IndicesBuffer*		m_pIndices;
	public:
		explicit RendBuffer( Device::RenderMode mode );
		~RendBuffer();
	public:
		BOHGE_FORCEINLINE const VertexLayoutList* GetVertexLayoutList() const
		{
			return m_pVertex->GetVertexLayoutList();
		}
		BOHGE_FORCEINLINE void SetVertexBuffer( VertexBuffer* buffer )
		{
			m_pVertex = buffer;
		}
		BOHGE_FORCEINLINE void SetIndicesBuffer( IndicesBuffer* buffer )
		{
			m_pIndices = buffer;
		}
		BOHGE_FORCEINLINE VertexBuffer* GetVertexBuffer( )
		{
			return m_pVertex;
		}
		BOHGE_FORCEINLINE IndicesBuffer* GetIndicesBuffer( )
		{
			return m_pIndices;
		}
		BOHGE_FORCEINLINE void SetDrawCount(uint count)//设置可绘制的个数，降低绘制开销
		{
			if( NULL != m_pIndices)
			{
				return m_pIndices->SetDrawCount( count );//有索引和没有索引绘制不一样
			}
		}
		BOHGE_FORCEINLINE uint DrawCount() const
		{
			if( NULL == m_pIndices)
			{
				return m_pVertex->VertexCount();//有索引和没有索引绘制不一样
			}
			return m_pIndices->DrawCount();
		}
		BOHGE_FORCEINLINE uint VertexSize() const
		{
			return m_pVertex->VertexSize();
		}
		BOHGE_FORCEINLINE bool hasIndies() const
		{
			return NULL != m_pIndices;
		}
		BOHGE_FORCEINLINE Device::RenderMode RenderMode() const
		{
			return m_eMode;
		}
		BOHGE_FORCEINLINE void SetRenderMode( Device::RenderMode m )
		{
			m_eMode = m;
		}
		BOHGE_FORCEINLINE const handle& VertexHanle() const
		{
			return m_pVertex->VertexHanle();
		}
		BOHGE_FORCEINLINE const handle& IndexHandle() const
		{
			return m_pIndices->IndexHandle();
		}
	};

#define SHARED_MEMERY
#ifdef SHARED_MEMERY
#if defined(_DEBUG)
#define  SHAREDMEMERY_DEBUG
#endif
#endif // _DEBUG
	//----------------------------------------------------------------------------------------------------------
	//这是一个供享的vbo提取器，所以同一时间只能提取一次buffer，且当这个buffer被使用，废弃后才能继续提取
	class SharedBuffer//这是一个共享内存的buffer
	{
	private: //共享内存分配器
		class SharedMemery
		{
		private:
			static SharedMemery*		m_Selfptr;
#ifdef SHAREDMEMERY_DEBUG
			bool						m_isInstanceFlushing;//Instance是否增在填充
			bool						m_isFlushing;//是否增在填充
			int							m_VertexIndex;
			int							m_IndiesIndex;
			int							m_InstanIndex;
#endif
			std::vector<float>			m_VertexBuffer;
			std::vector<ushort>			m_IndiesBuffer;
			std::vector<float>			m_InstanBuffer;
		private:
			SharedMemery()
#ifdef SHAREDMEMERY_DEBUG
				:m_isFlushing(false),
				m_isInstanceFlushing(false),
				m_VertexIndex(0),
				m_IndiesIndex(0),
				m_InstanIndex(0)
#endif
			{

			}
		public:
			static BOHGE_FORCEINLINE void Creat()
			{
				m_Selfptr = NEW SharedMemery;
			}
			static BOHGE_FORCEINLINE void Destroy()
			{
				SAFE_DELETE( m_Selfptr );
			}
			static BOHGE_FORCEINLINE SharedMemery& Instance()
			{
				return *m_Selfptr;
			}
		public:
			BOHGE_FORCEINLINE void AllocVertexBuffer(int size)
			{
				if( m_VertexBuffer.size() < size )
				{
					m_VertexBuffer.resize(size);
				}
			}
			BOHGE_FORCEINLINE void AllocIndiesBuffer(int size)
			{
				if( m_IndiesBuffer.size() < size )
				{
					m_IndiesBuffer.resize(size);
				}
			}
			BOHGE_FORCEINLINE void AllocInstanBuffer(int size)
			{
				if( m_InstanBuffer.size() < size )
				{
					m_InstanBuffer.resize(size);
				}
			}
		public:
#ifdef SHAREDMEMERY_DEBUG
			BOHGE_FORCEINLINE void BeginInstanceFlush()
			{
				ASSERT( !m_isInstanceFlushing );//如果终端到这里说明是用这个shared buffer的地方在其他地方没有使用完buffer的时候就使用了buffer，嵌套情况/shared buffer是不能嵌套使用的
				m_isInstanceFlushing = true;
			}
			BOHGE_FORCEINLINE void EndInstanceFlush()
			{
				m_isInstanceFlushing = false;
			}
			BOHGE_FORCEINLINE void BeginFlush()
			{
				if (	0 == m_VertexIndex
					&&	0 == m_IndiesIndex
					&&	0 == m_InstanIndex 
					&&  m_isFlushing )
				{
					m_isFlushing = false;
				}
				ASSERT( !m_isFlushing );//如果终端到这里说明是用这个shared buffer的地方在其他地方没有使用完buffer的时候就使用了buffer，嵌套情况/shared buffer是不能嵌套使用的
				m_isFlushing = true;
				m_VertexIndex = 0;
				m_IndiesIndex = 0;
				m_InstanIndex = 0;
			}
			BOHGE_FORCEINLINE void EndFlush()
			{
				m_isFlushing = false;
			}
#endif
			BOHGE_FORCEINLINE float* VertexPtr(int index)
			{
#ifdef SHAREDMEMERY_DEBUG
				if( m_isFlushing )
				{
					ASSERT( index >= m_VertexIndex );
					m_VertexIndex = index;
				}
#endif // SHAREDMEMERY_DEBUG
				return &m_VertexBuffer[index];
			}
			BOHGE_FORCEINLINE ushort* IndiesPtr(int index)
			{
#ifdef SHAREDMEMERY_DEBUG
				if( m_isFlushing )
				{
					ASSERT( index >= m_IndiesIndex );
					m_IndiesIndex = index;
				}
#endif // SHAREDMEMERY_DEBUG
				return &m_IndiesBuffer[index];
			}
			BOHGE_FORCEINLINE float* InstancePtr(int index)
			{
#ifdef SHAREDMEMERY_DEBUG
				if( m_isFlushing )
				{
					ASSERT( index >= m_InstanIndex );
					m_InstanIndex = index;
				}
#endif // SHAREDMEMERY_DEBUG
				return &m_InstanBuffer[index];
			}
		};

	private:
		uint					m_CountNode;//node个数
		uint					m_CountFloat;//每个vertex的float个数
		uint					m_SizeOfVertex;//每个顶点的bite大小
		uint					m_CountVertexPerNode;//每个node的vertex的数量
		uint					m_CountIndiesPerNode;//每个node的Indies的数量

		uint					m_FloatIndex;	//顶点数据
#ifndef SHARED_MEMERY
		std::vector<float>		m_Buffer;
#endif

		uint					m_IndiesIndex;
		uint					m_IndiesOffset;//连续读取不同的Node，需要这个offset
#ifndef SHARED_MEMERY
		std::vector<ushort>		m_vIndies;
#endif
		bool					m_isInstance;	//是否使用实例化
		uint					m_NodeIndex;//拷贝的第几个Node
#ifndef SHARED_MEMERY
		std::vector<float>		m_Instancebuffer;
#endif
	public:
		SharedBuffer(uint countfloat, uint countVertex, uint countIndies, uint countNode);//输入每个顶点的float个数，每个node的顶点个数，所以个数，node个数
		~SharedBuffer();
		void AddVertex(const void* vertexPtr, const ushort* indiesPtr );
		void CleanUp();//必须在每次填充之前调用
		void UseInstance();
		void AddMatrix( Matrix44f& mat);
		void CleanUpInstance();
	public:
		static BOHGE_FORCEINLINE void CreatSharedMemeryInstance()
		{
			SharedMemery::Creat();
		}
		static BOHGE_FORCEINLINE void DestorySharedMemeryInstance()
		{
			SharedMemery::Destroy();
		}
		BOHGE_FORCEINLINE void* VertexPtr()
		{
#ifdef SHAREDMEMERY_DEBUG
			SharedMemery::Instance().EndFlush();
#endif // SHAREDMEMERY_DEBUG
#ifdef SHARED_MEMERY
			return SharedMemery::Instance().VertexPtr(0);
#else
			return &m_Buffer[0];
#endif // SHARED_MEMERY
		}
		BOHGE_FORCEINLINE uint VertexSize()
		{
			return m_FloatIndex / m_CountFloat;
		}
		BOHGE_FORCEINLINE ushort* IndiesPtr()
		{
#ifdef SHAREDMEMERY_DEBUG
			SharedMemery::Instance().EndFlush();
#endif // SHAREDMEMERY_DEBUG
#ifdef SHARED_MEMERY
			return SharedMemery::Instance().IndiesPtr(0);
#else
			return &m_vIndies[0];
#endif // SHARED_MEMERY		
		}
		BOHGE_FORCEINLINE uint IndiesSize()//现有的索引尺寸
		{
			return m_IndiesIndex;
		}
		BOHGE_FORCEINLINE void* InstancePtr()
		{
#ifdef SHAREDMEMERY_DEBUG
			SharedMemery::Instance().EndInstanceFlush();
#endif // SHAREDMEMERY_DEBUG
#ifdef SHARED_MEMERY
			return SharedMemery::Instance().InstancePtr(0);
#else
			return &m_Instancebuffer[0];
#endif // SHARED_MEMERY
		}
		BOHGE_FORCEINLINE uint InstanceSize()
		{
			return m_NodeIndex * m_CountVertexPerNode;
		}
		BOHGE_FORCEINLINE uint CountOfAllIndies()//全部Indies的大小
		{
			return m_CountIndiesPerNode * m_CountNode;
		}
		BOHGE_FORCEINLINE uint BiteOfPerVertex()//每个顶点的bite数
		{
			return m_SizeOfVertex;
		}
		BOHGE_FORCEINLINE uint CountOfNode()//node的总数
		{
			return m_CountNode;
		}
		BOHGE_FORCEINLINE uint CountofVertexPerNode()//每个node的顶点数
		{
			return m_CountVertexPerNode;
		}
		BOHGE_FORCEINLINE uint CountOfAllVertex()//全部顶点个数
		{
			return m_CountVertexPerNode * m_CountNode;
		}
	};





	//----------------------------------------------------------------------------------------------------------
	//class VertexBuffer
	//{
	//private:
	//	enum InternalType
	//	{
	//		INTERNAL_VERTEX				= 1 << 0,
	//		INTERNAL_COLOR0				= 1 << 1,
	//		//INTERNAL_COLOR1				= 1 << 2,
	//		INTERNAL_COORDINATE0		= 1 << 3,
	//		INTERNAL_COORDINATE1		= 1 << 4,
	//		INTERNAL_NORMAL				= 1 << 5,
	//	};
	//public:
	//	enum Type // buffer 的类型
	//	{
	//		VERTEX						= INTERNAL_VERTEX,
	//		VERTEX_COLOR				= INTERNAL_VERTEX | INTERNAL_COLOR0,
	//		VERTEX_CROODINATE			= INTERNAL_VERTEX | INTERNAL_COORDINATE0,
	//		VERTEX_NORMAL				= INTERNAL_VERTEX | INTERNAL_NORMAL,
	//		VERTEX_CROODINATE_COLOR		= INTERNAL_VERTEX | INTERNAL_COORDINATE0 | INTERNAL_COLOR0,
	//		VERTEX_CROODINATE_NORMAL	= INTERNAL_VERTEX | INTERNAL_COORDINATE0 | INTERNAL_NORMAL,
	//		VERTEX_CROODINATEx2_NORMAL	= INTERNAL_VERTEX | INTERNAL_COORDINATE0 | INTERNAL_COORDINATE1 | INTERNAL_NORMAL,
	//	};
	//private:
	//	enum
	//	{
	//		CountPerVertex = 4,
	//		CountPerColor = 4,
	//		CountPerCoordinate = 2,
	//		CountPerNormal = 3,
	//	};
	//private:
	//	uint						m_uMaxNodeCount;//可读入的该种图源得罪大个数
	//	uint						m_nVertexCountPerNode;//每个节点含有的顶点的数目
	//	uint						m_nIndiesCountPerNode;//每个节点还有的索引的数目
	//
	//	uint						m_nMaxCount;//最大可读入顶点
	//	uint						m_nCount;//已读入顶点数目
	//	uint						m_nCountOfIndies;//已经读入的索引数目
	//
	//	Device::RenderMode			m_eRenderMode;//绘制方式
	//	uint						m_uType;//buffer类型的二进制数值
	//
	//
	//	//各种VBO handle
	//	bool						m_isFlushed;//是否为第一次填充
	//	Device::Useage				m_eBufferUsage;
	//	handle						m_hVertexBuffer;
	//	handle						m_hColorBuffer;
	//	handle*						m_hCoordinateBufferArray;
	//	handle						m_hNormalBuffer;
	//	handle						m_hIndexBuffer;
	//
	//	FloatVector*				m_vVertex;
	//	FloatVector*				m_vColor;
	//	uint						m_nCountCoordinateVector;//有几层纹理
	//	FloatVector**				m_pvCoordinate;	//复合纹理坐标
	//	FloatVector*				m_vNormal;
	//	IntVector*					m_vIndies;
	//private:
	//	void UsingBuffer(uint type);
	//public:
	//	VertexBuffer(Device::RenderMode Mode, Device::Useage use, Type type, uint countVertex, uint countIndies, uint countNode);
	//	VertexBuffer( const VertexBuffer& input);
	//	~VertexBuffer();
	//	void CleanUp();
	//	void AddVertex(const Vertex* const vertexArray, const uint Indieses[]);
	//	void AddVertex(const VertexColor* const vertexArray[], const uint Indieses[]);
	//	void AddVertex(const VertexTexture* const vertexArray[], const uint Indieses[]);
	//	void AddVertex(const VertexTextureNormal* const vertexArray[], const uint Indieses[]);
	//	void AddVertex(const VertexColor vertexArray[], const uint Indieses[]);
	//	void AddVertex(const VertexTexture vertexArray[], const uint Indieses[]);
	//	void AddVertex(const VertexTextureNormal vertexArray[], const uint Indieses[]);
	//	void AddVertex(const VertexTextureColor vertexArray[], const uint Indieses[]);
	//	void UsingBuffer();
	//	void UsingBuffer(Type type);
	//	void DropBuffer();
	//	void FlushBuffer();//重新填充buffer
	//private:
	//	BOHGE_FORCEINLINE bool isType(uint type1, InternalType type2)
	//	{
	//		if( 0 != ( type1 & static_cast<uint>(type2) ) )
	//		{
	//			return true;
	//		}
	//		return false;
	//	}
	//	BOHGE_FORCEINLINE bool isIncludeType(Type type)//需求的属性类型是否在此buffer中被填充
	//	{
	//		uint input = static_cast<uint>(type);
	//		if( input == ( m_uType & input ) )
	//		{
	//			return true;
	//		}
	//		return false;
	//	}
	//	BOHGE_FORCEINLINE void AddPostion(const vector4f& vertex)
	//	{
	//		m_vVertex->push_back(vertex.m_x);
	//		m_vVertex->push_back(vertex.m_y);
	//		m_vVertex->push_back(vertex.m_z);
	//		m_vVertex->push_back(vertex.m_w);
	//	}
	//	BOHGE_FORCEINLINE void AddColor(const Color& color)
	//	{
	//		m_vColor->push_back(color.m_r);
	//		m_vColor->push_back(color.m_g);
	//		m_vColor->push_back(color.m_b);
	//		m_vColor->push_back(color.m_a);
	//	}
	//	BOHGE_FORCEINLINE void AddCoordinate(uint index, const vector2f& coordinate)
	//	{
	//		m_pvCoordinate[index]->push_back(coordinate.m_u);
	//		m_pvCoordinate[index]->push_back(coordinate.m_v);
	//	}
	//	BOHGE_FORCEINLINE void AddNormal(const vector3f& normal)
	//	{
	//		m_vNormal->push_back(normal.m_x);
	//		m_vNormal->push_back(normal.m_y);
	//		m_vNormal->push_back(normal.m_z);
	//	}
	//public:
	//	BOHGE_FORCEINLINE bool isEmpty()
	//	{
	//		return m_nCount <= 0;
	//	}
	//	BOHGE_FORCEINLINE Device::RenderMode GetType()
	//	{
	//		return m_eRenderMode;
	//	}
	//	BOHGE_FORCEINLINE int GetIndiseCount()
	//	{
	//		return m_nCountOfIndies;
	//	}
	//	BOHGE_FORCEINLINE int* GetIndies()
	//	{
	//		return m_vIndies->GetDataArray();
	//	}
	//	BOHGE_FORCEINLINE handle IndiesBuffer()
	//	{
	//		return m_hIndexBuffer;
	//	}
	//};

}