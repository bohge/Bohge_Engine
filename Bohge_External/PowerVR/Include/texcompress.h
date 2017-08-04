//               Copyright (c) 2009 QUALCOMM Incorporated. 
//                         All Rights Reserved. 
//                      QUALCOMM Proprietary/GTDR

#ifndef __TEXCOMPRESS_H__
#define __TEXCOMPRESS_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _Color888 {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
} Color888_t;


// Signatures
#define ATITC_SIGNATURE                 0xCCC40002 // ATC Format
#define ATI1N_SIGNATURE                 0x31495441 // ATI1N 3DC_X format
#define ATI2N_SIGNATURE                 0x32495441 // ATI2N 3DC_XY format
#define ETC_SIGNATURE                   0xEC000001 // ETC format


// Errors
#define TXCMPR_ERROR_NO_ERROR                    0
#define TXCMPR_ERROR_BAD_DIMENSIONS              1
#define TXCMPR_ERROR_BUFFER_TOO_SMALL            2
#define TXCMPR_ERROR_BAD_FLAGS                   3
#define TXCMPR_ERROR_BAD_SIGNATURE               4
#define TXCMPR_ERROR_BAD_COLOR_CHANNEL_ORDER     5
#define TXCMPR_ERROR_LOW_QUALITY_RESULT          6
#define TXCMPR_ERROR_BAD_ARGUMENTS               7
#define TXCMPR_ERROR_CANNOT_OPEN_FILE            8
#define TXCMPR_ERROR_MEMORY_ALLOCATION_FAILED    9

// Flags
#define TXCMPR_RGB                       0x00000001 // Used to select RGB only compression
#define TXCMPR_RGBA                      0x00000002 // Used to select standard alpha compression scheme
#define TXCMPR_ALPHA_INTERP              0x00000010 // Used to select alpha interp. compression scheme instead of A4


// CTES file format header
typedef struct _TXCMPR_HEADER 
{
    unsigned int   signature;           // Compression format signature
    unsigned int   width;               // Width of base level in pixels
    unsigned int   height;              // Height of base level in pixels
    unsigned int   flags;               // Flags (TXCMPR_RGB, TXCMPR_RGBA, TXCMPR_ALPHA_INTERP)
    unsigned int   dataOffset;          // From start of header/file

    unsigned int   headerVersion;       // Version number of header file
    unsigned int   numLevels;           // Number of mip levels stored in file
    unsigned int   internalFormat;      // Internal format to be passed to glCompressedTexImage2D()
} TXCMPR_HEADER;

// CTES File Format details:
//    The CTES file format contains a header, followed by the compressed image data.  If
//    an entire mipchain is stored, all of the level data follows.
//
//    The format for a CTES file is as follows:
//    TXCMPR_HEADER:    [0..sizeof(TXCMPR_HEADER)]
///   User Data:        [(TXCMPR_HEADER END)..(TXCMPR_HEADER END) + TXCMPR_HEADER.dataOffset]
//    Image Data:       [(User Data END)..(User Data END) + TxCompressQueryEncodeBufferSize(TXCOMPR_HEADER...)]
//    

// Color channel order for decode
#define TXCMPR_RGB_RGBA_ORDER            1
#define TXCMPR_BGR_BGRA_ORDER            2

///
/// \brief Determines the size in bytes required to store the decompressed image in bytes
/// \param width Width of the compressed image in pixels
/// \param height Height of the compressed image in pixels
/// \param signature Texture compression format signature for the compressed image
///                  (ATITC_SIGNATURE, ATI1N_SIGNATURE, ATI2N_SIGNATURE, or ETC_SIGNATURE)
/// \param boolHasAlpha Whether the compressed image has an alpha channel
/// \param pcbOutputBuffer Pointer to an unsigned integer which will be assigned the size of 
///                        the decompressed image in bytes
/// \return TXCMPR_ERROR_NO_ERROR on success, otherwise one of the TXCMPR_ERROR_* error codes
///
extern unsigned int
TxCompressQueryDecodeBufferSize(
            int             width,
            int             height,
            unsigned int    signature,
            int             boolHasAlpha,
            unsigned int    *pcbOutputBuffer
            );


///
/// \brief Function to setup data in the TXCMPR_HEADER struct
/// \param pHeaderBuffer Pointer to the TXCMPR_HEADER struct to setup
/// \param width Width of the compressed image in pixels
/// \param height Height of the compressed image in pixels
/// \param numLevels The number of mipmap levels of the compressed image
/// \param dstFlags Compression format flags (TXCMPR_RGB, TXCMPR_RGBA, TXCMPR_ALPHA_INTERP)
/// \param signature Texture compression format signature for the compressed image
///                  (ATITC_SIGNATURE, ATI1N_SIGNATURE, ATI2N_SIGNATURE, or ETC_SIGNATURE)
/// \param numBytesPrivateData Number of bytes of data between header and start of compression (usually 0)
/// \return TXCMPR_ERROR_NO_ERROR on success, otherwise one of the TXCMPR_ERROR_* error codes
///
extern unsigned int 
TxCompressFillHeader(
            unsigned char *pHeaderBuffer,
            int width, 
            int height, 
            int numLevels, 
            unsigned int dstFlags, 
            unsigned int signature,
            unsigned int numBytesPrivateData
            );

