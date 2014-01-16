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



//--------------------------------------------------------------------------------
//目前这种机制m_isBufferChange会导致数据在服务器端的堆积
//如果服务器响应不过来，会把消息堆积起来，等待处理
//所以目前这种机制避免一次性的大量发送数据包
//这个类是收发安全的，不同线程可以同时调用收发
//--------------------------------------------------------------------------------

#pragma once
#include "NetMessage.h"
#include "NetBase.h"
#include <list>


namespace BohgeNet
{

	class Packer	//用于接收要发送的包裹
	{
	public:
		enum PackStatus
		{
			PS_BADPACKAGE,//坏包裹
			PS_FRAGMENT,//只是片断
			PS_ALLDONE,//收完毕
		};
	private:
		enum
		{
			Recev_Alloc_Size = 5,//一次准备5条接受数据的包裹
		};
	public:
		typedef std::vector<byte>	Buffer;
	private:
		struct RecevBuffer
		{
			uint		m_size;
			Buffer		m_Buffer;
			RecevBuffer()
				:m_size(0)
			{
				m_Buffer.resize( BUFFER_SIZE );
			}
		};
	private:
		typedef std::vector<RecevBuffer>	RecevBufferVector;
	private:

		//拷贝保存数据相关
		TCPHead								m_Head;//tcp的包头
		ushort								m_PackageCount;
		std::vector<ushort>					m_EachSize;//每一个包裹的长度
		Buffer								m_vInternalBuffer;//保存要发送的数据阵列字符串
		ushort								m_CopySize;//当前保存包裹的长度

		//发送过程填充新的数据了
		bool								m_isBufferChange;

		//发送相关
		int									m_FragamentCount;//片段流的数量
		int									m_NeedSend;//需要发送的数量
		int									m_SendedSize;//已近发送了的大小
		int									m_SendedPackage;//发送的包裹数量

