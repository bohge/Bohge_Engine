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
#include "3DMath.h"
#include "DecoderManager.h"
#include "IJob.h"
#include <string>



namespace BohgeEngine
{
	class IFile;
	class Decoder : public IJob
	{
		friend class DecoderManager;
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
			uint			m_nNextPosition;//下一块数据的索引
			bool			m_isDone;
		public:
			BufferChunk( char* buffer, uint size, uint next, bool done )
				:m_pBuffer( buffer ),
				m_nSize( size ),
				m_nNextPosition( next ),
				m_isDone( done )
			{}
		public:
			BOHGE_FORCEINLINE bool isDone( ) const
			{
				return m_isDone;
			}
			BOHGE_FORCEINLINE const char* GetBuffer( ) const
			{
				return m_pBuffer;
			}
			BOHGE_FORCEINLINE uint GetSize( ) const
			{
				return m_nSize;
			}
			BOHGE_FORCEINLINE uint GetNextIndex( ) const
			{
				return m_nNextPosition;
			}
		};
	private:
		typedef std::vector<char*> BufferVector;
	private:
		int						m_nFrequency;//频率
		int						m_nChannels;//声道
		int						m_nSourceTotalSize;//声音pcm的总大小
		double					m_dTime;
		Format					m_nFormat;//格式
		uint					m_nLoadedBufferIndex;//已经加载过的数据位置
		std::string				m_FilePath;
		SoundType				m_eDecoderTypes;
		bool					m_isLoaded;
		bool			m_isActived;//资源是否可以进行解码，读取等操作，当调用ReleaseDecoder后设置为false
		bool					m_isRequested;//是否已经在队列中插入解码请求
		bool					m_isDecoding;//是否在解码之中
		IFile*					m_pFileStream;//io
		BufferVector			m_BufferVector;//资源池
	protected:
		Decoder( SoundType type );
	public:
		virtual ~Decoder(void);
	private:
		virtual void _DoInitialization( int& freq, Format& format, int& ch, int& buffersize, double& time ) = 0;
		virtual void _DoReleaseDecoder() = 0;//释放资源
		virtual uint _DoDecodeAsyn( uint form, uint to ) = 0;//异步解码,返回加载的数量
	private:
		bool _PrepareDecode();//准备解析数据段
	public:
		void LoadResource( const std::string& path );
		void ReleaseDecoder( );//释放io和buffer
		BufferChunk GetBufferChunk( int index );//得到声音数据,会自动触发下一次加载，这个get只有一次的有效性
	public:
		virtual void DoJob(SmartPtr<IJob>& self);//继承自IAsynJob
	private:
		BOHGE_FORCEINLINE uint _GetEndPosition( uint form )
		{
			return form + DecoderManager::DC_DEFUALT_SOUND_BUFFER_SIZE > m_nSourceTotalSize ? m_nSourceTotalSize : form + DecoderManager::DC_DEFUALT_SOUND_BUFFER_SIZE;
		}
		BOHGE_FORCEINLINE void _Deactive()//关闭decoder，准备释放
		{
			m_isActived = false;
		}
	protected:
		BOHGE_FORCEINLINE IFile* _GetFileSteam()
		{
			return m_pFileStream;
		}
		BOHGE_FORCEINLINE char* _GetInternalBuffer()
		{
			return m_BufferVector[m_nLoadedBufferIndex];
		}
	public:
		BOHGE_FORCEINLINE int isChunkReady( uint index ) const //数据段是否准备好
		{
			return index < m_nLoadedBufferIndex;
		}
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
		BOHGE_FORCEINLINE bool isRequested() const
		{
			return m_isRequested;
		}
		BOHGE_FORCEINLINE bool isDecoding() const
		{
			return m_isDecoding;
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
