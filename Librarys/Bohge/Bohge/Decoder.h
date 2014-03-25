#pragma once
#include "3DMath.h"
#include "DecoderManager.h"
#include "IAsynJob.h"
#include <string>



namespace BohgeEngine
{
	class IReadFile;
	class Decoder : public IAsynJob
	{
	public:
		enum Format
		{
			DF_MONO_8 = 0,
			DF_STEREO_8,
			DF_MONO_16,
			DF_STEREO_16,
		};
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
			char*			m_pBuffer;//数据内存块
			uint			m_nSize;//数据块大小
			uint			m_nNextPosition;//下一块数据的起始地点
		public:
			BufferChunk( char* buffer, uint size, uint next )
				:m_pBuffer( buffer ),
				m_nSize( size ),
				m_nNextPosition( next )
			{}
		public:
			BOHGE_FORCEINLINE bool isValid( ) const
			{
				return NULL != m_pBuffer;
			}
			BOHGE_FORCEINLINE const char* GetBuffer( ) const
			{
				return m_pBuffer;
			}
			BOHGE_FORCEINLINE uint GetSize( ) const
			{
				return m_nSize;
			}
			BOHGE_FORCEINLINE uint GetNextPosition( ) const
			{
				return m_nNextPosition;
			}
		};
	private:
		int				m_nFrequency;//频率
		int				m_nChannels;//声道
		int				m_nSourceTotalSize;//声音pcm的总大小
		double			m_dTime;
		Format			m_nFormat;//格式
		vector2d		m_FromTo;//当前解压数据段
		std::string		m_FilePath;
		SoundType		m_eDecoderTypes;
		bool			m_isDecodeing;//是否正在解析中
		bool			m_isStorageBuffer;//是否保存已经解码的buffer（多个SoundResource需要这个资源，或者是loop音乐的时候，这个值为true，避免重复解压，空间换时间）
		bool			m_isActived;//资源是否可以进行解码，读取等操作，当调用ReleaseDecoder后设置为false
		IReadFile*		m_pFileStream;//io
		char*			m_pBuffer;//资源池
	public:
		static Decoder* DecoderFactory( const std::string& path );
		static void DestoryDecoder( Decoder* decoder );
	protected:
		Decoder( SoundType type );
		virtual ~Decoder(void);
	private:
		virtual void _DoInitialization( int& freq, Format& format, int& ch, int& buffersize, double& time ) = 0;
		virtual void _DoReleaseDecoder() = 0;//释放资源
		virtual void _DoDecodeAsyn( uint form, uint to ) = 0;//异步解码
	private:
		void _RequestDecode( uint form, uint to );//要求解析数据段from to
	public:
		void LoadResource( const std::string& path );
		void ReleaseDecoder( );//释放io和buffer
		void StorageBuffer( );//声音默认使用流式加载方式，当音源循环播放或者有多个使用者的时候，切换到此模式，用内存换时间
		BufferChunk GetBufferChunk( int pos );//得到声音数据,会自动触发下一次加载，这个get只有一次的有效性
		void RequestDecode( uint begin );
	public:
		virtual void AsyncDoJob();//继承自IAsynJob
	private:
		BOHGE_FORCEINLINE uint _GetEndPosition( uint form )
		{
			return form + DecoderManager::DC_DEFUALT_SOUND_BUFFER_SIZE > m_nSourceTotalSize ? m_nSourceTotalSize : form + DecoderManager::DC_DEFUALT_SOUND_BUFFER_SIZE;
		}
	protected:
		BOHGE_FORCEINLINE IReadFile* _GetFileSteam()
		{
			return m_pFileStream;
		}
		BOHGE_FORCEINLINE char* _GetInternalBuffer()
		{
			return m_pBuffer;
		}
	public:
		BOHGE_FORCEINLINE int GetFrequency() const
		{
			return m_nFrequency;
		}
		BOHGE_FORCEINLINE Format GetFormat() const
		{
			return m_nFormat;
		}
		BOHGE_FORCEINLINE int GetChannels() const
		{
			return m_nChannels;
		}
		BOHGE_FORCEINLINE int GetSourceSize() const
		{
			return m_nSourceTotalSize;
		}
		BOHGE_FORCEINLINE bool isActived() const
		{
			return m_isActived;
		}
		BOHGE_FORCEINLINE bool isDecoding() const
		{
			return m_isDecodeing;
		}
		BOHGE_FORCEINLINE SoundType GetDecoderType() const
		{
			return m_eDecoderTypes;
		}
		BOHGE_FORCEINLINE const std::string& GetFilePath() const
		{
			return m_FilePath;
		}
	};
}
