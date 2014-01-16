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

#include <stdexcept>
#include <assert.h>

//#define TEST_SMARTPTR

template <typename T>
class SmartPtr
{
private:
	bool		m_isRefence;
	T			*ptr;
	size_t		*pUse;
public:
	SmartPtr()
		:ptr(NULL),
		pUse(NULL)
	{

	}
	explicit SmartPtr(T *p )
		: ptr(p),
		pUse(new size_t(1)),
		m_isRefence(true)
	{ 
	}
	SmartPtr(const SmartPtr& src)
		: ptr(src.ptr),
		pUse(src.pUse),
		m_isRefence(true)
	{
		++*pUse;
	}
	~SmartPtr()
	{
		if( m_isRefence )
		{
			decrUse();
		}
#ifdef TEST_SMARTPTR
		std::cout<<"SmartPtr: Destructor"<<std::endl; // for testing
#endif
	}
	inline T *operator->() 
	{
#ifdef _DEBUG
		if (ptr)
			return ptr;
		throw std::runtime_error("access through NULL pointer");
#endif
		return ptr;
	}
	inline T &operator*() 
	{
#ifdef _DEBUG
		if (ptr)
			return *ptr;
		throw std::runtime_error("access through NULL pointer");
#endif
		return *ptr;
	}
	inline const T *operator->() const 
	{
#ifdef _DEBUG
		if (ptr)
			return ptr;
		throw std::runtime_error("access through NULL pointer");
#endif
		return ptr;
	}
	inline const T &operator*() const
	{
#ifdef _DEBUG
		if (ptr)
			return *ptr;
		throw std::runtime_error("access through NULL pointer");
#endif
		return *ptr;
	}
	inline SmartPtr& operator= (const SmartPtr& rhs)
	{
		// self-assigning is also right
		if( m_isRefence )
		{
			decrUse();
		}
		pUse = rhs.pUse;
		if( NULL != pUse )
		{
			++*pUse;
		}
		m_isRefence = true;
		ptr = rhs.ptr;
		return *this;
	}
	inline SmartPtr& operator= ( int p )
	{
		// self-assigning is also right
		if( m_isRefence )
		{
			decrUse();
		}
		m_isRefence = false;
		ptr = NULL;
		pUse = NULL;
		return *this;
	}
	inline bool operator== (const SmartPtr& rhs) const
	{
		return ptr == rhs.ptr;
	}
	inline bool operator== ( int p) const
	{
		return (int)ptr == p;
	}
	inline bool operator!= (const SmartPtr& rhs) const
	{
		return ptr != rhs.ptr;
	}
	inline bool operator!= ( int p) const
	{
		return (int)ptr != p;
	}
	inline operator int()
	{
		return (int) ptr;
	}
	inline void Copy( const SmartPtr<T>& rhs )
	{
		if( m_isRefence )
		{
			decrUse();
		}
		m_isRefence = false;
		ptr = rhs.ptr;
		pUse = rhs.pUse;
	}
private:
	inline void decrUse()
	{
		if( NULL != pUse )
		{
			if (--*pUse == 0) 
			{
				SAFE_DELETE( ptr );
				SAFE_DELETE( pUse );
			}
		}
	}
private:
	template <typename To, typename From>
	friend inline SmartPtr<To> check_pointer_cast( SmartPtr<From> const & p); //cast会增加引用计数
private:
	explicit SmartPtr( T *p, size_t* refptr)
		: ptr( p ),
		pUse(refptr),
		m_isRefence(true)
	{
		if( NULL != pUse )
		{
			++*pUse;
		}
	}
};

template <typename To, typename From>
inline SmartPtr<To>
check_pointer_cast( SmartPtr<From> const & p)
{
#ifdef _DEBUG
	assert( dynamic_cast<To*>(p.ptr) != NULL );
#endif
	return SmartPtr<To>( static_cast<To*>(p.ptr), p.pUse );
}