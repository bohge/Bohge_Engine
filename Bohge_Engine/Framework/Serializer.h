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
#include "Plugindefine.h"
#include "IIOSystem.h"
#include "3DMath.h"
#include "IFile.h"

#include <EASTL/string.h>


namespace BohgeEngine
{
	class BOHGE_FRAMEWORK Serializer
	{
	public:
	private:
		IFile::ActionType		m_eAction;
		eastl::string				m_Url;
		IFile*					m_FilePtr;
		uint					m_Cursor;
	public:
		Serializer( eastl::string filename );
		~Serializer();
	public:
		BOHGE_FORCEINLINE void Open( IFile::ActionType at )
		{
			ASSERT ( NULL == m_FilePtr );
			m_eAction = at;
			m_FilePtr = IIOSystem::Instance()->FileFactory( m_Url );
			m_FilePtr->OpenFile( m_eAction );
		}
		BOHGE_FORCEINLINE bool isOpen()
		{
			return m_FilePtr->isOpen();
		}
	public:
		BOHGE_FORCEINLINE int Read(int bytes, void * pBuffer)
		{
			if ( NULL == m_FilePtr )
			{
				m_eAction = IFile::AT_READ;
				m_FilePtr = IIOSystem::Instance()->FileFactory( m_Url );
				m_FilePtr->OpenFile( m_eAction );
			}
			m_Cursor+=bytes;
			ASSERT( IFile::AT_READ == m_eAction );
			return m_FilePtr->ReadFile( pBuffer, bytes );
		}
		BOHGE_FORCEINLINE int Write(int bytes, void * pBuffer)
		{
			if ( NULL == m_FilePtr )
			{
				m_eAction = IFile::AT_WRITE;
				m_FilePtr = IIOSystem::Instance()->FileFactory( m_Url );
				m_FilePtr->OpenFile( m_eAction );
			}
			m_Cursor+=bytes;
			ASSERT( IFile::AT_WRITE == m_eAction );
			return m_FilePtr->WriteFile( pBuffer, bytes );
		}

