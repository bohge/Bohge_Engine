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
//			类成员函数回调		   //
/////////////////////////////////////

#define NAME(a ,b )				NAME_T( a, b ) //这里使用代理的原应是#与##在宏展开的特殊性，当宏展开后如果遇到这两个符号，如果实参是宏，这个宏也不会继续展开了
#define NAME_T( a, b )			a##b
#define IFUNCTION_CLASS			NAME( IFunction, NAME_FIX )
#define FUNCBIND_CLASS			NAME( func_bind, NAME_FIX )
#define EVENT_CLASS				NAME( Event, NAME_FIX )


template <typename R, EVENT_PARAM_TEMPLATE>
class IFUNCTION_CLASS 	//函数指针的接口，提供Function调用的一个入口
{
public:
	virtual ~IFUNCTION_CLASS(){}
	virtual R Invoker(EVENT_PARAM_TYPES_INPUTS) = 0;
};

template <typename R,typename T, EVENT_PARAM_TEMPLATE >
class FUNCBIND_CLASS  : public IFUNCTION_CLASS<R, EVENT_PARAM_TYPES> //继承自IFunction借口，提供func调用
{
	typedef R (T::*fn)(EVENT_PARAM_TYPES);//定义一个rta的函数指针
	fn			m_func;//保存函数指针
	T* const	m_class;//保存这个类的指针
public:
	FUNCBIND_CLASS(fn f, T* const t)
		:m_func(f),
		m_class(t)
	{
	}
	FUNCBIND_CLASS(const FUNCBIND_CLASS& input)
		:m_func(input.m_func),
		m_class(input.m_class)
	{
	}
	BOHGE_FORCEINLINE FUNCBIND_CLASS& operator = (const FUNCBIND_CLASS& rhs)
	{
		m_func = rhs.m_func;
		m_class = rhs.m_class;
	}
	virtual R Invoker( EVENT_PARAM_TYPES_INPUTS ) //实现 IFunction函数接口，调用这个函数
	{
		return (m_class->*m_func)(EVENT_PARAM_INPUTS);
	}
};

template <typename R, typename T, EVENT_PARAM_TEMPLATE >
IFUNCTION_CLASS<R, EVENT_PARAM_TYPES>*  const make_bind( R (T::*fn)(EVENT_PARAM_TYPES), T* const t )	//制造一个函数成员指针类
{
	return dynamic_cast<IFUNCTION_CLASS<R, EVENT_PARAM_TYPES>* const>(NEW FUNCBIND_CLASS<R,T,EVENT_PARAM_TYPES>(fn,t));
};


template <typename R, EVENT_PARAM_TEMPLATE >
class EVENT_CLASS //多播事件
{
private:
	typedef std::map< int, IFUNCTION_CLASS<R,EVENT_PARAM_TYPES>* >		FunctionPtrMap;
	typedef typename FunctionPtrMap::iterator										FunctionIterator;
private:
	FunctionPtrMap			m_FuncMap;
private:
	EVENT_CLASS( const EVENT_CLASS<R, EVENT_PARAM_TYPES>& rhs ){}//Event 是不能拷贝的，指针的直接考本会带来问题
	EVENT_CLASS<R, EVENT_PARAM_TYPES>& operator = ( const EVENT_CLASS<R, EVENT_PARAM_TYPES>& rhs ){ return *this; }
public:
	EVENT_CLASS(){}
	~EVENT_CLASS()
	{
		this->ClearConnect();
	}
	BOHGE_FORCEINLINE FunctionPtr Connect(IFUNCTION_CLASS<R, EVENT_PARAM_TYPES>* const input)
	{
		m_FuncMap.insert( std::make_pair( (int)input, input ) );
		return input;
	}
	BOHGE_FORCEINLINE void Remove(FunctionPtr ptr)
	{
		FunctionIterator it = m_FuncMap.find( (int)ptr );
		if ( m_FuncMap.end() != it )
		{
			m_FuncMap.erase( it );
		}
	}
	BOHGE_FORCEINLINE void RemoveAll()
	{
		m_FuncMap.clear();
	}
	BOHGE_FORCEINLINE void ClearConnect()
	{
		for(FunctionIterator it = m_FuncMap.begin();
			it != m_FuncMap.end();
			it++)
		{
			SAFE_DELETE( it->second );
		}
		m_FuncMap.clear();
	}
	BOHGE_FORCEINLINE void Multicast( EVENT_PARAM_TYPES_INPUTS ) //多播不支持返回数值
	{
		for(FunctionIterator it = m_FuncMap.begin();
			it != m_FuncMap.end();
			)
		{
			FunctionIterator temp = it;//这么做为了在callback的时候删除自己的callback
			it ++;
			temp->second->Invoker(EVENT_PARAM_INPUTS);
		}
	}
	BOHGE_FORCEINLINE R Unicast(void* to, EVENT_PARAM_TYPES_INPUTS )//单播
	{
		FunctionIterator it = m_FuncMap.find( (int)to );
		if ( m_FuncMap.end() != it )
		{
			it->second->Invoker(EVENT_PARAM_INPUTS);
		}
	}
};

#undef NAME
#undef NAME_T
#undef IFUNCTION_CLASS
#undef FUNCBIND_CLASS
#undef EVENT_CLASS
#undef NAME_FIX
#undef EVENT_PARAM_TEMPLATE
#undef EVENT_PARAM_TYPES_INPUTS
#undef EVENT_PARAM_TYPES
#undef EVENT_PARAM_INPUTS
