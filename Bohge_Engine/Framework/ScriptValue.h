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
#include "Object.hpp"



namespace BohgeEngine
{
	class BOHGE_FRAMEWORK ScriptValue// : public Object
	{
		//RTTI_TYPE_DECLEAR( ScriptValue )
	typedef void(*ClearFunction)( Object* rec, Object* obj );
	private:
		Object*			m_pValue;
		Object*			m_rpRecycle;
		ClearFunction	m_fClear;
	public:
		ScriptValue(void);
		~ScriptValue(void);
	public:
		static void SafeDeleteCleaner( Object* rec, Object* obj );//最简单的delete清除器
		static void NullCleaner( Object* rec, Object* obj );//空清除器，不清除
	public:
		BOHGE_FORCEINLINE void SetValue( Object* val, ClearFunction f )
		{
			m_pValue = val;
			m_fClear = f;
		}
		BOHGE_FORCEINLINE void SetValue( const Object* val, ClearFunction f )
		{
			m_pValue = const_cast<Object*>(val);
			m_fClear = f;
		}
		BOHGE_FORCEINLINE void SetValue( Object* val, Object* rec, ClearFunction f )
		{
			m_pValue = val;
			m_rpRecycle = rec;
			m_fClear = f;
		}
		BOHGE_FORCEINLINE void SetValue( const Object* val, Object* rec, ClearFunction f )
		{
			m_pValue = const_cast<Object*>(val);
			m_rpRecycle = rec;
			m_fClear = f;
		}
		BOHGE_FORCEINLINE Object* GetObject()
		{
			return m_pValue;
		}
		BOHGE_FORCEINLINE const Object* GetObject() const
		{
			return m_pValue;
		}
	};
}