		//接受相关的
		uint								m_RecevFragment;//接受到的片段数量
		uint								m_ReadIndex;
		uint								m_WriteIndex;//当前收到的索引
		RecevBufferVector					m_RecevedVector;//接收到的
	public:
		Packer()
			:m_CopySize(0),
			m_PackageCount(0),
			m_WriteIndex(0),
			m_ReadIndex(0),
			m_SendedSize(0),
			m_SendedPackage(0),
			m_NeedSend(0),
			m_RecevFragment(0),
			m_FragamentCount(0),
			m_isBufferChange(false)
		{
			m_EachSize.resize( TCPHead::Max_Package_Count );
			m_vInternalBuffer.resize( BUFFER_SIZE );
			m_RecevedVector.resize( Recev_Alloc_Size );
		}
	public:
		void Reset()
		{
			m_isBufferChange = false;
			m_FragamentCount = 0;
			m_NeedSend = 0;//重要的清除
			m_CopySize = 0;//重要的清除
			m_PackageCount = 0;//重要的清除
			m_SendedPackage = 0;//重要的清除
			m_SendedSize = 0;//重要的清除
			memset( &m_EachSize[0], 0, m_EachSize.size() );
			memset( &m_vInternalBuffer[0], 0, m_vInternalBuffer.size() );
		}
		BOHGE_FORCEINLINE bool isMulitSend()
		{
			return m_FragamentCount > 1;
		}
		//当接收到数据时，调用此函数将数据拆包,返回值是指这个数据流是否接受完整
		//返回接收状态，接受完整了，可以进行读取
		PackStatus RecevData( byte* buffer )
		{
			memcpy( &m_Head, buffer, sizeof(m_Head) );//将头包裹拷贝进去
			if ( m_Head.m_PackageCount > TCPHead::Max_Package_Count
				|| m_Head.Size() > BUFFER_SIZE )
			{//无效数据包，退出,理论上是不可能的，防止有人黑服务器
				NETDEBUGLOG("Bad Head\n",1);
				ASSERT(false);
				return PS_BADPACKAGE;
			}
			m_RecevFragment ++;
			int copyedSize = sizeof(m_Head);
			for ( int i = 0 ; i < m_Head.m_PackageCount; i ++ )
			{
				if( m_WriteIndex >= m_RecevedVector.size() )//放不下了
				{
					m_RecevedVector.resize( m_RecevedVector.size() + Recev_Alloc_Size );//就再分配一段
				}
				m_RecevedVector[m_WriteIndex].m_size = m_Head.m_LengthEachPackage[i];
				memcpy( &(m_RecevedVector[m_WriteIndex].m_Buffer[0]), &buffer[copyedSize], m_Head.m_LengthEachPackage[i]);
				copyedSize += m_Head.m_LengthEachPackage[i];
				m_WriteIndex++;
			}
			if ( m_RecevFragment >= m_Head.m_FragmentCount )
			{
				m_RecevFragment = 0;
				return PS_ALLDONE;
			}
			return PS_FRAGMENT;
		}
		bool GetPackage( BohgeNet::NetPackage& pak )
		{
			if ( m_WriteIndex > 0 ) //如果写入个数大于0了，才说明有数据
			{
				if( m_ReadIndex >= m_WriteIndex )
				{
					m_WriteIndex = 0;
					m_ReadIndex = 0;
					return false;
				}
				memcpy( pak.GetBuffer(), &m_RecevedVector[m_ReadIndex].m_Buffer[0], m_RecevedVector[m_ReadIndex].m_size );
				pak.SetSize( m_RecevedVector[m_ReadIndex].m_size );
				m_ReadIndex ++;
				return true;
			}
			return false;
		}
		void CopyPackage( BohgeNet::NetPackage& pak )//复制包裹
		{
			m_NeedSend ++;
			m_isBufferChange = true;
			while( pak.Size() > ( m_vInternalBuffer.size() - m_CopySize ) ) //如果剩余的空间不够保存新发来的buffer，就扩大当前的发送buffer内存块
			{
				m_vInternalBuffer.resize( m_vInternalBuffer.size() + BUFFER_SIZE );
			}
			memcpy( &m_vInternalBuffer[m_CopySize] , pak.GetBuffer(), pak.Size() );
			m_CopySize += pak.Size();
			if( m_PackageCount == m_EachSize.size() )
			{
				m_EachSize.resize( m_EachSize.size() + TCPHead::Max_Package_Count );
			}
			m_EachSize[m_PackageCount] = pak.Size();
			m_PackageCount++;
		}
		bool SendData( byte* out_buffer, int& out_size) //发送当前buffer中的内容,返回的内容是是否还有需要发送的东西
		{
			if( m_NeedSend > 0 )
			{
				m_Head.Reset();
				if(	m_isBufferChange )//当改变过发送buffer,这里就要计算一次需要发送的fragment数量
				{//计算包裹数量
					m_isBufferChange = false;
					uint sizeCount = 0;//局部变量,计量每个包裹的数据量
					uint PackageCount = 0;//打包的包裹数量
					for ( int i = m_SendedPackage ; i < m_NeedSend ; i ++ )
					{
						if (	PackageCount + 1 > TCPHead::Max_Package_Count
							||	sizeCount + this->m_EachSize[i] > BUFFER_SIZE )
						{
							sizeCount = 0;
							PackageCount = 0;
							m_FragamentCount ++;
						}
						{//统计需要发送的讯息
							sizeCount += this->m_EachSize[i];
							PackageCount ++;
						}
					}//for
					if( 0 != PackageCount )//如果没包裹了，把最后这个数据包裹发送了
					{
						m_FragamentCount ++;
					}
				}
				{ //具体的开始发送
					uint sizeCount = 0;//局部变量,计量每个包裹的数据量
					uint PackageCount = 0;//打包的包裹数量
					uint packageIndex = 0;//当前包裹的计数
					m_Head.m_FragmentCount = m_FragamentCount;//写入这一串发送流的片段数量
					for ( int i = m_SendedPackage ; i < m_NeedSend ; i ++ )
					{
						if (	PackageCount + 1 > TCPHead::Max_Package_Count
							||	sizeCount + this->m_EachSize[i] > BUFFER_SIZE )
						{
							m_Head.m_PackageCount = PackageCount;
							memcpy( out_buffer, &m_Head, sizeof(m_Head) );//将头包裹拷贝进去
							memcpy( &out_buffer[sizeof(m_Head)], &this->m_vInternalBuffer[m_SendedSize], sizeCount );//拷贝数据段
							out_size = sizeCount + sizeof(m_Head);
							m_SendedSize += sizeCount;
							m_SendedPackage = i;//下次开始从i+1开始发送
							return true;
						}
						{//统计需要发送的讯息
							sizeCount += this->m_EachSize[i];
							m_Head.m_LengthEachPackage[packageIndex] = this->m_EachSize[i];
							PackageCount ++;
							packageIndex ++;
						}
					}//for
					if( 0 != PackageCount )//如果没包裹了，把最后这个数据包裹发送了
					{
						m_Head.m_PackageCount = PackageCount;
						memcpy( out_buffer, &m_Head, sizeof(m_Head) );//将头包裹拷贝进去
						memcpy( &out_buffer[sizeof(m_Head)], &this->m_vInternalBuffer[m_SendedSize], sizeCount );//拷贝数据段
						out_size = sizeCount + sizeof(m_Head);
						this->Reset();//发送完要reset
						return true;
					}
				}
			}
			return false;
		}
	public:
		static BOHGE_FORCEINLINE void GetRecevSignal( byte* out_buffer, int& out_size )
		{
			const static byte Signal[ sizeof( SM_RECEV_FRAGMENT ) ] = {
				(byte)(SM_RECEV_FRAGMENT >> 24),
				(byte)(SM_RECEV_FRAGMENT >> 16),
				(byte)(SM_RECEV_FRAGMENT >> 8),
				(byte)SM_RECEV_FRAGMENT
			};
			out_size = sizeof(Signal);
			memcpy( out_buffer, Signal, sizeof(Signal) );
		}
		static BOHGE_FORCEINLINE bool isRecevSignal( byte* buffer )
		{
			int s = buffer[0]<<24 | buffer[1]<<16 | buffer[2]<<8 | buffer[3];
			return s == SM_RECEV_FRAGMENT;
		}
	};

}