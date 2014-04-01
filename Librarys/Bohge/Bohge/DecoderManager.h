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
#include "Predefine.h"
#include "VariableType.h"


#include <string>
#include <map>
#include <list>

namespace BohgeEngine
{	
	class Decoder;
	class LessThread;
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
			uint			m_nReference;
			Decoder*		m_pDecoder;
			DecoderReference( Decoder* ptr ):m_nReference(1),m_pDecoder(ptr){}
		};
	private:
		typedef std::map< uint, DecoderReference* >	DecoderReferenceMap;//共享的解码数据段
		typedef std::list< Decoder* > DecoderTrashList; 
	private:
		LessThread*							m_pDecodingLessThread;//异步解码线程
		DecoderReferenceMap					m_DecoderMap;
		DecoderTrashList					m_DecoderTrashList;
	public:
		static BOHGE_FORCEINLINE DecoderManager* Instance() { return m_pInstance; };
		static void Create();
		static void Destroy();
	private:
		DecoderManager(void);
		~DecoderManager(void);
	private:
		Decoder* _DecoderFactory( const std::string& path );
		void _DestoryDecoder( Decoder* decoder );
	public:
		void PushDecodeJob( Decoder* job );
		Decoder* LoadSoundDecoder( const std::string& path );
		void UnloadSoundDecoder( Decoder* sr );
		void Update();
	};
}
