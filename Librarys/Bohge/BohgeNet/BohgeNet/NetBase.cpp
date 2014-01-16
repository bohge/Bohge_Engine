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
#include "BohgeNet.h"
#include "NetBase.h"
#include "NetMessage.h"


namespace BohgeNet
{

	//-------------------------------------------------------------------------------------------------------
	Address Domain::ToAddress()
	{
		int address = 0;
		struct hostent* remoteHost = gethostbyname( m_Domain.c_str() );
		if (remoteHost == NULL) 
		{
			NETDEBUGLOG("Cannot get domain %s's ip", m_Domain.c_str());
			return Address();
		} 
		else 
		{
			address = ntohl((*(u_long *) remoteHost->h_addr_list[0]) );
		}
		return Address( address, m_Port );
	}


	//////////////////////
	//		地址		//
	//////////////////////
	Address::Address()
		:m_Ip(0),
		m_Port(0)
	{

	}
	//-------------------------------------------------------------------------------------------------------
	Address::Address( byte a, byte b, byte c, byte d, ushort port )
	{
		m_Ip = ( a << 24 ) | ( b << 16 ) | ( c << 8 ) | d;
		m_Port = port;
	}
	//-------------------------------------------------------------------------------------------------------
	Address::Address( ulong ip, ushort port )
	{
		m_Ip = ip;
		m_Port = port;
	}


	//////////////////////
	//		端口		//
	//////////////////////
	//-------------------------------------------------------------------------------------------------------
	SocketUDP::SocketUDP(ushort port)
		:m_nSocket(0),
		m_LocalPort(port),
		m_isConnect(false)
	{

	}
	//-------------------------------------------------------------------------------------------------------
	SocketUDP::~SocketUDP()
	{
		this->Close();
	}
	//-------------------------------------------------------------------------------------------------------
	bool SocketUDP::Connect( bool isBlock )
	{
		// create socket

		m_nSocket = ::socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );

		if ( m_nSocket <= 0 )
		{
#if PLATFORM == PLATFORM_WINDOWS
			int e = WSAGetLastError();
#endif
			NETDEBUGLOG( "failed to create socket\n",0);
			m_nSocket = 0;
			return false;
		}

		// bind to port

		sockaddr_in address;
		address.sin_family = AF_INET;
		address.sin_addr.s_addr = INADDR_ANY;
		address.sin_port = htons( m_LocalPort );

		if ( bind( m_nSocket, (const sockaddr*) &address, sizeof(sockaddr_in) ) < 0 )
		{
			NETDEBUGLOG( "failed to bind socket\n",0);
			this->Close();
			return false;
		}

