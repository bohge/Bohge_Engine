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



/////////////////////////////////
//		资源加载系统		   //
/////////////////////////////////
#include "ResLoader.h"
#include "IOSystem.h"
#include "IFile.h"
#include "EncryptFile.h"

#include "png.h"
#include "pnginfo.h"
#include "pngstruct.h"

using namespace std;



namespace BohgeEngine
{
	//------------------------------------------------------------------------------------------------------
	ResLoader::ResLoader(void)
	{
	}
	//------------------------------------------------------------------------------------------------------
	ResLoader::~ResLoader(void)
	{
	}
	//------------------------------------------------------------------------------------------------------
	ResLoader::ResTextrue* ResLoader::LoadTexture(std::string filename)
	{
		//int find = filename.find(".tga");
		if( filename.find(".tga") != -1 || filename.find(".TGA") != -1 )
		{
			return LoadTga(filename);
			//if( filename.find(".enc") != -1 )
			//{
			//	return LoadTgaEncrypted(filename);
			//}
			//else
			//{
			//	return LoadTga(filename);
			//}
		}
		else if( filename.find(".png") != -1 || filename.find(".PNG") != -1 )
		{
			return LoadPng(filename);
			//if( filename.find(".enc") != -1 )
			//{
			//	return LoadPngEncrypted(filename);
			//}
			//else
			//{
			//	return LoadPng(filename);
			//}
		}
		return NULL;
	}
	//------------------------------------------------------------------------------------------------------
	ResLoader::ResTextrue* ResLoader::LoadTga(std::string filename)
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
		byte* p = pBits;
		for( int y=0; y<header.ImageHeight/2; y++ )
		{
			static vector<byte> swaper;
			if ( swaper.size() < header.ImageWidth*nPixelSize )
			{
				swaper.resize( header.ImageWidth*nPixelSize );
			}
			memcpy( &swaper[0], p + y*header.ImageWidth*nPixelSize, header.ImageWidth*nPixelSize );
			memcpy( p + y*header.ImageWidth*nPixelSize, p+(header.ImageHeight-y-1)*header.ImageWidth*nPixelSize, header.ImageWidth*nPixelSize );
			memcpy( p + (header.ImageHeight-y-1)*header.ImageWidth*nPixelSize, &swaper[0], header.ImageWidth*nPixelSize );
		}
		return NEW ResTextrue( vector2d(header.ImageWidth, header.ImageHeight), nPixelSize, pBits);
	}
	//------------------------------------------------------------------------------------------------------
	static void png_rw(png_structp png_ptr, png_bytep data, png_size_t length) 
	{
		IFile* file = static_cast<IFile*>(png_ptr->io_ptr);
		file->ReadFile(data, length);
		//File::Instance().ReadFile(data, length, 1, file);
	}
	ResLoader::ResTextrue* ResLoader::LoadPng(std::string filename)
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
		// 拷贝！！注意，如果你读取的png没有A通道，就要3位3位的读。还有就是注意字节对其的问题，最简单的就是别用不能被4整除的宽度就行了。读过你实在想用，就要在这里加上相关的对齐处理。
		switch ( channel )
		{
		case 1://L
			{
				int pos = (width * height * 1) - (1 * width);
				for(int row = 0; row < height; row++)
				{
					for(int col = 0; col < (1 * width); col += 1)
					{
						data[pos++] = row_pointers[row][col];        //l
					}
					pos=(pos - (width * 1)*2); //move the pointer back two rows
				}
			}
			break;
		case 3://rgb
			{
				int pos = (width * height * 3) - (3 * width);
				for(int row = 0; row < height; row++)
				{
					for(int col = 0; col < (3 * width); col += 3)
					{
						data[pos++] = row_pointers[row][col];        // red
						data[pos++] = row_pointers[row][col + 1]; // green
						data[pos++] = row_pointers[row][col + 2]; // blue
					}
					pos=(pos - (width * 3)*2); //move the pointer back two rows
				}
			}
			break;
		case 4://rgba
			{
				int pos = (width * height * 4) - (4 * width);
				for(int row = 0; row < height; row++)
				{
					for(int col = 0; col < (4 * width); col += 4)
					{
						data[pos++] = row_pointers[row][col];        // red
						data[pos++] = row_pointers[row][col + 1]; // green
						data[pos++] = row_pointers[row][col + 2]; // blue
						data[pos++] = row_pointers[row][col + 3]; // alpha
					}
					pos=(pos - (width * 4)*2); //move the pointer back two rows
				}
			}
			break;
		default:
			ASSERT(false);
		}
		png_destroy_read_struct(&PngPtr, &InfoPtr, 0);

		readfile->CloseFile();
		FILEDESTROY( readfile );
		return NEW ResTextrue( vector2d(width, height), channel, data);
	}
	//------------------------------------------------------------------------------------------------------

}