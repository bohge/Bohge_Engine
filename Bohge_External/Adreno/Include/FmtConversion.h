// (c) 2007-2008 Advanced Micro Devices, Inc. All Rights Reserved.
// This file and all information it contains are AMD CONFIDENTIAL and no part thereof 
// may, in whole or in part, be used, reproduced or transmitted in any form or by any 
// means without the prior written permission of Advanced Micro Devices, Inc.
//=================================================================================================================================
//                                                                                                 
// GPG Developer Tools Group
// Advanced Micro Devices, Inc.
//
// Some functions to convert between formats.
//                
//=================================================================================================================================
// $Id: //projects/ACG/Tools/Adreno SDK/Adreno SDK/Include/FmtConversion.h#1 $
//
// Last check-in:  $DateTime: 2012/07/11 14:44:36 $
// Last edited by: $Author: pgiokari $
//=================================================================================================================================
//   (C) Advanced Micro Devices, Inc.  All rights reserved.              
//=================================================================================================================================

#ifndef _FMTCONVERSIONS_H_
#define _FMTCONVERSIONS_H_

//#include "ArgDefines.h"

typedef char               char8;
typedef char               int8;
typedef short              int16;
typedef int                int32;
typedef __int64            int64;
typedef float              float32;
typedef double             float64;
typedef unsigned char      uint8;
typedef unsigned short     uint16;
typedef unsigned int       uint32;
typedef unsigned __int64   uint64;


//=================================================================================================================================
///
/// Converts a fixed 16.16 int to a float32
///
/// \param The fixed 16.16 int value
///
/// \return The float32 value
//=================================================================================================================================
inline float32 FixedToFloat( uint32 v )
{
   float32 v0 = (float32)(int32)v;
   uint32 v1 = v ? (*((uint32*)&v0) - 0x08000000) : 0;
   
   return *((float32*)&v1);
}

//=================================================================================================================================
///
/// Converts a float32 to a fixed 16.16 int
///
/// \param The float32 value
///
/// \return The fixed 16.16 int value
//=================================================================================================================================
inline uint32 FloatToFixed( float32 v )
{
    float32 b = v;
    uint32 ret;
    uint32 d = *((uint32*)&b) + 0x08000000;

    ret = (int32)(*((float32*)&d));
    return ret;

}

//=================================================================================================================================
///
/// Converts a float32 to a float16
///
/// \param The float32 value
///
/// \return The float16 value
//=================================================================================================================================
inline uint16 FloatToHalf( float32 v )
{
   uint32 src = *(uint32*)(&v);
   uint32 sign = src >> 31;
   uint16 ret;

   // Extract mantissa
   uint32 mantissa = src  &  (1 << 23) - 1;

   // extract & convert exp bits
   int32 exp = int32( src >> 23  &  0xFF ) - 127;
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
      {
         mantissa = 0;
      }
      else
      {
         mantissa = ( mantissa | 1 << 23 )  >>  (-exp - 14);
      }
      exp = -15;
   }
   ret = (sign << 15)  |  ((exp + 15)  << 10) |  (mantissa >> 13);
   return ret;
}

//=================================================================================================================================
///
/// Converts a float16 to a float32
///
/// \param The float16 value
///
/// \return The float32 value
//=================================================================================================================================
inline float32 HalfToFloat( uint16 bits )
{
   uint32 sign = bits >> 15;
   uint32 mantissa = bits & (1 << 10) - 1;
   int32 exp = int32( bits >> 10  &  0x1F ) - 15;
   
   if( exp == -15 && mantissa != 0 )
   {
      exp = -14;
      while( !(mantissa  &  1 << 10) )
      {
         mantissa <<= 1;
         exp -= 1;
      }
      mantissa &= 0x3FF;
   }
   else if( exp == -15 )
   {
      exp = -127;
      mantissa = 0;
   }
   uint32 f = (sign << 31)  |  ((exp + 127)  <<  23)  |  (mantissa << 13);

   return *(float32*)&f;
}

