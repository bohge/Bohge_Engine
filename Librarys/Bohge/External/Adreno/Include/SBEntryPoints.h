/* -------------------------------------------------------------------------
 *
 * (C) Copyright Advanced Micro Devices, Inc. 2002, 2007
 * Confidential and Proprietary Information of Advanced Micro Devices, Inc
 *
 * This file and all information it contains are AMD CONFIDENTIAL and no
 * part thereof may, in whole or in part, be used, reproduced or transmitted
 * in any form or by any means without the prior written permission of
 * Advanced Micro Devices, Inc
 *
 * $Id: //projects/ACG/Tools/Adreno SDK/Adreno SDK/Include/SBEntryPoints.h#3 $
 * Last check-in: $DateTime: 2012/07/17 08:59:10 $
 * Last edited by: $Author: pgiokari $
 *
 *//*!
 * \file    SBEntryPoints.h
 * \brief   Header file for the BinaryShader Libary. This libary is used to 
 * create shader binary buffers for use with the AMD OpenGL ES 2.0 Emulator.
 *//*----------------------------------------------------------------------*/

#ifndef _ENTRYPOINTS_H_
#define _ENTRYPOINTS_H_
#ifdef __cplusplus
extern "C" {
#endif

#ifdef SHADERBINARY_EXPORTS
#define SB_API __declspec(dllexport)
#else
#define SB_API __declspec(dllimport)
#endif // SHADERBINARY_EXPORTS

//
//  Flags that may be or'd together when calling SBCreateBinary to compose the flags parameter.
//
#define SB_SUPPRESS_SRC     0x00000001

//
//  Values that describe the binary type for the shader to be created.  Used as the binaryType parameter
//  of SBCreateBianry and SBFreeBuffer.  These values must be specified singly and may not be combined.
//
#define SB_BINARYTYPE_Z400  0x00008001

//=================================================================================================================================
///
/// SBCreateBinary - Takes in the vertex and fragment shader strings and creates a binary shader.
///
/// \param vertShader - Vertex shader string input (null means skip it)
/// \param fragShader - Fragment shader string input (null means skip it)
/// \param binary - The binary buffer output
/// \param bufferSize - Returns the size of the binary buffer
/// \param binaryType - The binary format that is to be created (ie SB_BINARYTYPE_Z400)
/// \param flags        - flags to control the operation of the offline shader compiler.
///
/// \return A Boolean indicating if this function passed or failed.  If it passed the value in **binary is the binary shader, 
///         and if it failed the value in **binary is an error string. 
//=================================================================================================================================
SB_API bool SBCreateBinary( const char *vertShader, const char *fragShader, void **binary, int *bufferSize, const int binaryType, unsigned int flags );

//=================================================================================================================================
///
/// SBFreeBuffer - Frees the binary buffer that was created with SBCreateBinary(). 
///
/// \param binary - The binary buffer that is to be deleted
/// \param binaryType - The binary format of the buffer to be freed (ie SB_BINARYTYPE_Z400)
///
/// \return A Boolean indicating if this function passed or failed.
//=================================================================================================================================
SB_API bool SBFreeBuffer( void *binary, const int binaryType );

#ifdef __cplusplus
}
#endif
#endif // _ENTRYPOINTS_H_

