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
#include "TextureSource.h"
#include "Utility.h"
#include "IOSystem.h"
#include "IFile.h"
#include "TextureResource.h"

#include "png.h"
#include "pnginfo.h"
#include "pngstruct.h"

using namespace std;
using namespace BohgeEngine::TextureStatus;

namespace BohgeEngine
{
	//------------------------------------------------------------------------------------------------------
	TextureSource::TextureSource( void )
		:ISource( ISource::ST_TEXTURE ),
		m_Anisotropic( 0 ),
		m_eTextureType( TextureStatus::TT_TEXTURE1D ),
		m_eFormat( TextureStatus::PF_A8 ),
		m_isLoaded(false)
	{
	}
	//------------------------------------------------------------------------------------------------------
	TextureSource::~TextureSource(void)
	{
		_ReleaseMemory();
	}
	//------------------------------------------------------------------------------------------------------
	void TextureSource::_ReleaseMemory()
	{
		FOR_EACH( TextrueSourceDataVector, it, m_DataVector )
		{
			SAFE_DELETE( *it );
		}
		m_DataVector.clear();
	}
	//------------------------------------------------------------------------------------------------------
	uint TextureSource::_GetResourceHashCode() const
	{
		const vector< string >& vecString = GetResourcePaths();
		string str;
		FOR_EACH_CONST( vector< string >, it, vecString )
		{
			str += *it;
		}
		str.push_back( m_Warp.m_SWarp + 1 );
		str.push_back( m_Warp.m_TWarp + 1 );
		str.push_back( m_Filter.m_MagFilter + 1 );
		str.push_back( m_Filter.m_MinFilter + 1 );
		str.push_back( m_eFormat + 1 );
		str.push_back( m_Anisotropic + 1 );
		str.push_back(0);
		return Utility::HashCode( str );
	}
	//------------------------------------------------------------------------------------------------------
	TextrueSourceData* TextureSource::_LoadTextureSource( const std::string& path )
	{
		if( path.find(".tga") != -1 || path.find(".TGA") != -1 )
		{
			return _LoadTga(path);
		}
		else if( path.find(".png") != -1 || path.find(".PNG") != -1 )
		{
			return _LoadPng(path);
		}
		return NULL;
	}
	//------------------------------------------------------------------------------------------------------
	TextrueSourceData* TextureSource::_LoadTga( std::string filename )
	{
		struct TARGA_HEADER
		{
			//字头的解释
			//http://blog.csdn.net/guanghua2009/article/details/6641665
			unsigned char   IDLength, ColormapType, ImageType;
			unsigned char   ColormapSpecification[5];
			unsigned short	XOrigin, YOrigin;
			unsigned short	ImageWidth, ImageHeight;
			unsigned char   PixelDepth;
			unsigned char   ImageDescriptor;
		};
		TARGA_HEADER header;

		byte* pBits;
		uint nPixelSize;
		IFile* readfile;
		readfile = FILEFACTORY( filename );
		if( !readfile->OpenFile( IFile::AT_READ ) )
		{
			return NULL;
		}

		readfile->ReadFile( &header, sizeof(header) );
		nPixelSize = header.PixelDepth >> 3;


		pBits = NEW byte[ nPixelSize * header.ImageWidth * header.ImageHeight ];//这里只分配，析构的时候会释放掉资源的

		if( 2 == header.ImageType )//非压缩文理
		{
			readfile->ReadFile( pBits, nPixelSize * header.ImageWidth * header.ImageHeight );
		}
		else//读取压缩纹理
		{
			uint currentpixel	= 0;												// Current pixel being read
			uint currentbyte	= 0;												// Current byte 
			uint line			= header.ImageHeight - 1;
			byte* colorbuffer = NEW byte[nPixelSize];//保存一个像素颜色的值
			do
			{
				byte chunkheader = 0; //chunk头
				readfile->ReadFile( &chunkheader, sizeof(byte) );
				if(chunkheader < 128)												// If the ehader is < 128, it means the that is the number of RAW color packets minus 1
				{																	// that follow the header
					chunkheader++;													// add 1 to get number of following color values
					for(short counter = 0; counter < chunkheader; counter++)		// Read RAW color values
					{
						//if(fread(colorbuffer, 1, tga.bytesPerPixel, fTGA) != tga.bytesPerPixel) // Try to read 1 pixel
						readfile->ReadFile( colorbuffer, sizeof(byte) * nPixelSize );
						// write to memory
						pBits[currentbyte		] = colorbuffer[2];				    // Flip R and B vcolor values around in the process 
						pBits[currentbyte + 1	] = colorbuffer[1];
						pBits[currentbyte + 2	] = colorbuffer[0];

						if( 4 == nPixelSize )												// if its a 32 bpp image
						{
							pBits[currentbyte + 3] = colorbuffer[3];				// copy the 4th byte
						}

						currentbyte += nPixelSize;										// Increase thecurrent byte by the number of bytes per pixel
						currentpixel++;
						if( header.ImageWidth == currentpixel )
						{
							currentpixel = 0;
							line--;
							currentbyte = line * nPixelSize * header.ImageWidth;
						}
					}
				}
				else																			// chunkheader > 128 RLE data, next color reapeated chunkheader - 127 times
				{
					chunkheader -= 127;															// Subteact 127 to get rid of the ID bit
					//if(fread(colorbuffer, 1, tga.bytesPerPixel, fTGA) != tga.bytesPerPixel)		// Attempt to read following color values
					readfile->ReadFile( colorbuffer, sizeof(byte) * nPixelSize );
					for(short counter = 0; counter < chunkheader; counter++)					// copy the color into the image data as many times as dictated 
					{																			// by the header
						pBits[currentbyte		] = colorbuffer[2];					// switch R and B bytes areound while copying
						pBits[currentbyte + 1	] = colorbuffer[1];
						pBits[currentbyte + 2	] = colorbuffer[0];

						if(4 == nPixelSize)												// If TGA images is 32 bpp
						{
							pBits[currentbyte + 3] = colorbuffer[3];				// Copy 4th byte
						}

						currentbyte += nPixelSize;										// Increase current byte by the number of bytes per pixel
						currentpixel++;															// Increase pixel count by 1
						if( header.ImageWidth == currentpixel )
						{
							currentpixel = 0;
							line--;
							currentbyte = line * nPixelSize * header.ImageWidth;
						}
					}
				}
			}
			while ( 0 < line );
			SAFE_DELETE_ARRAY( colorbuffer );
		}
		readfile->CloseFile();
		FILEDESTROY( readfile );

		// Convert the image from BGRA to RGBA
		if( 2 == header.ImageType )
		{
			byte* p = pBits;
			for( int y=0; y<header.ImageHeight; y++ )
			{
				for( int x=0; x<header.ImageWidth; x++ )
				{
					byte temp = p[2]; p[2] = p[0]; p[0] = temp;
					p += nPixelSize;
				}
			}
		}
		//byte* p = pBits;//y轴反向
		//for( int y=0; y<header.ImageHeight/2; y++ )
		//{
		//	static vector<byte> swaper;
		//	if ( swaper.size() < header.ImageWidth*nPixelSize )
		//	{
		//		swaper.resize( header.ImageWidth*nPixelSize );
		//	}
		//	memcpy( &swaper[0], p + y*header.ImageWidth*nPixelSize, header.ImageWidth*nPixelSize );
		//	memcpy( p + y*header.ImageWidth*nPixelSize, p+(header.ImageHeight-y-1)*header.ImageWidth*nPixelSize, header.ImageWidth*nPixelSize );
		//	memcpy( p + (header.ImageHeight-y-1)*header.ImageWidth*nPixelSize, &swaper[0], header.ImageWidth*nPixelSize );
		//}
		return NEW TextrueSourceData( vector2d(header.ImageWidth, header.ImageHeight), pBits);
	}
	//------------------------------------------------------------------------------------------------------
	static void png_rw(png_structp png_ptr, png_bytep data, png_size_t length) 
	{
		IFile* file = static_cast<IFile*>(png_ptr->io_ptr);
		file->ReadFile(data, length);
		//File::Instance().ReadFile(data, length, 1, file);
	}
	TextrueSourceData* TextureSource::_LoadPng( std::string filename )
	{
		png_structp PngPtr;
		png_infop InfoPtr;
		IFile* readfile;


		readfile = FILEFACTORY( filename );
		readfile->OpenFile( IFile::AT_READ );

		/* initialise structures for reading a png file */
		PngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
		InfoPtr = png_create_info_struct(PngPtr);

		setjmp(png_jmpbuf(PngPtr));

		//相同代码
		png_set_read_fn(PngPtr, readfile, png_rw);//调用回调函数读取数据
		png_read_png(PngPtr, InfoPtr, PNG_TRANSFORM_EXPAND, 0);

		int width = png_get_image_width(PngPtr, InfoPtr);
		int height = png_get_image_height(PngPtr, InfoPtr);

		int color_type = png_get_color_type(PngPtr, InfoPtr);

		int channel = png_get_channels(PngPtr, InfoPtr);

		{
			int bytedepth = png_get_bit_depth( PngPtr, InfoPtr );
			ASSERT( 8 == bytedepth );
		}

		//分辨是三通道还是四通道读取
		int size = height * width * channel;
		byte* data = NEW byte[size];
		png_bytep* row_pointers = png_get_rows(PngPtr, InfoPtr);
		memcpy( data, row_pointers, size );
		// 拷贝！！注意，如果你读取的png没有A通道，就要3位3位的读。还有就是注意字节对其的问题，最简单的就是别用不能被4整除的宽度就行了。读过你实在想用，就要在这里加上相关的对齐处理。
		//switch ( channel )
		//{
		//case 1://L
		//	{
		//		int pos = (width * height * 1) - (1 * width);
		//		for(int row = 0; row < height; row++)
		//		{
		//			for(int col = 0; col < (1 * width); col += 1)
		//			{
		//				data[pos++] = row_pointers[row][col];        //l
		//			}
		//			pos=(pos - (width * 1)*2); //move the pointer back two rows
		//		}
		//	}
		//	break;
		//case 3://rgb
		//	{
		//		int pos = (width * height * 3) - (3 * width);
		//		for(int row = 0; row < height; row++)
		//		{
		//			for(int col = 0; col < (3 * width); col += 3)
		//			{
		//				data[pos++] = row_pointers[row][col];        // red
		//				data[pos++] = row_pointers[row][col + 1]; // green
		//				data[pos++] = row_pointers[row][col + 2]; // blue
		//			}
		//			pos=(pos - (width * 3)*2); //move the pointer back two rows
		//		}
		//	}
		//	break;
		//case 4://rgba
		//	{
		//		int pos = (width * height * 4) - (4 * width);
		//		for(int row = 0; row < height; row++)
		//		{
		//			for(int col = 0; col < (4 * width); col += 4)
		//			{
		//				data[pos++] = row_pointers[row][col];        // red
		//				data[pos++] = row_pointers[row][col + 1]; // green
		//				data[pos++] = row_pointers[row][col + 2]; // blue
		//				data[pos++] = row_pointers[row][col + 3]; // alpha
		//			}
		//			pos=(pos - (width * 4)*2); //move the pointer back two rows
		//		}
		//	}
		//	break;
		//default:
		//	ASSERT(false);
		//}
		png_destroy_read_struct(&PngPtr, &InfoPtr, 0);

		readfile->CloseFile();
		FILEDESTROY( readfile );
		return NEW TextrueSourceData( vector2d(width, height), _FitFormat( width, height, channel, data ));
	}
	//------------------------------------------------------------------------------------------------------
	byte* TextureSource::_FitFormat( int width, int height, int channel, byte* input )
	{
		switch( m_eFormat )
		{
		case -1:
			{
				switch( channel )
				{
				case 1: m_eFormat = TextureStatus::PF_A8; break;
				case 3: m_eFormat = TextureStatus::PF_R8G8B8; break;
				case 4: m_eFormat = TextureStatus::PF_R8G8B8A8; break;
				default: ASSERT(false);
				}
				return input;
			}
		case TextureStatus::PF_A8 :
			{
				switch( channel )
				{
				case 1:
					{
						return input;
					}
				case 3:
					{
						byte* newData = NEW byte[width*height];
						for ( int i = 0 ; i < height ; i ++  )
						{
							for ( int j = 0 ; j < width ; j ++ )
							{
								newData[j + i * width] = input[ ( j + i * width ) * channel + 0 ] * 0.3
									+ input[ ( j + i * width ) * channel + 1 ] * 0.59
									+ input[ ( j + i * width ) * channel + 2 ] * 0.11;//转换到灰度
							}
						}
						SAFE_DELETE_ARRAY(input);
						return newData;
					}
				case 4:
					{
						byte* newData = NEW byte[width*height];
						for ( int i = 0 ; i < height ; i ++  )
						{
							for ( int j = 0 ; j < width ; j ++ )
							{

								newData[j + i * width] = input[ ( j + i * width ) * channel + 3 ]; //只要alpha通道
							}
						}
						SAFE_DELETE_ARRAY(input);
						return newData;
					}
				}
			}
		case TextureStatus::PF_L8 :
			{
				switch( channel )
				{
				case 1:
					{
						return input;
					}
				default:
					{
						byte* newData = NEW byte[width*height];
						for ( int i = 0 ; i < height ; i ++  )
						{
							for ( int j = 0 ; j < width ; j ++ )
							{
								newData[j + i * width] = input[ ( j + i * width ) * channel + 0 ] * 0.3
									+ input[ ( j + i * width ) * channel + 1 ] * 0.59
									+ input[ ( j + i * width ) * channel + 2 ] * 0.11;//转换到灰度
							}
						}
						SAFE_DELETE_ARRAY(input);
						return newData;
					}
				}
			}
		case TextureStatus::PF_R5G6B5 :	//需要数据转换
			{
				ASSERT( 1 != channel );
				ushort* newData = NEW ushort[width*height];
				for ( int i = 0 ; i < height ; i ++  )
				{
					for ( int j = 0 ; j < width ; j ++ )
					{
						ushort data = 0;
						data |= ( input[ ( j + i * width ) * channel + 0 ] >> 3 ) << 11;
						data |= ( input[ ( j + i * width ) * channel + 1 ] >> 2 ) << 5;
						data |= input[ ( j + i * width ) * channel + 2 ] >> 3;
						newData[j + i * width] = data;
					}
				}
				SAFE_DELETE_ARRAY(input);
				return (byte*)newData;
			}
		case TextureStatus::PF_R8G8B8 :
			{
				switch( channel )
				{
				case 3:
					{
						return input;
					}
				case 4://去掉alpha
					{
						byte* newData = NEW byte[width*height*3];
						for ( int i = 0 ; i < height ; i ++  )
						{
							for ( int j = 0 ; j < width ; j ++ )
							{
								int index = ( j + i * width ) * 3;
								int indexSor = ( j + i * width ) * 4;
								newData[ index + 0 ] = input[ indexSor + 0 ];
								newData[ index + 1 ] = input[ indexSor + 1 ];
								newData[ index + 2 ] = input[ indexSor + 2 ];
							}
						}
						SAFE_DELETE_ARRAY(input);
						return newData;
					}
				default:
					{
						ASSERT(false);
					}break;
				}
			}
		case TextureStatus::PF_R4G4B4A4 :
			{
				switch( channel )  //需要数据转换
				{
				case 3: // 这里就直接转换成 RGB565了，应为多alpha通道也是浪费
					{
						ushort* newData = NEW ushort[width*height];
						for ( int i = 0 ; i < height ; i ++  )
						{
							for ( int j = 0 ; j < width ; j ++ )
							{
								ushort data = 0;
								data |= ( input[ ( j + i * width ) * channel + 0 ] >> 3 ) << 11;
								data |= ( input[ ( j + i * width ) * channel + 1 ] >> 2 ) << 6;
								data |= input[ ( j + i * width ) * channel + 2 ] >> 3;
								newData[j + i * width] = data;
							}
						}
						SAFE_DELETE_ARRAY(input);
						return (byte*)newData;
					}
				case 4:
					{
						ushort* newData = NEW ushort[width*height];
						for ( int i = 0 ; i < height ; i ++  )
						{
							for ( int j = 0 ; j < width ; j ++ )
							{
								ushort data = 0;
								data |= ( input[ ( j + i * width ) * channel + 0 ] >> 4 ) << 12;
								data |= ( input[ ( j + i * width ) * channel + 1 ] >> 4 ) << 8;
								data |= ( input[ ( j + i * width ) * channel + 2 ] >> 4 ) << 4;
								data |=   input[ ( j + i * width ) * channel + 3 ] >> 4;
								newData[j + i * width] = data;
							}
						}
						SAFE_DELETE_ARRAY(input);
						return (byte*)newData;
					}
				default:
					{
						ASSERT(false);
					}break;
				}
			}
		case TextureStatus::PF_R8G8B8A8 :
			{
				switch( channel )
				{
				case 3:
					{
						byte* newData = NEW byte[width*height*4];
						for ( int i = 0 ; i < height ; i ++  )
						{
							for ( int j = 0 ; j < width ; j ++ )
							{
								int index = ( j + i * width ) * 4;
								int indexSor = ( j + i * width ) * 3;
								newData[ index + 0 ] = input[ indexSor + 0 ];
								newData[ index + 1 ] = input[ indexSor + 1 ];
								newData[ index + 2 ] = input[ indexSor + 2 ];
								newData[ index + 3 ] = 255;
							}
						}
						SAFE_DELETE_ARRAY(input);
						return newData;
					}
				case 4:
					{
						return input;
					}
				default:
					{
						ASSERT(false);
					}break;
				}
			}
		case TextureStatus::PF_DEPTH16 :
		case TextureStatus::PF_DEPTH24_STENCIL8 :
			{
				return input;
			}
		default:
			{
				ASSERT(false);
			}
		}
		return NULL;
	}
	//------------------------------------------------------------------------------------------------------
	void TextureSource::_DoLoadResource()
	{
		const vector< string >& vecString = GetResourcePaths();
		FOR_EACH_CONST( vector< string >, it, vecString )
		{
			m_DataVector.push_back( _LoadTextureSource(*it) );
		}
	}
	//------------------------------------------------------------------------------------------------------
	IResource* TextureSource::DoMakeResource()
	{
		ASSERT(false==m_isLoaded);
		TextureResource* res = TextureResource::Factory();
		res->GenerateTexture( m_DataVector, m_eTextureType, m_eFormat );
		res->SetAnisotropic( m_Anisotropic );
		res->SetFilterType( m_Filter );
		res->SetWarpType( m_Warp );
		_ReleaseMemory();
		m_isLoaded = true;
		return res;
	}

}
