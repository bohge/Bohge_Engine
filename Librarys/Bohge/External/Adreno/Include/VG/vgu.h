/*------------------------------------------------------------------------
 *
 * OpenVG 1.1 Reference Implementation
 * -----------------------------------
 *
 * Copyright (c) 2007 The Khronos Group Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and /or associated documentation files
 * (the "Materials "), to deal in the Materials without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Materials,
 * and to permit persons to whom the Materials are furnished to do so,
 * subject to the following conditions: 
 *
 * The above copyright notice and this permission notice shall be included 
 * in all copies or substantial portions of the Materials. 
 *
 * THE MATERIALS ARE PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE MATERIALS OR
 * THE USE OR OTHER DEALINGS IN THE MATERIALS.
 *
 *//**
 * \file
 * \brief OpenVG VGU 1.1 API.
 *//*-------------------------------------------------------------------*/

#ifndef _VGU_H
#define _VGU_H

#ifdef __cplusplus 
extern "C" { 
#endif

#include "VG/openvg.h"

#define VGU_VERSION_1_0   1
#define VGU_VERSION_1_1     2

#ifndef VGU_API_CALL
#define VGU_API_CALL VG_API_CALL
#endif

typedef enum {
  VGU_NO_ERROR                                 = 0,
  VGU_BAD_HANDLE_ERROR                         = 0xF000,
  VGU_ILLEGAL_ARGUMENT_ERROR                   = 0xF001,
  VGU_OUT_OF_MEMORY_ERROR                      = 0xF002,
  VGU_PATH_CAPABILITY_ERROR                    = 0xF003,
  VGU_BAD_WARP_ERROR                           = 0xF004
} VGUErrorCode;

typedef enum {
  VGU_ARC_OPEN                                 = 0xF100,
  VGU_ARC_CHORD                                = 0xF101,
  VGU_ARC_PIE                                  = 0xF102
} VGUArcType;

VGU_API_CALL VGUErrorCode vguLine(VGPath path,
          VGfloat x0, VGfloat y0,
          VGfloat x1, VGfloat y1);

VGU_API_CALL VGUErrorCode vguPolygon(VGPath path,
            const VGfloat * points, VGint count,
            VGboolean closed);

VGU_API_CALL VGUErrorCode vguRect(VGPath path,
         VGfloat x, VGfloat y,
         VGfloat width, VGfloat height);

VGU_API_CALL VGUErrorCode vguRoundRect(VGPath path,
              VGfloat x, VGfloat y,
              VGfloat width, VGfloat height,
              VGfloat arcWidth, VGfloat arcHeight);
  
VGU_API_CALL VGUErrorCode vguEllipse(VGPath path,
            VGfloat cx, VGfloat cy,
            VGfloat width, VGfloat height);

VGU_API_CALL VGUErrorCode vguArc(VGPath path,
        VGfloat x, VGfloat y,
        VGfloat width, VGfloat height,
        VGfloat startAngle, VGfloat angleExtent,
        VGUArcType arcType);

VGU_API_CALL VGUErrorCode vguComputeWarpQuadToSquare(VGfloat sx0, VGfloat sy0,
                                                    VGfloat sx1, VGfloat sy1,
                                                    VGfloat sx2, VGfloat sy2,
                                                    VGfloat sx3, VGfloat sy3,
                                                    VGfloat * matrix);

VGU_API_CALL VGUErrorCode vguComputeWarpSquareToQuad(VGfloat dx0, VGfloat dy0,
                                                    VGfloat dx1, VGfloat dy1,
                                                    VGfloat dx2, VGfloat dy2,
                                                    VGfloat dx3, VGfloat dy3,
                                                    VGfloat * matrix);

VGU_API_CALL VGUErrorCode vguComputeWarpQuadToQuad(VGfloat dx0, VGfloat dy0,
                                                  VGfloat dx1, VGfloat dy1,
                                                  VGfloat dx2, VGfloat dy2,
                                                  VGfloat dx3, VGfloat dy3,
                          VGfloat sx0, VGfloat sy0,
                                                  VGfloat sx1, VGfloat sy1,
                                                  VGfloat sx2, VGfloat sy2,
                                                  VGfloat sx3, VGfloat sy3,
                                                  VGfloat * matrix);

#ifdef __cplusplus 
} /* extern "C" */
#endif

#endif /* #ifndef _VGU_H */
