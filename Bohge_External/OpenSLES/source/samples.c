//--------------------------------------------------------------------------------------
// File:		samples.c
//
// Desc:		This file contains 4 samples that show how to use OpenSL ES
//
// Author:      QUALCOMM, Advanced Content Group - Snapdragon SDK
//
//              Copyright (c) 2011 QUALCOMM Incorporated. 
//                        All Rights Reserved. 
//                     QUALCOMM Proprietary/GTDR
//--------------------------------------------------------------------------------------

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <memory.h>
#include <stdio.h>

#include <SLES/OpenSLES.h>

#include "sample_framework.h"

/**************************************************************
 * Demostrates a quick example of how to use the functions above
 **************************************************************/
int SampleSimple()
{

	SLresult            Result = SL_RESULT_SUCCESS;
    SLESSystemData      System;
    PlayerData          Player;    
    SLuint32            i = 0;
    const float         FadeTime = 10.0f;
    const SLuint32      FadeIntervals = 1000;

	PRINTF("SAMPLE SIMPLE...");

    memset(&System, 0 , sizeof(System));
    memset(&Player, 0 , sizeof(Player));

    // Initialize the SLES System
    Result = InitSLES(&System, SL_BOOLEAN_FALSE);
    ASSERT(Result == SL_RESULT_SUCCESS);

    // Create a Player
    Result = CreatePlayerSimple(&Player, &System, "drumloop.wav");
    ASSERT(Result == SL_RESULT_SUCCESS);
    
    // Play the sound for 1 second and then do a 10 second fade
    Result = Play(&Player);
    ASSERT(Result == SL_RESULT_SUCCESS);
    
    SLES_Sleep(1000);

    // Fade out
    for(i=0; i<FadeIntervals; ++i) 
    {
       SetVolume(&Player, ((float)(FadeIntervals - i) / (float)FadeIntervals) );
       SLES_Sleep((int)((FadeTime / (float)FadeIntervals)*1000.0f));
    }

    // Stop once fade is done
    Result = Stop(&Player);
    ASSERT(Result == SL_RESULT_SUCCESS);

    // Kill all survivers
    Result = DestroyPlayer(&Player);
    ASSERT(Result == SL_RESULT_SUCCESS);

    Result = DestroySLES(&System);
    ASSERT(Result == SL_RESULT_SUCCESS);

	PRINTF("...");

    return 0;
}


/**************************************************************
 * Demonstrates a quick example of how to use 3D in OpenSLES
 **************************************************************/
int Sample3D()
{
	SLresult            Result = SL_RESULT_SUCCESS;
    SLESSystemData      System;
    PlayerData          Player;    
    SLuint32            i            = 0;
    float               Angle        = 0.0f;
    const float         Radius       = 1.0f;
    const float         Freq         = 0.2f;    // Rotations per Second
    const int           FrameTime    = 15;     // ms

	PRINTF("SAMPLE 3D...");

    memset(&System, 0 , sizeof(System));
    memset(&Player, 0 , sizeof(Player));

    // Initialize the SLES System
    Result = InitSLES(&System, SL_BOOLEAN_FALSE);
    ASSERT(Result == SL_RESULT_SUCCESS);
    
    Result = CreateListener(&System);
    ASSERT(Result == SL_RESULT_SUCCESS);

    Result = SetListenerPosition(&System, 0, 0, 0, 1, 0);
    ASSERT(Result == SL_RESULT_SUCCESS);

    // Create a Player
    Result = CreatePlayer3d(&Player, &System, "drumloop.wav");
    ASSERT(Result == SL_RESULT_SUCCESS);
    
    // Play 
    Result = Play(&Player);
    ASSERT(Result == SL_RESULT_SUCCESS);
    
    // Circle around your head
    for(i = 0; i < 1000; ++i)
    {
        float X=0, Y=0;  
        Angle += 360.0f*Freq*FrameTime*0.001f;
	    while(Angle > 360.0f)
	       	Angle -= 360.0f;
	       		
	    X = Radius * cosf( DEG2RAD(Angle) );
		Y = Radius * sinf( DEG2RAD(Angle) );

        //PRINTF("Pos: (%0.3f, %0.3f) A: %f", X, Y, Angle);

        Result = SetPosition( &Player, &System, X, Y, 0);
        ASSERT(Result == SL_RESULT_SUCCESS);

        SLES_Sleep(FrameTime);
    }

    // Stop 
    Result = Stop(&Player);
    ASSERT(Result == SL_RESULT_SUCCESS);

    // Kill all survivers
    Result = DestroyPlayer(&Player);
    ASSERT(Result == SL_RESULT_SUCCESS);

    Result = DestroySLES(&System);
    ASSERT(Result == SL_RESULT_SUCCESS);

	PRINTF("...");

    return 0;
}