//=================================================================================================================================
///
/// Converts 4 floats into a GL_UNSIGNED_INT_2_10_10_10_REV
///
/// \param The float16 value
///
/// \return The float32 value
//=================================================================================================================================
inline uint32 FloatsToUint2_10_10_10( float32 a, float32 b, float32 g, float32 r)
{
	// Clamp from 0 to 1
	float32 fTempR = (r > 1.0f) ? 1.0f : (r < 0.0f) ? 0.0f : r;
	float32 fTempG = (g > 1.0f) ? 1.0f : (g < 0.0f) ? 0.0f : g;
	float32 fTempB = (b > 1.0f) ? 1.0f : (b < 0.0f) ? 0.0f : b;
	float32 fTempA = (a > 1.0f) ? 1.0f : (a < 0.0f) ? 0.0f : a;

	// Convert to unsigned integer 0 to 1023 range
	uint32 iR = (uint32) (fTempR * 1023.0f);
	uint32 iG = (uint32) (fTempG * 1023.0f);
	uint32 iB = (uint32) (fTempB * 1023.0f);
	uint32 iA = (uint32) (fTempA * 3.0f);

	return ((iA & 0x3) << 30) | ((iB & 0x3FF) << 20) | ((iG & 0x3FF) << 10) | (iR & 0x3FF);
}

//=================================================================================================================================
///
/// Converts 4 floats into a GL_UNSIGNED_INT_10_10_10_2
///
/// \param The float16 value
///
/// \return The float32 value
//=================================================================================================================================
inline uint32 FloatsToUint10_10_10_2( float32 r, float32 g, float32 b, float32 a )
{
   // Clamp from 0 to 1
   float32 fTempR = (r > 1.0f) ? 1.0f : (r < 0.0f) ? 0.0f : r;
   float32 fTempG = (g > 1.0f) ? 1.0f : (g < 0.0f) ? 0.0f : g;
   float32 fTempB = (b > 1.0f) ? 1.0f : (b < 0.0f) ? 0.0f : b;
   float32 fTempA = (a > 1.0f) ? 1.0f : (a < 0.0f) ? 0.0f : a;

   // Convert to unsigned integer 0 to 1023 range
   uint32 iR = (uint32) (fTempR * 1023.0f);
   uint32 iG = (uint32) (fTempG * 1023.0f);
   uint32 iB = (uint32) (fTempB * 1023.0f);
   uint32 iA = (uint32) (fTempA * 3.0f);

   return ((iR & 0x3FF) << 22) | ((iG & 0x3FF) << 12) | ((iB & 0x3FF) << 2) | (iA & 0x3);
}

//=================================================================================================================================
///
/// Converts 3 floats into a GL_INT_10_10_10
///
/// \param The float16 value
///
/// \return The float32 value
//=================================================================================================================================
inline uint32 FloatsToInt10_10_10( float32 r, float32 g, float32 b )
{
   // Clamp from -1 to 1
   float32 fTempR = (r > 1.0f) ? 1.0f : (r < -1.0f) ? -1.0f : r;
   float32 fTempG = (g > 1.0f) ? 1.0f : (g < -1.0f) ? -1.0f : g;
   float32 fTempB = (b > 1.0f) ? 1.0f : (b < -1.0f) ? -1.0f : b;

   // Convert to signed integer -511 to +511 range
   int32 iR = (uint32) (fTempR * 511.0f);
   int32 iG = (uint32) (fTempG * 511.0f);
   int32 iB = (uint32) (fTempB * 511.0f);

   return ((iR & 0x3FF) << 22) | ((iG & 0x3FF) << 12) | ((iB & 0x3FF) << 2) | (0 & 0x3);
}

//=================================================================================================================================
///
/// Converts a GL_UNSIGNED_INT_10_10_10_2 to 3 floats
///
/// \param The float16 value
///
/// \return The float32 value
//=================================================================================================================================
inline void Uint10_10_10_2ToFloats( int32 inVal, float32 *outVal )
{
   uint32 iR = (inVal >> 22) & 0x3FF;
   uint32 iG = (inVal >> 12) & 0x3FF;
   uint32 iB = (inVal >> 2) & 0x3FF;
   uint32 iA = (inVal & 0x3 );
   
   outVal[0] = (float32)iR;
   outVal[1] = (float32)iG;
   outVal[2] = (float32)iB;
   outVal[3] = (float32)iA;

}

