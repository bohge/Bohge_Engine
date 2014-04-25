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
#include "VariableType.h"

#include <memory.h>


namespace BohgeEngine
{
	namespace VertexCompress
	{
		typedef void (*VertexCompressFunction)(const float* input, byte* output, byte count );
		//-------------------------------------------------------------------------------------------------------
		static void Float_Float ( const float* input, byte* output, byte count )
		{
			memcpy( output, input, count*sizeof(float) );
		}
		//-------------------------------------------------------------------------------------------------------
		static void Float_HalfFloat ( const float* input, byte* output, byte count )
		{
			ushort* pdest = reinterpret_cast<ushort*>(output);
			for ( int i = 0 ; i < count ; i ++ )
			{
				uint src = *(unsigned long*)(&input[i]);
				uint sign = src >> 31;
				// Extract mantissa
				uint mantissa = src  &  (1 << 23) - 1;
				// extract & convert exp bits
				long exp = (long)( src >> 23  &  0xFF ) - 127;
				if( exp > 16 )
				{
					// largest possible number...
					exp = 16;
					mantissa = (1 << 23) - 1;
				}
				else if( exp <= -15 )
				{
					// handle wraparound of shifts (done mod 64 on PPC)
					// All float32 denorm/0 values map to float16 0
					if( exp <= - 14 - 24 )
						mantissa = 0;
					else
						mantissa = ( mantissa | 1 << 23 )  >>  (-exp - 14);
					exp = -15;
				}
				pdest[i] = (unsigned short)(sign<<15) | (unsigned short)((exp+15)<<10) | (unsigned short)(mantissa>>13);
			}
		}
		//-------------------------------------------------------------------------------------------------------
		static void Float_INT101010 ( const float* input, byte* output, byte count )
		{
			ulong* pdest = reinterpret_cast<ulong*>(output);
			*pdest = 0;
			for ( int i = 0 ; i < count ; i ++ )
			{
				*pdest |= ( ( (unsigned long)(input[i]*511.0f) ) & 0x000003ff ) <<  (i*10);
			}
		}
		//-------------------------------------------------------------------------------------------------------
		static void Float_UINT101010 ( const float* input, byte* output, byte count )
		{
			ulong* pdest = reinterpret_cast<ulong*>(output);
			*pdest = 0;
			for ( int i = 0 ; i < count ; i ++ )
			{
				*pdest |= ( ( (unsigned long)(input[i]*1023.0f) ) & 0x000003ff ) <<  (i*10);
			}
		}
		//-------------------------------------------------------------------------------------------------------
		static void Float_INT8888 ( const float* input, byte* output, byte count )
		{
			ulong* pdest = reinterpret_cast<ulong*>(output);
			*pdest = 0;
			for ( int i = 0 ; i < count ; i ++ )
			{
				*pdest |= ( ( (unsigned long)(input[i]*127.0f) ) & 0x000000ff ) <<  (i*8);
			}
		}
		//-------------------------------------------------------------------------------------------------------
		static void Float_UINT8888 ( const float* input, byte* output, byte count )
		{
			ulong* pdest = reinterpret_cast<ulong*>(output);
			*pdest = 0;
			for ( int i = 0 ; i < count ; i ++ )
			{
				*pdest |= ( ( (unsigned long)(input[i]*255.0f) ) & 0x000000ff ) <<  (i*8);
			}
		}
		//-------------------------------------------------------------------------------------------------------
	};	 
}
