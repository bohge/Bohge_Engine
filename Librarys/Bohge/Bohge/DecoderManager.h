#pragma once
#include "Predefine.h"
#include "VariableType.h"


#include <string>
#include <map>

namespace BohgeEngine
{	
	class Decoder;
	class QueueThread;
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
			uint					m_nReference;
			Decoder*	m_pDecoder;
			DecoderReference( Decoder* ptr ):m_nReference(1),m_pDecoder(ptr){}
		};
	private:
		typedef std::map< uint, DecoderReference* >	DecoderReferenceMap;//共享的解码数据段
	private:
		QueueThread*						m_pDecodingQueueThread;//异步解码线程
		DecoderReferenceMap					m_DecoderMap;
	public:
		static BOHGE_FORCEINLINE DecoderManager* Instance() { return m_pInstance; };
		static void Create();
		static void Destroy();
	private:
		DecoderManager(void);
		~DecoderManager(void);
	public:
		void PushDecodeJob( Decoder* job );
		Decoder* LoadSoundDecoder( const std::string& path );
		void UnloadSoundDecoder( Decoder* sr );
	};
}