		if( !isBlock )// set non-blocking io
		{
#if PLATFORM == PLATFORM_WINDOWS

			DWORD nonBlocking = 1;
			if ( ioctlsocket( m_nSocket, FIONBIO, &nonBlocking ) != 0 )
			{
				NETDEBUGLOG( "failed to set non-blocking socket\n" );
				this->Close();
				return false;
			}
#elif PLATFORM == PLATFORM_IOS || PLATFORM == PLATFORM_ANDROID

			int nonBlocking = 1;
			if ( fcntl( m_nSocket, F_SETFL, O_NONBLOCK, nonBlocking ) == -1 )
			{
				NETDEBUGLOG( "failed to set non-blocking socket\n",0);
				this->Close();
				return false;
			}
#endif
		}
		m_isConnect = true;
		return true;
	}
	//-------------------------------------------------------------------------------------------------------
	void SocketUDP::Close()
	{
		m_isConnect = false;
		if ( m_nSocket != 0 )
		{
#if PLATFORM == PLATFORM_IOS || PLATFORM == PLATFORM_ANDROID
			close( m_nSocket );
#elif PLATFORM == PLATFORM_WINDOWS
			closesocket( m_nSocket );
#endif
			m_nSocket = 0;
		}
	}
	//-------------------------------------------------------------------------------------------------------
	bool SocketUDP::Send( const Address & destination, const void * data, int size )
	{
		if ( m_nSocket == 0 )
			return false;
		ASSERT( destination.IP() != 0 );
		ASSERT( destination.Port() != 0 );
		sockaddr_in address;
		address.sin_family = AF_INET;
		address.sin_addr.s_addr = htonl( destination.IP() );
		address.sin_port = htons( (ushort) destination.Port() );

		int sent_bytes = sendto( m_nSocket, (const char*)data, size, 0, (sockaddr*)&address, sizeof(sockaddr_in) );

		return -1 != sent_bytes; //如果返回-1说明端口错误了
	}
	//-------------------------------------------------------------------------------------------------------
	int SocketUDP::Receive(Address & sender, void * data, int size)
	{
		if ( m_nSocket == 0 )
			return 0;
#if PLATFORM == PLATFORM_WINDOWS
		typedef int socklen_t;
#endif
		sockaddr_in from;
		socklen_t fromLength = sizeof( from );
		int received_bytes = recvfrom( m_nSocket, (char*)data, size, 0, (sockaddr*)&from, &fromLength );
		if ( received_bytes <= 0 )
			return 0;
		uint address = ntohl( from.sin_addr.s_addr );
		ushort port = ntohs( from.sin_port );
		sender = Address( address, port );
		return received_bytes;
	}




	//////////////////////
	//		端口		//
	//////////////////////
	//-------------------------------------------------------------------------------------------------------
	SocketTCP::SocketTCP( )
		:m_nSocket(0),
		m_isConnect(false)
	{

	}
	//-------------------------------------------------------------------------------------------------------
	SocketTCP::~SocketTCP()
	{
		this->Close();
	}
	//-------------------------------------------------------------------------------------------------------
	bool SocketTCP::Connect( const Address& server )
	{
		// create socket
		m_nSocket = ::socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );

		if ( m_nSocket <= 0 )
		{
#if PLATFORM == PLATFORM_WINDOWS
			int e = WSAGetLastError();
#endif
			NETDEBUGLOG( "failed to create socket\n",0);
			m_nSocket = 0;
			return false;
		}
		//设置非延迟类
		const int chOpt=1;
		if( setsockopt(  m_nSocket,   IPPROTO_TCP,   TCP_NODELAY,   (char*)&chOpt,   sizeof(chOpt)) == -1 )   
		{   
			NETDEBUGLOG( "setsockopt()   error\n",1);
			m_nSocket = 0;
			return false;   
		}
		ASSERT( 0 != server.IP() );
		ASSERT( 0 != server.Port() );
		// bind to port
		sockaddr_in address;
		address.sin_family = AF_INET;
		address.sin_addr.s_addr = htonl( server.IP() );
		address.sin_port = htons( server.Port() );
		if ( connect( m_nSocket, reinterpret_cast<const struct sockaddr *>(&address),sizeof(address)) == -1 ) 
		{
			NETDEBUGLOG("cant not connect server！\n", 1);
#if PLATFORM == PLATFORM_WINDOWS
			int e = WSAGetLastError();
			NETDEBUGLOG("Error code %dn", e);
#endif
			this->Close();
			return false; 
		}
		// set non-blocking io
#if PLATFORM == PLATFORM_WINDOWS

		DWORD nonBlocking = 1;
		if ( ioctlsocket( m_nSocket, FIONBIO, &nonBlocking ) != 0 )
		{
			NETDEBUGLOG( "failed to set non-blocking socket\n" );
			this->Close();
			return false;
		}
#elif PLATFORM == PLATFORM_IOS || PLATFORM == PLATFORM_ANDROID

		int nonBlocking = 1;
		if ( fcntl( m_nSocket, F_SETFL, O_NONBLOCK, nonBlocking ) == -1 )
		{
			NETDEBUGLOG( "failed to set non-blocking socket\n",0);
			this->Close();
			return false;
		}
#endif
		m_isConnect = true;
		return true;
	}
	//-------------------------------------------------------------------------------------------------------
	void SocketTCP::Close()
	{
		m_isConnect = false;
		if ( m_nSocket != 0 )
		{
#if PLATFORM == PLATFORM_IOS || PLATFORM == PLATFORM_ANDROID
			close( m_nSocket );
#elif PLATFORM == PLATFORM_WINDOWS
			closesocket( m_nSocket );
#endif
			m_nSocket = 0;
		}
	}
	//-------------------------------------------------------------------------------------------------------
	bool SocketTCP::Send( const void * data, int size )
	{
		if ( m_nSocket != 0 )
		{
			int sent_bytes = send( m_nSocket, (const char*)data, size, 0 );
			return -1 != sent_bytes;
		}
#ifdef _DEBUG
		else
		{
			ASSERT(false);
		}
#endif
		return false;
	}
	//-------------------------------------------------------------------------------------------------------
	int SocketTCP::Receive( void * data, int size)
	{
		if ( m_nSocket != 0 )
		{
			int received_bytes = recv( m_nSocket, (char*)data, size, 0 );
			return received_bytes;
		}
#ifdef _DEBUG
		else
		{
			ASSERT(false);
		}
#endif
		return 0;
	}





	//////////////////////
	//		网络包		//
	//////////////////////
	//-------------------------------------------------------------------------------------------------------
	NetPackage::NetPackage()
		:m_Cursor(0)
	{
		m_vBuffer.resize( BUFFER_SIZE );
	}
	//-------------------------------------------------------------------------------------------------------
	//-------------------------------------------------------------------------------------------------------
	//-------------------------------------------------------------------------------------------------------

}