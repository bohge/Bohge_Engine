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
#include "NodeProperty.h"



namespace BohgeEngine
{
	class IComponent;
	class BOHGE_FRAMEWORK NodeMessage
	{
	private:
		const IComponent*				m_Sender;
		const Object*					m_MessageData;
		NodeProperty::SetAttribute		m_MessageType;
	public:
		explicit NodeMessage( const IComponent* sender, const Object* data, NodeProperty::SetAttribute type );
		~NodeMessage();
	public:
		BOHGE_FORCEINLINE bool isValidMessage( const IComponent* ptr ) const//不是自己发给自己的就是有效信息
		{
			//return m_Sender->isType( ptr->RTTI() );
			return m_Sender != ptr;
		}
		BOHGE_FORCEINLINE NodeProperty::SetAttribute GetMessageType() const
		{
			return m_MessageType;
		}
		template<typename T> const T& GetData() const
		{
			return *static_cast<const T*>(m_MessageData);
		}
	};
}