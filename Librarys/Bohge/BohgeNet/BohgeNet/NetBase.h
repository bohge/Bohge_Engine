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
//		网络基础类	   //
/////////////////////////
#pragma once
#include "BohgeNet.h"
#include <vector>
#include <string>

namespace BohgeNet
{
	class Address;
	class Domain
	{
	private:
		std::string		m_Domain;
		ushort			m_Port;
	public:
		Domain()
			:m_Port(0)
		{

		}
		Domain( const std::string& domain, ushort port )
			:m_Domain(domain),
			m_Port(port)
		{
		}
		BOHGE_FORCEINLINE bool operator == (const Domain& r) const
		{
			if( m_Port == r.m_Port )
			{
				if( m_Domain == r.m_Domain )
				{
					return true;
				}
			}
			return false;
		}
		BOHGE_FORCEINLINE bool operator != (const Domain& r) const
		{
			return !( *this == r );
		}
		Address ToAddress();
	};
	class Address //网络IP和端口
	{
	private:
		ulong	m_Ip;
		ushort	m_Port;
	public:
		Address();
		Address( byte a, byte b, byte c, byte d, ushort port );
		Address( ulong ip, ushort port );
	public:
		BOHGE_FORCEINLINE ulong IP() const
		{
			return m_Ip;
		}
		BOHGE_FORCEINLINE byte A() const
		{
			return static_cast<byte>(m_Ip>>24);
		}
		BOHGE_FORCEINLINE byte B() const
		{
			return static_cast<byte>(m_Ip>>16);
		}
		BOHGE_FORCEINLINE byte C() const
		{
			return static_cast<byte>(m_Ip>>8);
		}
		BOHGE_FORCEINLINE byte D() const
		{
			return static_cast<byte>(m_Ip);
		}
		BOHGE_FORCEINLINE ushort Port() const
		{
			return m_Port;
		}
		BOHGE_FORCEINLINE void NewPort( ushort p )
		{
			m_Port = p;
		}
		BOHGE_FORCEINLINE void Clear()
		{
			m_Ip = 0;
			m_Port = 0;
		}
		BOHGE_FORCEINLINE bool operator == (const Address& r) const
		{
			return (m_Ip == r.m_Ip) && (m_Port == r.m_Port);
		}
		BOHGE_FORCEINLINE bool operator != (const Address& r) const
		{
			return (m_Ip != r.m_Ip) || (m_Port != r.m_Port);
		}

		//为map构造的
		BOHGE_FORCEINLINE bool operator < (const Address& r) const
		{
			if ( m_Ip < r.m_Ip )
			{
				return true;
			}
			else if ( m_Ip > r.m_Ip)
			{
				return false;
			}
			else
			{
				if( m_Port < r.m_Port )
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			return false;
		}
	};
	//-------------------------------------------------------------------------------------------------------
	class SocketUDP //通讯端口
	{
		friend class UDPServer;
		friend class UDPCorrespondent;
	private:
		bool					m_isConnect;//是否成功打开
		int						m_nSocket;
		ushort					m_LocalPort;//打开的本地端口
	private:
		SocketUDP(ushort port);//构造的时候绑定一个端口
		~SocketUDP();//析构的时候关闭端口
	public:
		bool Connect( bool isBlock );
		void Close( );
		bool Send( const Address & destination, const void * data, int size );//返回指是否端口异常
		int Receive(Address & sender, void * data, int size);
	public:
		BOHGE_FORCEINLINE bool isConnect()
		{
			return m_isConnect;
		}
	};
	//-------------------------------------------------------------------------------------------------------
	class SocketTCP //通讯端口
	{
		friend class TCPCorrespondent;
	private:
		bool					m_isConnect;//是否成功打开
		int						m_nSocket;
	private:
		SocketTCP( );//构造的时候绑定一个端口
		~SocketTCP();//析构的时候关闭端口
	public:
		bool Connect( const Address& server );
		void Close( );
		bool Send( const void * data, int size );//返回指是否端口异常
		int Receive( void * data, int size);
	public:
		BOHGE_FORCEINLINE bool isConnect()
		{
			return m_isConnect;
		}
	};
	//-------------------------------------------------------------------------------------------------------
	class NetPackage
	{
		friend class Packer;
	private:
		std::vector<byte>		m_vBuffer;
		uint					m_Cursor;
		uint					m_Size;
	public:
		NetPackage();
	private:
		BOHGE_FORCEINLINE void SetSize( uint size )//packer读取的时候将此包裹的大小设定，不能使用m_Cursor这样没法转包，会丢失字节
		{
			m_Size = size;
		}
	public:
		BOHGE_FORCEINLINE byte* GetBuffer()
		{
			return &m_vBuffer[0];
		}
		BOHGE_FORCEINLINE uint Size() const
		{
			//return m_Cursor;
			return m_Size;
		}
		BOHGE_FORCEINLINE void Begin()//开始读写前调用
		{
			m_Cursor = 0;
		}
		static BOHGE_FORCEINLINE uint Sizeof( const std::string& str )
		{
			return sizeof(int) + str.size();
		}
	private:
		BOHGE_FORCEINLINE void Read(int bytes, void * pBuffer)
		{
			memcpy( pBuffer, &m_vBuffer[m_Cursor], bytes );
			m_Cursor+=bytes;//如果只读取则不会改变包裹的size
		}
		BOHGE_FORCEINLINE void Write(int bytes, const void * pBuffer)
		{
			memcpy( &m_vBuffer[m_Cursor], pBuffer, bytes );
			m_Cursor+=bytes;
			m_Size = m_Cursor;//写入后，会导致包裹大小的变化
		}
	public:
		//写入
		BOHGE_FORCEINLINE NetPackage& operator<< (const std::string& n)
		{
			ushort size = n.size();
			*this<<size;
			for(int i = 0 ; i < size ; i ++ )
			{
				*this<<n[i];
			}
			return *this;
		}
		BOHGE_FORCEINLINE NetPackage& operator<< (const std::wstring& n)
		{
			ushort size = n.size();
			*this<<size;
			for(int i = 0 ; i < size ; i ++ )
			{
				*this<<n[i];
			}
			return *this;
		}
		BOHGE_FORCEINLINE NetPackage& operator<< (bool n)
		{
			this->Write(sizeof(bool), &n);
			return *this;
		}
		BOHGE_FORCEINLINE NetPackage& operator<< (char n)
		{
			this->Write(sizeof(char), &n);
			return *this;
		}
		BOHGE_FORCEINLINE NetPackage& operator<< (wchar_t n)
		{
			this->Write(2, &n);
			return *this;
		}
		BOHGE_FORCEINLINE NetPackage& operator<< (int n)
		{
			this->Write(sizeof(int), &n);
			return *this;
		}
		BOHGE_FORCEINLINE NetPackage& operator<< (uint n)
		{
			this->Write(sizeof(uint), &n);
			return *this;
		}
		BOHGE_FORCEINLINE NetPackage& operator<< (short n)
		{
			this->Write(sizeof(short), &n);
			return *this;
		}
		BOHGE_FORCEINLINE NetPackage& operator<< (ushort n)
		{
			this->Write(sizeof(ushort), &n);
			return *this;
		}
		BOHGE_FORCEINLINE NetPackage& operator<< (float n)
		{
			this->Write(sizeof(float), &n);
			return *this;
		}
		BOHGE_FORCEINLINE NetPackage& operator<< (double n)
		{
			this->Write(sizeof(double), &n);
			return *this;
		}
		NetPackage& operator<<( const std::vector<bool>& vec )
		{
			int Volume = sizeof( int ) * 8;//一个int有多少位
			ushort size = vec.size();
			*this<<size;
			for ( int i = 0 ; i < vec.size() ; i += Volume )
			{
				int Container = 0;
				for( int j = 0 ; j < Volume ; j ++ )
				{
					if( i+j > vec.size() )
					{
						break;
					}
					else
					{
						if( vec[ i + j ] )
						{
							Container |= ( 1 << j );
						}
					}
				}
				(*this)<<Container;
			}
			return *this;
		}