		//这两个函数太危险了，还是不用了，比如当class增加类成员的时候
		//template<typename T> BOHGE_FORCEINLINE void WriteClass( const T& c )//只是bite级别的拷贝写入，注意string，vector，指针
		//{
		//	this->Write( sizeof(T), &c );
		//}
		//template<typename T> BOHGE_FORCEINLINE void ReadClass( T& c )
		//{
		//	this->Read( sizeof(T), &c );
		//}
	public:
		//写入
		BOHGE_FORCEINLINE Serializer& operator<< (const eastl::string& n)
		{
			int size = n.size() + 1; // +1是终止符号
			this->Write(sizeof(int), &size);
			for(int i = 0 ; i < size ; i ++ )
			{
				*this<<n[i];
			}
			return *this;
		}
		BOHGE_FORCEINLINE Serializer& operator<< (bool n)
		{
			this->Write(sizeof(bool), &n);
			return *this;
		}
		BOHGE_FORCEINLINE Serializer& operator<< (unsigned char n)
		{
			this->Write(sizeof(unsigned char), &n);
			return *this;
		}
		BOHGE_FORCEINLINE Serializer& operator<< (char n)
		{
			this->Write(sizeof(char), &n);
			return *this;
		}
		BOHGE_FORCEINLINE Serializer& operator<< (int n)
		{
			this->Write(sizeof(int), &n);
			return *this;
		}
		BOHGE_FORCEINLINE Serializer& operator<< (unsigned int n)
		{
			this->Write(sizeof(unsigned int), &n);
			return *this;
		}
		BOHGE_FORCEINLINE Serializer& operator<< (short n)
		{
			this->Write(sizeof(short), &n);
			return *this;
		}
		BOHGE_FORCEINLINE Serializer& operator<< (unsigned short n)
		{
			this->Write(sizeof(unsigned short), &n);
			return *this;
		}
		BOHGE_FORCEINLINE Serializer& operator<< (float n)
		{
			this->Write(sizeof(float), &n);
			return *this;
		}
		BOHGE_FORCEINLINE Serializer& operator<< (double n)
		{
			this->Write(sizeof(double), &n);
			return *this;
		}
		BOHGE_FORCEINLINE Serializer& operator<< (long long n)
		{
			this->Write(sizeof(long long), &n);
			return *this;
		}
		BOHGE_FORCEINLINE Serializer& operator<< (unsigned long long n)
		{
			this->Write(sizeof(unsigned long long), &n);
			return *this;
		}
		template<typename T> BOHGE_FORCEINLINE Serializer& operator<< ( const vector2<T>& v)
		{
			return *this<<v.m_x<<v.m_y;
		}
		template<typename T> BOHGE_FORCEINLINE Serializer& operator<< ( const vector3<T>& v)
		{
			return *this<<v.m_x<<v.m_y<<v.m_z;
		}
		template<typename T> BOHGE_FORCEINLINE Serializer& operator<< ( const vector4<T>& v)
		{
			return *this<<v.m_x<<v.m_y<<v.m_z<<v.m_w;
		}
		template<typename T> BOHGE_FORCEINLINE Serializer& operator<< ( const Quaternion<T>& v)
		{
			return *this<<v.m_x<<v.m_y<<v.m_z<<v.m_w;
		}
		//读取
		BOHGE_FORCEINLINE Serializer& operator>> (eastl::string& n)
		{
			int num;
			*this>>num;
			char* str = NEW char[num];
			for( int i = 0 ; i < num ; i ++ )
			{
				this->Read(sizeof(char), &str[i]);
			}
			n = eastl::string(str);
			SAFE_DELETE_ARRAY(str);
			return *this;
		}
		BOHGE_FORCEINLINE Serializer& operator>> (bool& n)
		{
			this->Read(sizeof(bool), &n);
			return *this;
		}
		BOHGE_FORCEINLINE Serializer& operator>> (unsigned char& n)
		{
			this->Read(sizeof( unsigned char), &n);
			return *this;
		}
		BOHGE_FORCEINLINE Serializer& operator>> (char& n)
		{
			this->Read(sizeof(char), &n);
			return *this;
		}
		BOHGE_FORCEINLINE Serializer& operator>> (short& n)
		{
			this->Read(sizeof(short), &n);
			return *this;
		}
		BOHGE_FORCEINLINE Serializer& operator>> (unsigned short& n)
		{
			this->Read(sizeof(unsigned short), &n);
			return *this;
		}
		BOHGE_FORCEINLINE Serializer& operator>> (int& n)
		{
			this->Read(sizeof(int), &n);
			return *this;
		}
		BOHGE_FORCEINLINE Serializer& operator>> (unsigned int& n)
		{
			this->Read(sizeof(unsigned int), &n);
			return *this;
		}
		BOHGE_FORCEINLINE Serializer& operator>> (float& n)
		{
			this->Read(sizeof(float), &n);
			return *this;
		}
		BOHGE_FORCEINLINE Serializer& operator>> (double& n)
		{
			this->Read(sizeof(double), &n);
			return *this;
		}
		BOHGE_FORCEINLINE Serializer& operator>> (long long& n)
		{
			this->Read(sizeof(long long), &n);
			return *this;
		}
		BOHGE_FORCEINLINE Serializer& operator>> (unsigned long long& n)
		{
			this->Read(sizeof(unsigned long long), &n);
			return *this;
		}
		template<typename T> BOHGE_FORCEINLINE Serializer& operator>> ( vector2<T>& v)
		{
			return *this>>v.m_x>>v.m_y;
		}
		template<typename T> BOHGE_FORCEINLINE Serializer& operator>> ( vector3<T>& v)
		{
			return *this>>v.m_x>>v.m_y>>v.m_z;
		}
		template<typename T> BOHGE_FORCEINLINE Serializer& operator>> ( vector4<T>& v)
		{
			return *this>>v.m_x>>v.m_y>>v.m_z>>v.m_w;
		}
		template<typename T> BOHGE_FORCEINLINE Serializer& operator>> ( Quaternion<T>& v)
		{
			return *this>>v.m_x>>v.m_y>>v.m_z>>v.m_w;
		}

	public:
		BOHGE_FORCEINLINE bool isEnd()
		{
			return IFile::AT_WRITE == m_eAction ? false : m_Cursor >= m_FilePtr->GetSize();
		}
		BOHGE_FORCEINLINE bool IsStoring()
		{
			return m_eAction == IFile::AT_WRITE;
		}
	};

}