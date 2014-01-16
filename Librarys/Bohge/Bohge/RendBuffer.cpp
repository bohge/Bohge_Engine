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


#include "RendBuffer.h"
#include "Engine.h"

namespace BohgeEngine
{




	/////////////////////////////
	//			RendBuffer	   //
	/////////////////////////////
	//-------------------------------------------------------------------
	RendBuffer::RendBuffer( Device::RenderMode mode )
		:m_eMode(mode),
		m_pVertex(NULL),
		m_pIndices(NULL)
	{

	}
	//-------------------------------------------------------------------
	RendBuffer::~RendBuffer()
	{
	}





	SharedBuffer::SharedMemery*	SharedBuffer::SharedMemery::m_Selfptr = NULL;

	/////////////////////////////////
	//			SharedBuffer	   //
	/////////////////////////////////
	//--------------------------------------------------------------------------------------------------------------------------------------
	SharedBuffer::SharedBuffer(uint countfloat, uint countVertex, uint countIndies, uint countNode)
		:m_CountVertexPerNode(countVertex),
		m_CountIndiesPerNode(countIndies),
		m_CountNode(countNode),
		m_CountFloat(countfloat),
		m_SizeOfVertex(sizeof(float)*m_CountFloat),
		m_FloatIndex(0),
		m_IndiesIndex(0),
		m_IndiesOffset(0),
		m_isInstance(false),
		m_NodeIndex(0)
	{
#ifdef SHARED_MEMERY
		SharedMemery::Instance().AllocVertexBuffer(m_CountFloat * m_CountVertexPerNode * m_CountNode);
		SharedMemery::Instance().AllocIndiesBuffer(m_CountIndiesPerNode * m_CountNode);
#else
		m_Buffer.resize(m_CountFloat * m_CountVertexPerNode * m_CountNode);
		m_vIndies.resize(m_CountIndiesPerNode * m_CountNode);
#endif // SHARED_MEMERY
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	SharedBuffer::~SharedBuffer()
	{
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	void SharedBuffer::AddVertex(const void* vertexPtr, const ushort* indiesPtr )
	{
		for( uint i = 0; i < m_CountIndiesPerNode; i ++ )
		{
#ifdef SHARED_MEMERY
			*SharedMemery::Instance().IndiesPtr(m_IndiesIndex) = indiesPtr[i] + m_IndiesOffset;
#else
			m_vIndies[m_IndiesIndex] = indiesPtr[i] + m_IndiesOffset;
#endif // SHARED_MEMERY
			m_IndiesIndex++;
		}
		m_IndiesOffset += m_CountVertexPerNode;
		for( uint i = 0 ; i < m_CountVertexPerNode; i ++ )
		{
#ifdef SHARED_MEMERY
			memcpy( SharedMemery::Instance().VertexPtr(m_FloatIndex), static_cast<const char*>(vertexPtr) + m_SizeOfVertex * i, m_SizeOfVertex );
#else
			memcpy( &m_Buffer[m_FloatIndex], static_cast<const char*>(vertexPtr) + m_SizeOfVertex * i, m_SizeOfVertex );
#endif // SHARED_MEMERY
			m_FloatIndex += m_CountFloat;
		}
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	void SharedBuffer::CleanUp()
	{
#ifdef SHAREDMEMERY_DEBUG
		SharedMemery::Instance().BeginFlush();
#endif // SHAREDMEMERY_DEBUG
		m_IndiesIndex = 0;
		m_FloatIndex = 0;
		m_IndiesOffset = 0;
		//m_Buffer.clear();
		//m_vIndies.clear();
		//m_Buffer.resize(m_CountFloat * m_CountVertexPerNode * m_CountNode);
		//m_vIndies.resize(m_CountIndiesPerNode * m_CountNode);
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	void SharedBuffer::UseInstance()
	{
		m_isInstance = true;
		int size = m_CountNode * m_CountVertexPerNode * Matrix44f::Size();
#ifdef SHARED_MEMERY
		SharedMemery::Instance().AllocInstanBuffer(size);
#else
		m_Instancebuffer.resize( size );
#endif // SHARED_MEMERY	
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	void SharedBuffer::AddMatrix( Matrix44f& mat)
	{
		for ( int j = 0 ; j < m_CountVertexPerNode ; j ++ )
		{
#ifdef SHARED_MEMERY
			memcpy( SharedMemery::Instance().InstancePtr( ( j + m_NodeIndex * m_CountVertexPerNode ) * Matrix44f::Size() ), mat.GetMatrixArray(), Matrix44f::BitSize() );
#else
			memcpy( &m_Instancebuffer[ ( j + m_NodeIndex * m_CountVertexPerNode ) * Matrix44f::Size() ], mat.GetMatrixArray(), Matrix44f::BitSize() );
#endif // SHARED_MEMERY
		}
		m_NodeIndex ++;
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	void SharedBuffer::CleanUpInstance()
	{
		m_NodeIndex = 0;
#ifdef SHAREDMEMERY_DEBUG
		SharedMemery::Instance().BeginInstanceFlush();
#endif // SHAREDMEMERY_DEBUG
		//m_Instancebuffer.clear();
		//int size = m_CountNode*m_CountVertexPerNode*sizeof(Matrix44f);
		//m_Instancebuffer.resize( size );
	}





	//
	///////////////////////////////////
	////			VertexBuffer	   //
	///////////////////////////////////
	//VertexBuffer::VertexBuffer(Device::RenderMode Mode, Device::Useage use, Type type, uint countVertex, uint countIndies, uint countNode)
	//:m_uMaxNodeCount(countNode),
	//m_eBufferUsage(use),
	//m_nVertexCountPerNode(countVertex),
	//m_nIndiesCountPerNode(countIndies),
	//m_nMaxCount(countNode * countVertex),
	//m_nCount(0),
	//m_eRenderMode(Mode),
	//m_uType(static_cast<uint>(type)),
	//m_nCountOfIndies(0),
	//m_nCountCoordinateVector(0),
	//m_vIndies(NULL),
	//m_vVertex(NULL),
	//m_vColor(NULL),
	//m_pvCoordinate(NULL),
	//m_vNormal(NULL),
	//m_isFlushed(false),
	//m_hVertexBuffer(-1),
	//m_hColorBuffer(-1),
	//m_hNormalBuffer(-1),
	//m_hIndexBuffer(-1),
	//m_hCoordinateBufferArray(NULL)
	//{
	//	bool haveNorma = false;
	//	bool haveColor = false;
	//
	//
	//	if( isType( m_uType, INTERNAL_COLOR0 ) )
	//	{
	//		haveColor = true;
	//	}
	//	if( isType( m_uType, INTERNAL_COORDINATE0 ) )
	//	{
	//		m_nCountCoordinateVector++;
	//	}
	//	if( isType( m_uType, INTERNAL_COORDINATE1 ) )
	//	{
	//		m_nCountCoordinateVector++;
	//	}
	//	if( isType( m_uType, INTERNAL_NORMAL ) )
	//	{
	//		haveNorma = true;
	//	}
	//
	//	engine& engine = engine::Instance();
	//	m_vVertex = NEW FloatVector(countNode * countVertex * CountPerVertex);
	//	m_vIndies = NEW IntVector(countNode * countIndies);
	//
	//	//初始化颜色数组
	//	if( true == haveColor )
	//	{
	//		m_vColor = NEW FloatVector(countNode * countVertex * CountPerColor);
	//	}
	//
	//	//初始化纹理坐标数组
	//	if( 0 != m_nCountCoordinateVector )
	//	{
	//		m_hCoordinateBufferArray = NEW handle [m_nCountCoordinateVector];
	//		m_pvCoordinate = NEW FloatVector*[m_nCountCoordinateVector];
	//		for( uint i = 0 ; i < m_nCountCoordinateVector ; i ++ )
	//		{
	//			m_pvCoordinate[i] = NEW FloatVector(countNode * countVertex * CountPerCoordinate);
	//		}
	//	}
	//	//初始化法线数组
	//	if( true == haveNorma )
	//	{
	//		m_vNormal = NEW FloatVector(countNode * countVertex * CountPerNormal);
	//	}
	//}
	////-------------------------------------------------------------------
	//VertexBuffer::VertexBuffer( const VertexBuffer& input)
	//:m_nVertexCountPerNode(input.m_nVertexCountPerNode),
	//m_nIndiesCountPerNode(input.m_nIndiesCountPerNode),
	//m_nCount(input.m_nCount),
	//m_eRenderMode(input.m_eRenderMode),
	//m_uType(input.m_uType),
	//m_nCountOfIndies(input.m_nCountOfIndies),
	//m_nCountCoordinateVector(input.m_nCountCoordinateVector),
	//m_hVertexBuffer(-1),
	//m_hColorBuffer(-1),
	//m_hNormalBuffer(-1),
	//m_hIndexBuffer(-1),
	//m_hCoordinateBufferArray(NULL)
	//{
	//	m_vIndies = NULL == input.m_vIndies ? NULL : NEW IntVector( *input.m_vIndies );
	//	m_vVertex = NULL == input.m_vVertex ? NULL : NEW FloatVector( *input.m_vVertex );
	//	m_vColor  = NULL == input.m_vColor  ? NULL : NEW FloatVector( *input.m_vColor );
	//	m_vNormal = NULL == input.m_vNormal ? NULL : NEW FloatVector( *input.m_vNormal );
	//	if( 0 != m_nCountCoordinateVector )
	//	{
	//		m_pvCoordinate = NEW FloatVector*[m_nCountCoordinateVector];
	//		for( uint i = 0 ; i < m_nCountCoordinateVector ; i ++ )
	//		{
	//			m_pvCoordinate[i] = NEW FloatVector( *input.m_pvCoordinate[i] );
	//		}
	//	}
	//	else
	//	{
	//		m_pvCoordinate = NULL;
	//	}
	//}
	////-------------------------------------------------------------------
	//VertexBuffer::~VertexBuffer ()
	//{
	//	engine& engine = engine::Instance();
	//
	//	SAFE_DELETE(m_vVertex);
	//	SAFE_DELETE(m_vColor);
	//	SAFE_DELETE(m_vNormal);
	//	SAFE_DELETE(m_vIndies);
	//
	//	if( NULL != m_pvCoordinate )
	//	{
	//		for( uint i = 0 ; i < m_nCountCoordinateVector ; i ++ )
	//		{
	//			SAFE_DELETE(m_pvCoordinate[i]);
	//		}
	//		SAFE_DELETE_ARRAY(m_pvCoordinate);
	//	}
	//
	//	//清理VBO
	//	if( isType( m_uType, INTERNAL_VERTEX ) )
	//	{
	//		engine.GetDevice()->DeleteBuffer( m_hVertexBuffer );
	//	}
	//	if( isType( m_uType, INTERNAL_COLOR0 ) )
	//	{
	//		engine.GetDevice()->DeleteBuffer( m_hColorBuffer );
	//	}
	//	if( isType( m_uType, INTERNAL_COORDINATE0 ) )
	//	{
	//		engine.GetDevice()->DeleteBuffer( m_hCoordinateBufferArray[0] );
	//	}
	//	if( isType( m_uType, INTERNAL_COORDINATE1 ) )
	//	{
	//		engine.GetDevice()->DeleteBuffer( m_hCoordinateBufferArray[1] );
	//	}
	//	if( isType( m_uType, INTERNAL_NORMAL ) )
	//	{
	//		engine.GetDevice()->DeleteBuffer( m_hNormalBuffer );
	//	}
	//	if( 0 != m_nCountCoordinateVector )
	//	{
	//		SAFE_DELETE_ARRAY(m_hCoordinateBufferArray);
	//	}
	//}
	////-------------------------------------------------------------------
	//void VertexBuffer::CleanUp()
	//{
	//	if( NULL != m_vVertex)
	//	{
	//		m_vVertex->clean();
	//	}
	//	if( NULL != m_vNormal)
	//	{
	//		m_vNormal->clean();
	//	}
	//	if( NULL != m_vIndies)
	//	{
	//		m_vIndies->clean();
	//	}
	//	if( NULL != m_vColor)
	//	{
	//		m_vColor->clean();
	//	}
	//	for( uint i = 0 ; i < m_nCountCoordinateVector ; i ++ )
	//	{
	//		m_pvCoordinate[i]->clean();
	//	}
	//	m_nCount = 0;
	//	m_nCountOfIndies = 0;
	//}
	////-------------------------------------------------------------------
	//void VertexBuffer::FlushBuffer()
	//{
	//	engine& engine = engine::Instance();
	//	if( m_isFlushed )
	//	{
	//		ASSERT( Device::DYNAMIC == m_eBufferUsage );//必须是动态的才可以
	//		if( NULL != m_vVertex)
	//		{
	//			engine.GetDevice()->ChangeData( m_hVertexBuffer, Device::VERTEX_BUFFER, 0,  m_vVertex->MaxBitSize(), m_vVertex->GetDataArray()  );
	//		}
	//		if( NULL != m_vNormal)
	//		{
	//			engine.GetDevice()->ChangeData( m_hNormalBuffer, Device::VERTEX_BUFFER, 0, m_vNormal->MaxBitSize(), m_vNormal->GetDataArray()  );
	//		}
	//		if( NULL != m_vIndies)
	//		{
	//			engine.GetDevice()->ChangeData( m_hIndexBuffer, Device::INDIES_BUFFER, 0, m_vIndies->MaxBitSize(), m_vIndies->GetDataArray()  );
	//		}
	//		if( NULL != m_vColor)
	//		{
	//			engine.GetDevice()->ChangeData( m_hColorBuffer, Device::VERTEX_BUFFER, 0, m_vColor->MaxBitSize(), m_vColor->GetDataArray()  );
	//		}
	//		for( uint i = 0 ; i < m_nCountCoordinateVector ; i ++ )
	//		{
	//			engine.GetDevice()->ChangeData( m_hCoordinateBufferArray[i], Device::VERTEX_BUFFER, 0, m_pvCoordinate[i]->MaxBitSize(), m_pvCoordinate[i]->GetDataArray()  );
	//		}
	//	}
	//	else
	//	{
	//		m_isFlushed = true;
	//		if( NULL != m_vVertex)
	//		{
	//			engine.GetDevice()->CreateBuffer( m_hVertexBuffer, Device::VERTEX_BUFFER, m_vVertex->MaxBitSize(), m_vVertex->GetDataArray(), m_eBufferUsage );
	//		}
	//		if( NULL != m_vNormal)
	//		{
	//			engine.GetDevice()->CreateBuffer( m_hNormalBuffer, Device::VERTEX_BUFFER, m_vNormal->MaxBitSize(), m_vNormal->GetDataArray(), m_eBufferUsage );
	//		}
	//		if( NULL != m_vIndies)
	//		{
	//			engine.GetDevice()->CreateBuffer( m_hIndexBuffer, Device::INDIES_BUFFER, m_vIndies->MaxBitSize(), m_vIndies->GetDataArray(), m_eBufferUsage );
	//		}
	//		if( NULL != m_vColor)
	//		{
	//			engine.GetDevice()->CreateBuffer( m_hColorBuffer, Device::VERTEX_BUFFER, m_vColor->MaxBitSize(), m_vColor->GetDataArray(), m_eBufferUsage );
	//		}
	//		for( uint i = 0 ; i < m_nCountCoordinateVector ; i ++ )
	//		{
	//			engine.GetDevice()->CreateBuffer( m_hCoordinateBufferArray[i], Device::VERTEX_BUFFER, m_pvCoordinate[i]->MaxBitSize(), m_pvCoordinate[i]->GetDataArray(), m_eBufferUsage );
	//		}
	//		if ( Device::STATIC == m_eBufferUsage )//如果是静态的就可以清除这些个数据了
	//		{
	//			SAFE_DELETE(m_vVertex);
	//			SAFE_DELETE(m_vColor);
	//			SAFE_DELETE(m_vNormal);
	//			SAFE_DELETE(m_vIndies);
	//
	//			for( uint i = 0 ; i < m_nCountCoordinateVector ; i ++ )
	//			{
	//				SAFE_DELETE(m_pvCoordinate[i]);
	//			}
	//			SAFE_DELETE_ARRAY(m_pvCoordinate);
	//		}
	//	}
	//}
	////-------------------------------------------------------------------
	//void VertexBuffer::AddVertex(const Vertex* const vertexArray, const uint Indieses[])
	//{
	//	for( uint i = 0 ; i < m_nVertexCountPerNode ; i ++ )
	//	{
	//		AddPostion(vertexArray[i].m_Vertex);
	//	}
	//	for( uint i = 0 ; i < m_nIndiesCountPerNode ; i ++ )
	//	{
	//		m_vIndies->push_back(Indieses[i] + m_nCount);
	//	}
	//	m_nCount += m_nVertexCountPerNode;
	//	m_nCountOfIndies += m_nIndiesCountPerNode;
	//}
	////-------------------------------------------------------------------
	////根据不同的顶点形式添加不同的数据
	//void VertexBuffer::AddVertex(const VertexColor* const vertexArray[], const uint Indieses[])
	//{
	//	for( uint i = 0 ; i < m_nVertexCountPerNode ; i ++ )
	//	{
	//		AddPostion(vertexArray[i]->m_Vertex);
	//		if( isType( m_uType, INTERNAL_COLOR0 ) )
	//		{
	//			AddColor(vertexArray[i]->m_Color);
	//		}
	//	}
	//	for( uint i = 0 ; i < m_nIndiesCountPerNode ; i ++ )
	//	{
	//		m_vIndies->push_back(Indieses[i] + m_nCount);
	//	}
	//	m_nCount += m_nVertexCountPerNode;
	//	m_nCountOfIndies += m_nIndiesCountPerNode;
	//}
	////-------------------------------------------------------------------
	//void VertexBuffer::AddVertex(const VertexTexture* const vertexArray[], const uint Indieses[])
	//{
	//	for( uint i = 0 ; i < m_nVertexCountPerNode ; i ++ )
	//	{
	//		AddPostion(vertexArray[i]->m_Vertex);
	//		if( 0 != m_nCountCoordinateVector )
	//		{
	//			AddCoordinate( 0, vertexArray[i]->m_Coordinate);
	//		}
	//	}
	//	for( uint i = 0 ; i < m_nIndiesCountPerNode ; i ++ )
	//	{
	//		m_vIndies->push_back(Indieses[i] + m_nCount);
	//	}
	//	m_nCount += m_nVertexCountPerNode;
	//	m_nCountOfIndies += m_nIndiesCountPerNode;
	//}
	////-------------------------------------------------------------------
	//void VertexBuffer::AddVertex(const VertexTextureNormal* const vertexArray[], const uint Indieses[])
	//{
	//	for( uint i = 0 ; i < m_nVertexCountPerNode ; i ++ )
	//	{
	//		AddPostion(vertexArray[i]->m_Vertex);
	//		if( 0 != m_nCountCoordinateVector )
	//		{
	//			AddCoordinate( 0, vertexArray[i]->m_Coordinate);
	//		}
	//		if( isType( m_uType, INTERNAL_NORMAL ) )
	//		{
	//			AddNormal(vertexArray[i]->m_Normal);
	//		}
	//	}
	//	for( uint i = 0 ; i < m_nIndiesCountPerNode ; i ++ )
	//	{
	//		m_vIndies->push_back(Indieses[i] + m_nCount);
	//	}
	//	m_nCount += m_nVertexCountPerNode;
	//	m_nCountOfIndies += m_nIndiesCountPerNode;
	//}
	////-------------------------------------------------------------------
	//void VertexBuffer::AddVertex(const VertexColor vertexArray[], const uint Indieses[])
	//{
	//	for( uint i = 0 ; i < m_nVertexCountPerNode ; i ++ )
	//	{
	//		AddPostion(vertexArray[i].m_Vertex);
	//		if( isType( m_uType, INTERNAL_COLOR0 ) )
	//		{
	//			AddColor(vertexArray[i].m_Color);
	//		}
	//	}
	//	for( uint i = 0 ; i < m_nIndiesCountPerNode ; i ++ )
	//	{
	//		m_vIndies->push_back(Indieses[i] + m_nCount);
	//	}
	//	m_nCount += m_nVertexCountPerNode;
	//	m_nCountOfIndies += m_nIndiesCountPerNode;
	//}
	////-------------------------------------------------------------------
	//void VertexBuffer::AddVertex(const VertexTexture vertexArray[], const uint Indieses[])
	//{
	//	for( uint i = 0 ; i < m_nVertexCountPerNode ; i ++ )
	//	{
	//		AddPostion(vertexArray[i].m_Vertex);
	//		if( 0 != m_nCountCoordinateVector )
	//		{
	//			AddCoordinate( 0, vertexArray[i].m_Coordinate);
	//		}
	//	}
	//	for( uint i = 0 ; i < m_nIndiesCountPerNode ; i ++ )
	//	{
	//		m_vIndies->push_back(Indieses[i] + m_nCount);
	//	}
	//	m_nCount += m_nVertexCountPerNode;
	//	m_nCountOfIndies += m_nIndiesCountPerNode;
	//}
	////-------------------------------------------------------------------
	//void VertexBuffer::AddVertex(const VertexTextureNormal vertexArray[], const uint Indieses[])
	//{
	//	for( uint i = 0 ; i < m_nVertexCountPerNode ; i ++ )
	//	{
	//		AddPostion(vertexArray[i].m_Vertex);
	//		if( 0 != m_nCountCoordinateVector )
	//		{
	//			AddCoordinate( 0, vertexArray[i].m_Coordinate);
	//		}
	//		if( isType( m_uType, INTERNAL_NORMAL ) )
	//		{
	//			AddNormal(vertexArray[i].m_Normal);
	//		}
	//	}
	//	for( uint i = 0 ; i < m_nIndiesCountPerNode ; i ++ )
	//	{
	//		m_vIndies->push_back(Indieses[i] + m_nCount);
	//	}
	//	m_nCount += m_nVertexCountPerNode;
	//	m_nCountOfIndies += m_nIndiesCountPerNode;
	//}
	////-------------------------------------------------------------------
	//void VertexBuffer::AddVertex(const VertexTextureColor vertexArray[], const uint Indieses[])
	//{
	//	for( uint i = 0 ; i < m_nVertexCountPerNode ; i ++ )
	//	{
	//		AddPostion(vertexArray[i].m_Vertex);
	//		if( 0 != m_nCountCoordinateVector )
	//		{
	//			AddCoordinate( 0, vertexArray[i].m_Coordinate);
	//		}
	//		if( isType( m_uType, INTERNAL_COLOR0 ) )
	//		{
	//			AddColor(vertexArray[i].m_Color);
	//		}
	//	}
	//	for( uint i = 0 ; i < m_nIndiesCountPerNode ; i ++ )
	//	{
	//		m_vIndies->push_back(Indieses[i] + m_nCount);
	//	}
	//	m_nCount += m_nVertexCountPerNode;
	//	m_nCountOfIndies += m_nIndiesCountPerNode;
	//}
	////-------------------------------------------------------------------
	//void VertexBuffer::UsingBuffer(uint type)
	//{
	//	ASSERT( true == m_isFlushed );//必须是填充过的
	//	engine& engine = engine::Instance();
	//	if( isType( type, INTERNAL_VERTEX ) )
	//	{
	//		engine.BindBuffer( m_hVertexBuffer, Device::VERTEX_BUFFER);
	//		//engine.UsingVertexAttribPointer(Attributes::ATTRIBUTE_POSITION, CountPerVertex, false, 0, m_vVertex );
	//		engine.UsingVertexAttribPointer(Attributes::ATTRIBUTE_POSITION, CountPerVertex, false, 0, NULL );
	//	}
	//	if( isType( type, INTERNAL_COLOR0 ) )
	//	{
	//		engine.BindBuffer( m_hColorBuffer, Device::VERTEX_BUFFER);
	//		//engine.UsingVertexAttribPointer(Attributes::ATTRIBUTE_COLOR0, CountPerColor, false, 0, m_vColor );
	//		engine.UsingVertexAttribPointer(Attributes::ATTRIBUTE_COLOR0, CountPerColor, false, 0, NULL );
	//	}
	//	if( isType( type, INTERNAL_COORDINATE0 ) )
	//	{
	//		engine.BindBuffer( m_hCoordinateBufferArray[0], Device::VERTEX_BUFFER);
	//		//engine.UsingVertexAttribPointer(Attributes::ATTRIBUTE_TEXCOORD0, CountPerCoordinate, false, 0, m_pvCoordinate[0] );
	//		engine.UsingVertexAttribPointer(Attributes::ATTRIBUTE_TEXCOORD0, CountPerCoordinate, false, 0, NULL );
	//	}
	//	if( isType( type, INTERNAL_COORDINATE1 ) )
	//	{
	//		engine.BindBuffer( m_hCoordinateBufferArray[1], Device::VERTEX_BUFFER);
	//		//engine.UsingVertexAttribPointer(Attributes::ATTRIBUTE_TEXCOORD1, CountPerCoordinate, false, 0, m_pvCoordinate[1] );
	//		engine.UsingVertexAttribPointer(Attributes::ATTRIBUTE_TEXCOORD1, CountPerCoordinate, false, 0, NULL );
	//	}
	//	if( isType( type, INTERNAL_NORMAL ) )
	//	{
	//		engine.BindBuffer( m_hNormalBuffer, Device::VERTEX_BUFFER);
	//		//engine.UsingVertexAttribPointer(Attributes::ATTRIBUTE_NORMAL, CountPerNormal, false, 0, m_vNormal );
	//		engine.UsingVertexAttribPointer(Attributes::ATTRIBUTE_NORMAL, CountPerNormal, false, 0, NULL );
	//	}
	//}
	////-------------------------------------------------------------------
	//void VertexBuffer::UsingBuffer()
	//{
	//	UsingBuffer( m_uType );
	//}
	////-------------------------------------------------------------------
	//void VertexBuffer::UsingBuffer(Type type)
	//{
	//	uint utype = static_cast<uint>(type);
	//	UsingBuffer( utype );
	//}
	////-------------------------------------------------------------------
	//void VertexBuffer::DropBuffer()
	//{
	//	engine& engine = engine::Instance();
	//	if( isType( m_uType, INTERNAL_VERTEX ) )
	//	{
	//		engine.DropVertexAttribPointer(Attributes::ATTRIBUTE_POSITION);
	//	}
	//	if( isType( m_uType, INTERNAL_COLOR0 ) )
	//	{
	//		engine.DropVertexAttribPointer(Attributes::ATTRIBUTE_COLOR0);
	//	}
	//	if( isType( m_uType,  INTERNAL_COORDINATE0 ) )
	//	{
	//		engine.DropVertexAttribPointer(Attributes::ATTRIBUTE_TEXCOORD0);
	//	}
	//	if( isType( m_uType,  INTERNAL_COORDINATE1 ) )
	//	{
	//		engine.DropVertexAttribPointer(Attributes::ATTRIBUTE_TEXCOORD1);
	//	}
	//	if( isType( m_uType,  INTERNAL_NORMAL ) )
	//	{
	//		engine.DropVertexAttribPointer(Attributes::ATTRIBUTE_NORMAL);
	//	}
	//}

}