		//读取
		BOHGE_FORCEINLINE NetPackage& operator>> (std::string& n)
		{
			ushort num;
			*this>>num;
			if( num > BUFFER_SIZE )//超过这个buffer最大的发送范围，肯定是不正确的包裹
			{
				return *this;
			}
			if ( n.size() != num )
			{
				n.resize( num );
			}
			for( int i = 0 ; i < num ; i ++ )
			{
				*this>>n[i];
			}
			return *this;
		}
		BOHGE_FORCEINLINE NetPackage& operator>> (std::wstring& n)
		{
			ushort num;
			*this>>num;
			if( num > BUFFER_SIZE )//超过这个buffer最大的发送范围，肯定是不正确的包裹
			{
				return *this;
			}
			if ( n.size() != num )
			{
				n.resize( num );
			}
			for( int i = 0 ; i < num ; i ++ )
			{
				*this>>n[i];
			}
			return *this;
		}
		BOHGE_FORCEINLINE NetPackage& operator>> (bool& n)
		{
			this->Read(sizeof(bool), &n);
			return *this;
		}
		BOHGE_FORCEINLINE NetPackage& operator>> (char& n)
		{
			this->Read(sizeof(char), &n);
			return *this;
		}
		BOHGE_FORCEINLINE NetPackage& operator>> (wchar_t& n)
		{
			this->Read(2, &n);
			return *this;
		}
		BOHGE_FORCEINLINE NetPackage& operator>> (int& n)
		{
			this->Read(sizeof(int), &n);
			return *this;
		}
		BOHGE_FORCEINLINE NetPackage& operator>> (uint& n)
		{
			this->Read(sizeof(uint), &n);
			return *this;
		}
		BOHGE_FORCEINLINE NetPackage& operator>> (short& n)
		{
			this->Read(sizeof(short), &n);
			return *this;
		}
		BOHGE_FORCEINLINE NetPackage& operator>> (ushort& n)
		{
			this->Read(sizeof(ushort), &n);
			return *this;
		}
		BOHGE_FORCEINLINE NetPackage& operator>> (float& n)
		{
			this->Read(sizeof(float), &n);
			return *this;
		}
		BOHGE_FORCEINLINE NetPackage& operator>> (double& n)
		{
			this->Read(sizeof(double), &n);
			return *this;
		}
		NetPackage& operator>>( std::vector<bool>& vec )
		{
			int Volume = sizeof( int ) * 8;//一个int有多少位
			ushort size = 0;
			*this>>size;
#ifdef _DEBUG
			ASSERT( vec.size() == size);
#else
			if( size != vec.size() )
			{
				vec.resize( size );
			}
#endif
			for ( int i = 0 ; i < vec.size() ; i += Volume )
			{
				int Container = 0;
				(*this)>>Container;
				for( int j = 0 ; j < Volume ; j ++ )
				{
					if( i+j > vec.size() )
					{
						break;
					}
					else
					{
						if( ( ( Container >> j ) & 0x00000001 ) == 1 )
						{
							vec[i+j] = true;
						}
						else
						{
							vec[i+j] = false;
						}
					}
				}
			}
			return *this;
		}
	};
}
