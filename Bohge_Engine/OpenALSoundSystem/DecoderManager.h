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
#include "Framework/IIOSystem.h"
#include "Framework/IJobDispatcher.h"
#include "Framework/VariableType.h"
#include "Framework/SmartPtr.hpp"
#include "Framework/Predefine.h"



#include <EASTL/string.h>
#include <EASTL/map.h>
#include <EASTL/list.h>


namespace OpenALSoundSystemPlugin
{	
	class Decoder;
	class DecoderManager
	{
	public:
		enum Constant
		{
			DC_DEFUALT_SOUND_BUFFER_SIZE	= 1024 * 8,
		};
	private:
		static DecoderManager*		m_pInstance;
	private:
		struct DecoderReference
		{
			uint										m_nReference;
			BohgeEngine::SmartPtr<Decoder>				m_pDecoder;
			DecoderReference( BohgeEngine::SmartPtr<Decoder> ptr ):m_nReference(1),m_pDecoder(ptr){}
		};
	private:
		typedef eastl::map< uint, DecoderReference* >	DecoderReferenceMap;//共享的解码数据段
	private:
		BohgeEngine::IJobDispatcher*		m_DecodJobThread;//异步解码线程
		DecoderReferenceMap					m_DecoderMap;
	public:
		static BOHGE_FORCEINLINE DecoderManager* Instance() { return m_pInstance; };
		static void Create( BohgeEngine::Framework& framework );
		static void Destroy( BohgeEngine::Framework& framework );
	private:
		DecoderManager(void);
		~DecoderManager(void);
	private:
		BohgeEngine::SmartPtr<Decoder> _DecoderFactory( const eastl::string& path );
	public:
		void PushDecodeJob( BohgeEngine::SmartPtr<Decoder>& job );
		BohgeEngine::SmartPtr<Decoder> LoadSoundDecoder( const eastl::string& path );
		void UnloadSoundDecoder( BohgeEngine::SmartPtr<Decoder>& sr );
		void Update();
	};
}
