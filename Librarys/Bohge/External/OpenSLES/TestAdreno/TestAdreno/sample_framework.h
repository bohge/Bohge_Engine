//--------------------------------------------------------------------------------------
// File:		sample_framework.h
//
// Desc:		This file wraps OpenSL ES to make it easier for writing sample code.
//
// Author:      QUALCOMM, Advanced Content Group - Snapdragon SDK
//
//              Copyright (c) 2011 QUALCOMM Incorporated. 
//                        All Rights Reserved. 
//                     QUALCOMM Proprietary/GTDR
//--------------------------------------------------------------------------------------

#ifndef SAMPLE_FRAMEWORK_H
#define SAMPLE_FRAMEWORK_H

#include <SLES/OpenSLES.h>

#define PI          (3.14159265f)
#define DEG2RAD(_d) ( (_d)*0.01745329f )

// dB Helpers
#include <math.h>
#define TODB(_v)		(float)(20.0f*log10f((_v)))
#define TOMB(_v)		(float)(2000.0f*log10f((_v)))
#define FROMDB(_v)	    (powf(10,(_v)/20.0f))


// Sleep Helper
void SLES_Sleep(unsigned int ms);

// Assert
#if defined(_DEBUG) || defined(DEBUG)
    #include <assert.h>
    #define ASSERT(_exp)    assert( (_exp) )
#else
    #define ASSERT(_exp)                
#endif

// If _exp is false, ASSERT() then goto error;
#define ASSERT_GOTO_ERROR(_exp)     if(!(_exp)) { ASSERT((_exp)); goto error;  }

// Quick Print Helpers
#include <stdio.h>
#define PRINT(_str) 		printf("%s\n", _str)
#define PRINTF(_str, ...)	printf(_str"\n", __VA_ARGS__)


// Simple struct to store data for the SLES System
typedef struct SLESSystemData_
{
    SLObjectItf         EngineObj;
    SLEngineItf         EngineItf;

    SLObjectItf         OutputMixObj;
    SLOutputMixItf      OutputMixItf;
    SLPresetReverbItf   PresetReverbItf;

    // Used for 3D Sample
    SLObjectItf			ListenerObj;
    SL3DLocationItf		ListenerLocItf;
    SLVec3D             vListenerPos;
    SLVec3D             vListenerDir;

} SLESSystemData;

// Simple struct to store our SL Player Object data
typedef struct PlayerData_
{
    SLObjectItf			Object;
    SLPlayItf			PlayItf;
    
    SLVolumeItf			VolumeItf;
    SLSeekItf           SeekItf;

    // Used for 3D Sample 
    SL3DSourceItf       ThreeDSourceItf;
    SL3DLocationItf     ThreeDLocationItf;

    // Used for Effects
    SLBassBoostItf      BassBoostItf;
    SLEqualizerItf      EqualizerItf;
    SLVirtualizerItf    VirtualizerItf;

    // Used for Streaming
    SLBufferQueueItf    BufferQueueItf;
} PlayerData;

enum 
{
    EFFECT_BASSBOOST    = (1<<1),
    EFFECT_EQUALIZER    = (1<<3),
    EFFECT_VIRTUALIZER  = (1<<4),
};


// Framework Functions
SLresult InitSLES( SLESSystemData* pSystem, SLboolean bAddPresetReverb);
SLresult DestroySLES( SLESSystemData* pSystem );
SLresult GetMimeType( const SLchar* pUri, SLchar* pMimeType );
SLresult Play( PlayerData* pPlayer );
SLresult Stop( PlayerData* pPlayer );
SLresult SetVolume( PlayerData* pPlayer, float Volume );

// Simple Sample
SLresult CreatePlayerSimple( PlayerData* pPlayer, const SLESSystemData* pSystem, const SLchar* pzUri );

// Effects Sample
SLresult CreatePlayerEffects(PlayerData* pPlayer, const SLESSystemData* pSystem, SLuint32 Effects, const SLchar* pzUri);

// 3D Sample
SLresult CreatePlayer3d( PlayerData* pPlayer, const SLESSystemData* pSystem, const SLchar* pzUri );
SLresult CreateListener( SLESSystemData* pSystem );
SLresult SetListenerPosition(SLESSystemData* pSystem, float PosX, float PosY, float PosZ, float DirX, float DirY );
SLresult SetPosition( PlayerData* pPlayer, const SLESSystemData* pSystem, float PosX, float PosY, float PosZ);

// Streaming Sample
#define MAX_STREAM_BUFFERS 10
SLresult CreatePlayerStream(PlayerData* pPlayer, const SLESSystemData* pSystem, 
                            SLmilliHertz SampleRate, SLuint32 BitsPerSample, SLuint32 nChannels,
                            slBufferQueueCallback Callback, void* pContext);
SLresult QueueBuffer(PlayerData* pPlayer, void* pBuf, SLuint32 Size);

SLresult DestroyPlayer(PlayerData* pPlayer);
#endif // SAMPLE_FRAMEWORK_H