/**************************************************************
 * Demonstrates a quick example of how to use an effect in OpenSLES
 **************************************************************/
int SampleEffects()
{
	SLresult            Result = SL_RESULT_SUCCESS;
    SLESSystemData      System;
    PlayerData          Player;    
    SLuint16            iPreset      = 0;

	PRINTF("SAMPLE EFFECTS...");

    memset(&System, 0 , sizeof(System));
    memset(&Player, 0 , sizeof(Player));

    // Initialize the SLES System
    Result = InitSLES(&System, SL_BOOLEAN_TRUE);
    ASSERT(Result == SL_RESULT_SUCCESS);
       

    // Create a Player
    Result = CreatePlayerEffects(&Player, &System, EFFECT_VIRTUALIZER, "drumloop.wav");
    ASSERT(Result == SL_RESULT_SUCCESS);
    
    // Play the sound 
    Result = Play(&Player);
    ASSERT(Result == SL_RESULT_SUCCESS);
    
    SLES_Sleep(5000);

    // PresetReverb
    if(System.PresetReverbItf) 
    {
        for(iPreset=0; iPreset<7; ++iPreset)
        {       
            PRINTF("Setting PresetReverb %d", iPreset);
            Result = (*System.PresetReverbItf)->SetPreset(System.PresetReverbItf, iPreset);
            ASSERT(Result == SL_RESULT_SUCCESS);
            SLES_Sleep(5000);
        }
    }

    SLES_Sleep(10000);

    // Stop once fade is done
    Result = Stop(&Player);
    ASSERT(Result == SL_RESULT_SUCCESS);

    // Kill all survivers
    Result = DestroyPlayer(&Player);
    ASSERT(Result == SL_RESULT_SUCCESS);

    Result = DestroySLES(&System);
    ASSERT(Result == SL_RESULT_SUCCESS);

	PRINTF("...");

	return 0;
}


/**************************************************************
 * Streaming sample callback function
 **************************************************************/
#define STREAM_BUFFER_SIZE (2*1024)

typedef struct WavHeaderData_ {
    char        RiffTag[4];         
    unsigned    ChunkSize;
    char        Format[4];          
    char        Subchunk1ID[4];     
    unsigned    Subchunk1Size;
    short       AudioFormat;        
    short       nChannels;           
    unsigned    SampleRate;         
    unsigned    ByteRate;
    short       BlockAlign;
    short       BitsPerSample;      
    char        Subchunk2ID[4];     
    unsigned    Subchunk2Size;      
} WavHeaderData;

typedef struct StreamData_ {
    FILE*       pFile;          // File we are streaming    
    SLuint32    FileLength;     // File Length
    SLuint32    FileOffset;     // Offset of where we are streaming from
    PlayerData* pPlayer;        // SLES Player    
    void*       pBuffers[MAX_STREAM_BUFFERS];   // Buffer pool we cycle through
    SLuint32    iPlayingBuffer;   // The index of the first buffer in the queue that is playing.
    SLuint32    iQueueBuffer;     // The index of the last buffer in the queue that is ready to be filled and enqueued.
}StreamData;

// This callback is called everytime a buffer is dequeued
void SLAPIENTRY StreamCallback(SLBufferQueueItf caller, void *pContext)
{
    SLresult    Result  = SL_RESULT_SUCCESS;
    StreamData* pStream = (StreamData*)pContext;

    ASSERT(pStream);

    PRINTF("Stream Callback iPlayed: %d iQueued: %d Streamed: %d/%d", pStream->iPlayingBuffer, pStream->iQueueBuffer, pStream->FileOffset, pStream->FileLength);

    // Cleanup the buffer that was just used. We are reusing buffers so just clear the memory.
    //memset(pStream->pBuffers[pStream->iPlayingBuffer], 0, STREAM_BUFFER_SIZE);
    pStream->iPlayingBuffer = (pStream->iPlayingBuffer < MAX_STREAM_BUFFERS-1) ? pStream->iPlayingBuffer+1 : 0;

    // Enqueue Next Buffer
    if(pStream->FileOffset < pStream->FileLength) {
        int Read = fread(pStream->pBuffers[pStream->iQueueBuffer], 1, STREAM_BUFFER_SIZE, pStream->pFile);
        if(Read) {
            Result = QueueBuffer(pStream->pPlayer, pStream->pBuffers[pStream->iQueueBuffer], Read);
            ASSERT(Result == SL_RESULT_SUCCESS);

            pStream->iQueueBuffer = (pStream->iQueueBuffer < MAX_STREAM_BUFFERS-1) ? pStream->iQueueBuffer+1 : 0;
            pStream->FileOffset += Read;
        }
    }
}