///
/// \brief Determines the size in bytes required to store the compressed image in bytes
/// \param width Width of the uncompressed image in pixels
/// \param height Height of the uncompressed image in pixels
/// \param numLevels The number of levels to calculate space for
/// \param dstFlags Compression format flags (TXCMPR_RGB, TXCMPR_RGBA, TXCMPR_ALPHA_INTERP)
/// \param signature Texture compression format signature for the compressed image
///                  (ATITC_SIGNATURE, ATI1N_SIGNATURE, ATI2N_SIGNATURE, or ETC_SIGNATURE)
/// \param pcbOutputBuffer Pointer to an unsigned integer which will be assigned the size of 
///                        the compressed image data in bytes
/// \return TXCMPR_ERROR_NO_ERROR on success, otherwise one of the TXCMPR_ERROR_* error codes
///
extern unsigned int
TxCompressQueryEncodeBufferSize( 
            int             width, 
            int             height, 
            int             numLevels,
            unsigned int    dstFlags,
            unsigned int    signature,
            unsigned int    *pcbOutputBuffer
            );


///
/// \brief Compress an uncompressed texture image
/// \param maskRed Mask for Red bits in source
/// \param maskGreen Mask for Green bits in source
/// \param maskBlue Mask for Blue bits in source
/// \param maskAlpha Mask for Alpha bits in source
/// \param width Width of the uncompressed image in pixels
/// \param height Height of the uncompressed image in pixels
/// \param stride Stride in bytes of the uncompressed image
/// \param dstFlags Compression format flags (TXCMPR_RGB, TXCMPR_RGBA, TXCMPR_ALPHA_INTERP)
/// \param signature Texture compression format signature for the compressed image
///                  (ATITC_SIGNATURE, ATI1N_SIGNATURE, ATI2N_SIGNATURE, or ETC_SIGNATURE)
/// \param pOutputBuffer Pointer to where the compressed image texel data is written
/// \param pcbOutputBuffer Pointer to where the size of the output 
/// \param pcbOutputBuffer Size of output buffer on input; number of bytes used on output
/// \return TXCMPR_ERROR_NO_ERROR on success, otherwise one of the TXCMPR_ERROR_* error codes
///
extern unsigned int 
TxCompressEncode( 
             const unsigned char  *pPixelDataIn,

             unsigned int   maskRed,            
             unsigned int   maskGreen,          
             unsigned int   maskBlue,           
             unsigned int   maskAlpha,          

             int            width,              
             int            height,             
             int            stride,             
             unsigned int   dstFlags,

             unsigned int   signature,

             unsigned char  *pOutputBuffer,
             unsigned int   *pcbOutputBuffer
           );

///
/// \brief Decompress a compressed texture image
/// \param signature Texture compression format signature for the compressed image
///                  (ATITC_SIGNATURE, ATI1N_SIGNATURE, ATI2N_SIGNATURE, or ETC_SIGNATURE)
/// \param width Width of the compressed image in pixels
/// \param height Height of the compressed image in pixels
/// \param flags Compression format flags (TXCMPR_RGB, TXCMPR_RGBA, TXCMPR_ALPHA_INTERP)
/// \param pEncodedData Compressed mip levels (little-endian) (Do not adjust for each level)
/// \param level Mipmap level or zero if a single level
/// \param colorChannelOrder Desired color channel order (TXCMPR_RGB_RGBA_ORDER or TXCMPR_BGR_BGRA_ORDER)
/// \param pDecodedData Output buffer for decoded image
/// \param cbOutputBuffer Size of the output buffer in bytes
/// \return TXCMPR_ERROR_NO_ERROR on success, otherwise one of the TXCMPR_ERROR_* error codes
///
extern unsigned int 
TxCompressDecode(           
            unsigned int   signature,
            unsigned int   width,
            unsigned int   height,
            unsigned int   flags,
            unsigned char *pEncodedData,       
            int           level,               
            unsigned int  colorChannelOrder, 
            unsigned char *pDecodedData, 
            unsigned int  cbOutputBuffer
           );

///
/// \brief Decompress a compressed texture image (from a CTES file)
/// \param pFileHeader Pointer to TXCMPR_HEADER for CTES file
/// \param pEncodedData Pointer to encoded image data (includes all mip levels - do not adjust for each level)
/// \param level Mipmap level or zero if a single level
/// \param colorChannelOrder Desired color channel order (TXCMPR_RGB_RGBA_ORDER or TXCMPR_BGR_BGRA_ORDER)
/// \param pDecodedData Output buffer for decoded image
/// \param cbOutputBuffer Size of the output buffer in bytes
/// \return TXCMPR_ERROR_NO_ERROR on success, otherwise one of the TXCMPR_ERROR_* error codes
///
extern unsigned int 
TxCompressDecodeFileData( 
             unsigned char *pFileHeader,
             unsigned char *pEncodedData,
             int           level,
             unsigned int  colorChannelOrder,
             unsigned char *pDecodedData,
             unsigned int  cbOutputBuffer
            );

#ifdef __cplusplus
}
#endif

#endif  /* __TEXCOMPRESS_H__ */