//=================================================================================================================================
///
/// Converts a GL_UNSIGNED_INT_10_10_10_2 to 3 normalized floats
///
/// \param The float16 value
///
/// \return The float32 value
//=================================================================================================================================
inline void Uint10_10_10_2ToFloatsN( int32 inVal, float32 *outVal )
{
   uint32 iR = (inVal >> 22) & 0x3FF;
   uint32 iG = (inVal >> 12) & 0x3FF;
   uint32 iB = (inVal >> 2) & 0x3FF;
   uint32 iA = (inVal & 0x3 );
   
   outVal[0] = float32(iR) / 1023.0f;
   outVal[1] = float32(iG) / 1023.0f;
   outVal[2] = float32(iB) / 1023.0f;
   outVal[3] = float32(iA) / 3.0f;
}

//=================================================================================================================================
///
/// Converts a GL_INT_10_10_10_2 to 3 floats
///
/// \param The float16 value
///
/// \return The float32 value
//=================================================================================================================================
inline void Int10_10_10_2ToFloats( int32 inVal, float32 *outVal )
{
   int32 iR = (inVal >> 22) & 0x3FF;
   if( iR & 0x200 )
      iR |= 0xfffffc00; // handle negitive numbers
   int32 iG = (inVal >> 12) & 0x3FF;
   if( iG & 0x200 )
      iG |= 0xfffffc00; // handle negitive numbers
   int32 iB = (inVal >> 2) & 0x3FF;
   if( iB & 0x200 )
      iB |= 0xfffffc00; // handle negitive numbers
   
   outVal[0] = (float32)iR;
   outVal[1] = (float32)iG;
   outVal[2] = (float32)iB;
}

//=================================================================================================================================
///
/// Converts a GL_INT_10_10_10_2 to 3 normalized floats
///
/// \param The float16 value
///
/// \return The float32 value
//=================================================================================================================================
inline void Int10_10_10_2ToFloatsN( int32 inVal, float32 *outVal )
{
   int32 iR = (inVal >> 22) & 0x3FF;
   if( iR & 0x200 )
      iR |= 0xfffffc00; // handle negitive numbers
   int32 iG = (inVal >> 12) & 0x3FF;
   if( iG & 0x200 )
      iG |= 0xfffffc00; // handle negitive numbers
   int32 iB = (inVal >> 2) & 0x3FF;
   if( iB & 0x200 )
      iB |= 0xfffffc00; // handle negitive numbers
   
   outVal[0] = float32(iR) / 511.0f;
   outVal[1] = float32(iG) / 511.0f;
   outVal[2] = float32(iB) / 511.0f;
}

//=================================================================================================================================
///
/// Converts a 3 floats in the range 0-1 to a packed short565
///
/// \param r - red value
/// \param g - green value
/// \param b - blue value
///
/// \return The packed short 565
//=================================================================================================================================
inline uint16 FloatsToShort5_6_5( float32 r, float32 g, float32 b )
{
   // Clamp from 0 to max
   float32 fTempR = (r < 0.0f) ? 0.0f : (r > 1.0f) ? 1.0f : r;
   float32 fTempG = (g < 0.0f) ? 0.0f : (g > 1.0f) ? 1.0f : g;
   float32 fTempB = (b < 0.0f) ? 0.0f : (b > 1.0f) ? 1.0f : b;

   // Convert to integer max range (32 to 64)
   uint16 iR = (uint32) (fTempR * 31.0f);
   uint16 iG = (uint32) (fTempG * 63.0f);
   uint16 iB = (uint32) (fTempB * 31.0f);

   return ((iR & 0x1F) << 11) | ((iG & 0x3F) << 5) | ((iB & 0x1F) << 0);
}

//=================================================================================================================================
///
/// Converts a an uint24 to an uint32
///
/// \param val - the input uint24 value
///
/// \return The uint32 value
//=================================================================================================================================
inline uint32 Uint24ToUint32( uint8* val )
{
   uint32 p0 = val[0];
   uint32 p1 = val[1];
   uint32 p2 = val[2];
   uint32 out;

   out = ( ( p0 << 0 ) + ( p1 << 8 ) + ( p2 << 16 ) ) << 8;
   return out;
}

//=================================================================================================================================
///
/// Converts a an uint32 to an uint24
///
/// \param inVal - The input uint32 value
/// \param outVal - The output uint24 value
///
/// \return void
//=================================================================================================================================
inline void Uint32ToUint24( uint32 inVal, uint8 *outVal )
{
   outVal[0] = ((int8*)&inVal)[1];
   outVal[1] = ((int8*)&inVal)[2];
   outVal[2] = ((int8*)&inVal)[3];
}

#endif // _FMTCONVERSIONS_H_