/**************************************************************
 * A quick example of how to do streaming audio
 **************************************************************/
int SampleStream()
{
	SLresult            Result = SL_RESULT_SUCCESS;
    SLESSystemData      System;
    PlayerData          Player;  
    StreamData          Stream;
    SLuint32            i = 0;
    SLuint32            Read = 0;
    WavHeaderData       WavHeader;

	PRINTF("SAMPLE STREAM...");

    memset(&System, 0 , sizeof(System));
    memset(&Player, 0 , sizeof(Player));
    memset(&Stream, 0 , sizeof(Stream));

    // Init Stream Data
    for(i=0; i<MAX_STREAM_BUFFERS; ++i) {
        Stream.pBuffers[i] = (StreamData*) malloc(STREAM_BUFFER_SIZE);
        ASSERT(Stream.pBuffers[i]);
    }

    // Open a file and stream that data
    Stream.pFile = fopen("./media/stream.wav", "rb");   
    ASSERT(Stream.pFile);

    // Get the File Size
    Stream.FileOffset = ftell(Stream.pFile);    
    fseek(Stream.pFile, 0L, SEEK_END);
    Stream.FileLength = ftell(Stream.pFile);
    fseek(Stream.pFile, Stream.FileOffset, SEEK_SET);

    // Get Wav Information
    Read = fread(&WavHeader, sizeof(WavHeaderData), 1, Stream.pFile);
    Stream.FileOffset += Read;
    ASSERT(!memcmp(WavHeader.RiffTag, "RIFF", 4));

    // Initialize the SLES System
    Result = InitSLES(&System, SL_BOOLEAN_FALSE);
    ASSERT(Result == SL_RESULT_SUCCESS);

    // Create a Player
    Result = CreatePlayerStream(&Player, &System, WavHeader.SampleRate*1000, WavHeader.BitsPerSample, WavHeader.nChannels, StreamCallback, &Stream);
    ASSERT(Result == SL_RESULT_SUCCESS);
        

    // Enqueue a few buffers to kick it off
    Stream.iPlayingBuffer   = 0; 
    Stream.iQueueBuffer     = 0; 
    Stream.pPlayer          = &Player;

    for(i=0; i<3;++i) {
        if(Stream.FileOffset < Stream.FileLength) {
            Read = fread(Stream.pBuffers[Stream.iQueueBuffer], 1, STREAM_BUFFER_SIZE, Stream.pFile);
            if(Read) {
                Result = QueueBuffer(&Player, Stream.pBuffers[Stream.iQueueBuffer], Read);
                ASSERT(Result == SL_RESULT_SUCCESS);

                Stream.iQueueBuffer = (Stream.iQueueBuffer < MAX_STREAM_BUFFERS-1) ? Stream.iQueueBuffer+1 : 0;
                Stream.FileOffset += Read;
            }
        }
    }
    
    // The Buffers are queued so lets let them play for 10 seconds. 
    // Note that most of the action now is in the callback
    Result = Play(&Player);
    ASSERT(Result == SL_RESULT_SUCCESS);
    SLES_Sleep(10*1000);

    // Stop 
    Result = Stop(&Player);
    ASSERT(Result == SL_RESULT_SUCCESS);

    // Kill all survivers
    Result = DestroyPlayer(&Player);
    ASSERT(Result == SL_RESULT_SUCCESS);

    Result = DestroySLES(&System);
    ASSERT(Result == SL_RESULT_SUCCESS);

    // Clean up StreamData
    for(i=0; i<MAX_STREAM_BUFFERS; ++i) {
        if(Stream.pBuffers[i]) {
            free(Stream.pBuffers[i]);
            Stream.pBuffers[i] = NULL;
        }
    }
	
	PRINTF("...");
	
    return 0;
}


/**************************************************************
 * main
 **************************************************************/
int main()
{
    // Simply run through all samples back to back
	SampleSimple();
    Sample3D();
    SampleEffects();
    SampleStream();
}

