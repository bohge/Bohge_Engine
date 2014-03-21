#pragma once
#include "VariableType.h"
#include "SoundResourceManager.h"
#include <string>



namespace BohgeEngine
{
	class IReadFile;
	class SoundResourceDecoder
	{
	protected:
		enum SoundType
		{
			ST_OGG		= 0,
			ST_MP3		= 1,
			ST_WAV		= 2,
		};
	public:
		class BufferChunk
		{
		private:
			byte*			m_pBuffer;//数据内存块
			uint		m_nSize;//数据块大小
		public:
			BufferChunk( byte* buffer, uint size)
				:m_pBuffer( buffer ),
				m_nSize( size )
			{}
		public:
			inline bool isValid( ) const
			{
				return NULL != m_pBuffer;
			}
			inline const byte* GetBuffer( ) const
			{
				return m_pBuffer;
			}
			inline uint GetSize( ) const
			{
				return m_nSize;
			}
		};
	private:
		unsigned long			m_nFrequency;//频率
		unsigned long			m_nFormat;//格式
		unsigned long			m_nChannels;//声道
		unsigned long			m_nBufferSize;//声音buffer的总大小
		SoundType				m_eDecoderTypes;
		bool					m_isDecodeing;//是否正在解析中
		bool					m_isStorageBuffer;//是否保存已经解码的buffer（多个SoundResource需要这个资源，或者是loop音乐的时候，这个值为true，避免重复解压，空间换时间）
		bool					m_isActived;//资源是否可以进行解码，读取等操作，当调用ReleaseDecoder后设置为false
		IReadFile*				m_pFileStream;//io
		byte*					m_pBuffer;//资源池
	public:
		static SoundResourceDecoder* SoundResourceDecoderFactory( const std::string& path );
	protected:
		SoundResourceDecoder( IReadFile* stream, SoundType type );
		virtual ~SoundResourceDecoder(void);
	private:
		void _RequestDecode( uint form, uint to );//要求解析数据段from to
	public:
		void ReleaseDecoder( );//释放io和buffer
		void StorageBuffer( );//声音默认使用流式加载方式，当音源循环播放或者有多个使用者的时候，切换到此模式，用内存换时间
		BufferChunk GetBufferChunk( int pos );//得到声音数据
		void OnEndDecode( uint form, uint to );//解压完成后,buffer需要拷贝一次
	public:
		virtual void DoDecodeAsyn( uint form, uint to ) = 0;//异步解码
	private:
		inline uint _GetEndPosition( uint form )
		{
			return form + SoundResourceManager::SC_DEFUALT_SOUND_BUFFER_SIZE > m_nBufferSize ? m_nBufferSize : form + SoundResourceManager::SC_DEFUALT_SOUND_BUFFER_SIZE;
		}
	protected:
		inline IReadFile* _GetFileSteam()
		{
			return m_pFileStream;
		}
	public:
		inline unsigned long GetFrequency()
		{
			return m_nFrequency;
		}
		inline unsigned long GetFormat()
		{
			return m_nFormat;
		}
		inline unsigned long GetChannels()
		{
			return m_nChannels;
		}
		inline unsigned long GetBufferSize()
		{
			return m_nBufferSize;
		}
		inline bool isActived()
		{
			return m_isActived;
		}
		inline SoundType GetDecoderType()
		{
			return m_eDecoderTypes;
		}
	};
}
