//--------------------------------------------------------------------------------------
// File:		sample_framework.c
//
// Desc:		This file wraps OpenSL ES to make it easier for writing sample code.
//
// Author:      QUALCOMM, Advanced Content Group - Snapdragon SDK
//
//              Copyright (c) 2011 QUALCOMM Incorporated. 
//                        All Rights Reserved. 
//                     QUALCOMM Proprietary/GTDR
//--------------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>

#if defined(_WIN32) || defined(WIN32)
#include <Windows.h>
#include <SLES/OpenSLES_SRSLabs.h>
#endif

#include <SLES/OpenSLES_Android.h>

#include "sample_framework.h"

#define MEDIA_PATH "./media" // no trailing '/'

/**************************************************************
 * Sleep Helper function
 **************************************************************/
void SLES_Sleep(unsigned int ms)
{
    Sleep(ms);
}

#if defined(_WIN32) || defined(WIN32)
// This will get the window handle of a console program. This not work on a windowed program.
static HWND GetConsoleHandle(void)
{
    const int   BufferSize   = 512;
    LPSTR       pOldTitle    = (LPSTR) calloc(BufferSize, sizeof(TCHAR));
    LPSTR       pNewTitle    = (LPSTR) calloc(BufferSize, sizeof(TCHAR));
    HWND        WindowHandle = NULL;

    // Get current title
    GetConsoleTitle(pOldTitle, BufferSize);

    // Create new unique title
    wsprintf(pNewTitle, TEXT("OpenSLES-123456"));
    SetConsoleTitle(pNewTitle);
    Sleep(10);                                         // Make sure the window get the new title before we continue
    WindowHandle = FindWindow(NULL, pNewTitle);        // Find the window with unique title
    SetConsoleTitle(pOldTitle);                        // Restore old title

    // Free buffers
    free(pOldTitle);
    free(pNewTitle);
    return(WindowHandle);
}
#endif // WIN32

/**************************************************************
 * Create the SLES Engine and Output Mix
 **************************************************************/
SLresult InitSLES( SLESSystemData* pSystem, SLboolean bAddPresetReverb )
{
    SLresult 			        Result = 0;
    SLuint32                    nInterfaces = 0;
    SLInterfaceID               Interfaces[10];
    SLboolean                   bRequired[10];
    SLAndroidConfigurationItf   ConfigItf = 0;

    PRINT(__FUNCTION__);

    if(!pSystem)
        return SL_RESULT_PARAMETER_INVALID;

	// Create Engine
#if defined WIN32 || defined _WIN32
    Interfaces[nInterfaces] = SL_IID_ANDROIDCONFIGURATION; bRequired[nInterfaces] = SL_BOOLEAN_TRUE; ++nInterfaces;
#endif

    slCreateEngine(&pSystem->EngineObj, 0, 0, nInterfaces, Interfaces, bRequired);
	ASSERT(Result == SL_RESULT_SUCCESS);

    Result = (*pSystem->EngineObj)->Realize(pSystem->EngineObj, SL_BOOLEAN_FALSE);
	ASSERT(Result == SL_RESULT_SUCCESS);

    Result = (*pSystem->EngineObj)->GetInterface(pSystem->EngineObj, SL_IID_ENGINE, &pSystem->EngineItf);
	ASSERT(Result == SL_RESULT_SUCCESS);

#if defined WIN32 || defined _WIN32
    {
        HWND hWnd = 0;
        hWnd = GetConsoleHandle();

        Result = (*pSystem->EngineObj)->GetInterface(pSystem->EngineObj, SL_IID_ANDROIDCONFIGURATION, &ConfigItf);
	    ASSERT(Result == SL_RESULT_SUCCESS);

        if(ConfigItf) {
            Result = (*ConfigItf)->SetConfiguration(ConfigItf, SL_SRSLABS_CONFIGKEY_WINDOW_HANDLE, &hWnd, sizeof(hWnd));
            ASSERT(Result == SL_RESULT_SUCCESS);
        }
    }
#endif // WIN32

    // Create Output Mix
    if(bAddPresetReverb)
    {
        Interfaces[nInterfaces] = SL_IID_PRESETREVERB; bRequired[nInterfaces] = SL_BOOLEAN_TRUE; nInterfaces++;
    }

    Result = (*pSystem->EngineItf)->CreateOutputMix(pSystem->EngineItf, &pSystem->OutputMixObj, nInterfaces, Interfaces, bRequired);
    ASSERT_GOTO_ERROR(Result == SL_RESULT_SUCCESS);

    Result = (*pSystem->OutputMixObj)->Realize(pSystem->OutputMixObj, SL_BOOLEAN_FALSE);
    ASSERT_GOTO_ERROR(Result == SL_RESULT_SUCCESS);

    Result = (*pSystem->OutputMixObj)->GetInterface(pSystem->OutputMixObj, SL_IID_OUTPUTMIX, &pSystem->OutputMixItf);
    ASSERT_GOTO_ERROR(Result == SL_RESULT_SUCCESS);

    if(bAddPresetReverb)
    {
        Result = (*pSystem->OutputMixObj)->GetInterface(pSystem->OutputMixObj, SL_IID_PRESETREVERB, &pSystem->PresetReverbItf);
        ASSERT_GOTO_ERROR(Result == SL_RESULT_SUCCESS);
    }

error:
    return Result;
}

/**************************************************************
 * Clears all memebers of pSystem. Does not free pSystem.
 **************************************************************/
SLresult DestroySLES( SLESSystemData* pSystem )
{
    PRINT(__FUNCTION__);

    if(!pSystem)
        return SL_RESULT_PARAMETER_INVALID;

    if(pSystem->ListenerObj)
	{
		(*pSystem->ListenerObj)->Destroy(pSystem->ListenerObj);
		pSystem->ListenerObj=NULL;
	}

	if(pSystem->OutputMixObj)
	{
		(*pSystem->OutputMixObj)->Destroy(pSystem->OutputMixObj);
		pSystem->OutputMixObj = 0;
	}

	if(pSystem->EngineObj)
	{
		(*pSystem->EngineObj)->Destroy(pSystem->EngineObj);
		pSystem->EngineObj = 0;
	}

    memset(pSystem, 0, sizeof(SLESSystemData));
    return SL_RESULT_SUCCESS;
}

/**************************************************************
 * Helper to get the mime type from a uri
 **************************************************************/
SLresult GetMimeType(const SLchar* pUri, SLchar* pMimeType)
{
    if(!pUri || ! pMimeType)
        return SL_RESULT_PARAMETER_INVALID;

    *pMimeType = '\0';
    if( strstr(pUri, ".wav") )        strcpy(pMimeType, "audio/wave");
    else if( strstr(pUri, ".ogg") )   strcpy(pMimeType, "audio/ogg");
    else if( strstr(pUri, ".mp3") )   strcpy(pMimeType, "audio/mpeg");

    return (*pMimeType != '\0') ? SL_RESULT_SUCCESS : SL_RESULT_PARAMETER_INVALID;
}

/**************************************************************
 * Create a Player Object for the simple sample
 **************************************************************/
SLresult CreatePlayerSimple(PlayerData* pPlayer, const SLESSystemData* pSystem, const SLchar* pzUri)
{
    SLresult					Result = 0;

	SLDataSource                DataSource;
	SLDataLocator_URI           DataLocatorURISource;
	SLDataFormat_MIME           DataFormatMIME;
    SLchar                      zMimeType[32];
    SLchar                      zFullUri[64];

	SLDataSink                  DataSink;
	SLDataLocator_OutputMix     DataLocatorSink;


	SLuint32                    nInterfaces = 0;
	SLInterfaceID               Interfaces[16];
	SLboolean                   bRequired[16];

    PRINT(__FUNCTION__);

    if(!pPlayer || !pSystem || !pzUri)
        return SL_RESULT_PARAMETER_INVALID;

    // Get the mime type
    Result = GetMimeType(pzUri, zMimeType);
    ASSERT(Result == SL_RESULT_SUCCESS);

    // Get the URI
    sprintf(zFullUri, "%s/%s", MEDIA_PATH, pzUri);

    // Setup the Source
	DataLocatorURISource.locatorType = SL_DATALOCATOR_URI;
	DataLocatorURISource.URI         = (SLchar*) zFullUri;
	DataFormatMIME.formatType        = SL_DATAFORMAT_MIME;
	DataFormatMIME.containerType     = SL_CONTAINERTYPE_WAV;
	DataFormatMIME.mimeType          = zMimeType;

	DataSource.pLocator			 	 = &DataLocatorURISource;
	DataSource.pFormat				 = &DataFormatMIME;

	// Setup Sink
	DataLocatorSink.locatorType   = SL_DATALOCATOR_OUTPUTMIX;
	DataLocatorSink.outputMix     = pSystem->OutputMixObj;

	DataSink.pLocator             = &DataLocatorSink;
	DataSink.pFormat              = 0; //Ignored with OutputMix

	// Explicit Interfaces
    Interfaces[nInterfaces] = SL_IID_VOLUME;    bRequired[nInterfaces] = SL_BOOLEAN_TRUE; nInterfaces++;
    Interfaces[nInterfaces] = SL_IID_SEEK;      bRequired[nInterfaces] = SL_BOOLEAN_TRUE; nInterfaces++;

    // Player
	Result = (*pSystem->EngineItf)->CreateAudioPlayer(pSystem->EngineItf, &pPlayer->Object, &DataSource, &DataSink, nInterfaces, Interfaces, bRequired);
	ASSERT_GOTO_ERROR(Result == SL_RESULT_SUCCESS);

	Result = (*(pPlayer->Object))->Realize(pPlayer->Object, SL_BOOLEAN_FALSE);
	ASSERT_GOTO_ERROR(Result == SL_RESULT_SUCCESS);

	// Cache Interfaces
    Result = (*(pPlayer->Object))->GetInterface(pPlayer->Object, SL_IID_PLAY, &pPlayer->PlayItf);
	ASSERT_GOTO_ERROR(Result == SL_RESULT_SUCCESS);
    
    Result = (*(pPlayer->Object))->GetInterface(pPlayer->Object, SL_IID_VOLUME, &pPlayer->VolumeItf);
	ASSERT_GOTO_ERROR(Result == SL_RESULT_SUCCESS);

    Result = (*(pPlayer->Object))->GetInterface(pPlayer->Object, SL_IID_SEEK, &pPlayer->SeekItf);
	ASSERT_GOTO_ERROR(Result == SL_RESULT_SUCCESS);

    // We are looping the sound sample so that we can keep the wave file size down.
    // You can remove this block if you dont want the sound to loop
    if(pPlayer->SeekItf)
    {
		SLmillisecond Dur = 0;
		Result = (*(pPlayer->PlayItf))->GetDuration(pPlayer->PlayItf, &Dur);
		ASSERT_GOTO_ERROR(Result == SL_RESULT_SUCCESS);
            	

		(*(pPlayer->SeekItf))->SetLoop(pPlayer->SeekItf, SL_BOOLEAN_TRUE, 0, Dur);
		ASSERT_GOTO_ERROR(Result == SL_RESULT_SUCCESS);	
    }

error:
    return Result;
}

/**************************************************************
 * Create a Player Object for the simple sample
 **************************************************************/
SLresult CreatePlayerEffects(PlayerData* pPlayer, const SLESSystemData* pSystem, SLuint32 Effects, const SLchar* pzUri)
{
    SLresult					Result = 0;

	SLDataSource                DataSource;
	SLDataLocator_URI           DataLocatorURISource;
	SLDataFormat_MIME           DataFormatMIME;
    SLchar                      zMimeType[32];
    SLchar                      zFullUri[64];

	SLDataSink                  DataSink;
	SLDataLocator_OutputMix     DataLocatorSink;

	SLuint32                    nInterfaces = 0;
	SLInterfaceID               Interfaces[16];
	SLboolean                   bRequired[16];

    if(!pPlayer || !pSystem || !pzUri)
        return SL_RESULT_PARAMETER_INVALID;

    // Get the mime type
    Result = GetMimeType(pzUri, zMimeType);
    ASSERT(Result == SL_RESULT_SUCCESS);

    // Get the URI
    sprintf(zFullUri, "%s/%s", MEDIA_PATH, pzUri);

    // Setup the Source
	DataLocatorURISource.locatorType = SL_DATALOCATOR_URI;
	DataLocatorURISource.URI         = zFullUri;
	DataFormatMIME.formatType        = SL_DATAFORMAT_MIME;
	DataFormatMIME.containerType     = SL_CONTAINERTYPE_WAV;
	DataFormatMIME.mimeType          = zMimeType;

	DataSource.pLocator			 	 = &DataLocatorURISource;
	DataSource.pFormat				 = &DataFormatMIME;

	// Setup Sink
	DataLocatorSink.locatorType   = SL_DATALOCATOR_OUTPUTMIX;
	DataLocatorSink.outputMix     = pSystem->OutputMixObj;

	DataSink.pLocator             = &DataLocatorSink;
	DataSink.pFormat              = 0; //Ignored with OutputMix

	// Explicit Interfaces
    Interfaces[nInterfaces] = SL_IID_VOLUME;    bRequired[nInterfaces] = SL_BOOLEAN_TRUE; nInterfaces++;
	Interfaces[nInterfaces] = SL_IID_SEEK;      bRequired[nInterfaces] = SL_BOOLEAN_TRUE; nInterfaces++;

    if(Effects & EFFECT_BASSBOOST) {
        Interfaces[nInterfaces] = SL_IID_BASSBOOST;    bRequired[nInterfaces] = SL_BOOLEAN_TRUE; nInterfaces++;
    }
    if(Effects & EFFECT_VIRTUALIZER) {
        Interfaces[nInterfaces] = SL_IID_VIRTUALIZER;  bRequired[nInterfaces] = SL_BOOLEAN_TRUE; nInterfaces++;
    }
    if(Effects & EFFECT_EQUALIZER) {
        Interfaces[nInterfaces] = SL_IID_EQUALIZER;    bRequired[nInterfaces] = SL_BOOLEAN_TRUE; nInterfaces++;
    }

    // Player
	Result = (*pSystem->EngineItf)->CreateAudioPlayer(pSystem->EngineItf, &pPlayer->Object, &DataSource, &DataSink, nInterfaces, Interfaces, bRequired);
	ASSERT_GOTO_ERROR(Result == SL_RESULT_SUCCESS);

	Result = (*(pPlayer->Object))->Realize(pPlayer->Object, SL_BOOLEAN_FALSE);
	ASSERT_GOTO_ERROR(Result == SL_RESULT_SUCCESS);

	// Cache Interfaces
    Result = (*(pPlayer->Object))->GetInterface(pPlayer->Object, SL_IID_PLAY, &pPlayer->PlayItf);
	ASSERT_GOTO_ERROR(Result == SL_RESULT_SUCCESS);

    Result = (*(pPlayer->Object))->GetInterface(pPlayer->Object, SL_IID_VOLUME, &pPlayer->VolumeItf);
	ASSERT_GOTO_ERROR(Result == SL_RESULT_SUCCESS);

	Result = (*(pPlayer->Object))->GetInterface(pPlayer->Object, SL_IID_SEEK, &pPlayer->SeekItf);
	ASSERT_GOTO_ERROR(Result == SL_RESULT_SUCCESS);
	
    if(Effects & EFFECT_BASSBOOST) {
        Result = (*(pPlayer->Object))->GetInterface(pPlayer->Object, SL_IID_BASSBOOST, &pPlayer->BassBoostItf);
	    ASSERT_GOTO_ERROR(Result == SL_RESULT_SUCCESS);
    }
    if(Effects & EFFECT_VIRTUALIZER) {
        Result = (*(pPlayer->Object))->GetInterface(pPlayer->Object, SL_IID_VIRTUALIZER, &pPlayer->VirtualizerItf);
	    ASSERT_GOTO_ERROR(Result == SL_RESULT_SUCCESS);
    }
    if(Effects & EFFECT_EQUALIZER) {
        Result = (*(pPlayer->Object))->GetInterface(pPlayer->Object, SL_IID_EQUALIZER, &pPlayer->EqualizerItf);
	    ASSERT_GOTO_ERROR(Result == SL_RESULT_SUCCESS);
    }

    if(pPlayer->SeekItf)
    {
		SLmillisecond Dur = 0;
		Result = (*(pPlayer->PlayItf))->GetDuration(pPlayer->PlayItf, &Dur);
		ASSERT(Result == SL_RESULT_SUCCESS);
            	

		(*(pPlayer->SeekItf))->SetLoop(pPlayer->SeekItf, SL_BOOLEAN_TRUE, 0, Dur);
		ASSERT(Result == SL_RESULT_SUCCESS);	
    }

error:
    return Result;
}

/**************************************************************
 * Clears all members of pPlayer. Does not free pPlayer.
 **************************************************************/
SLresult DestroyPlayer(PlayerData* pPlayer)
{
    PRINTF(__FUNCTION__);
    if(pPlayer)
    {
        if(pPlayer->Object)
        {
		    (*(pPlayer->Object))->Destroy(pPlayer->Object);
		    pPlayer->Object = 0;

        }

        memset(pPlayer, 0, sizeof(PlayerData));
        return SL_RESULT_SUCCESS;	
    }
    return SL_RESULT_PARAMETER_INVALID;
}

/**************************************************************
 * Create the SL Engine and Output Mix
 **************************************************************/
SLresult Play(PlayerData* pPlayer)
{
    PRINTF(__FUNCTION__);
    if(pPlayer && pPlayer->PlayItf)
    {
        return (*(pPlayer->PlayItf))->SetPlayState(pPlayer->PlayItf, SL_PLAYSTATE_PLAYING);		
    }
    return SL_RESULT_PARAMETER_INVALID;
}

/**************************************************************
 * Create the SL Engine and Output Mix
 **************************************************************/
SLresult Stop(PlayerData* pPlayer)
{
    PRINTF(__FUNCTION__);
    if(pPlayer && pPlayer->PlayItf)
    {
        return (*(pPlayer->PlayItf))->SetPlayState(pPlayer->PlayItf, SL_PLAYSTATE_STOPPED);		
    }
    return SL_RESULT_PARAMETER_INVALID;
}

/**************************************************************
 * Sets the Volume
 * pPlayer:    Pointer to the player
 * Volume:     Linear value between 0.0-1.0.
 **************************************************************/
SLresult SetVolume(PlayerData* pPlayer, float Volume)
{
    SLresult Result = SL_RESULT_FEATURE_UNSUPPORTED;

	// Clamp 0.0-1.0
    // TODO: Should we return SL_RESULT_PARAMETER_INVALID instead of clamping?
	if(Volume<0.0f) Volume = 0.0f;
	if(Volume>1.0f) Volume = 1.0f;

	if(pPlayer->VolumeItf)
	{
		SLmillibel  mBVolume = (Volume == 0.0f) ? SL_MILLIBEL_MIN : (SLmillibel)(TOMB(Volume)); // Note: mB != dB/1000. mB = 0.001B; dB = 0.1B

		Result = (*(pPlayer->VolumeItf))->SetVolumeLevel(pPlayer->VolumeItf, mBVolume);
		ASSERT(Result == SL_RESULT_SUCCESS);
	}

	return Result;
}

/**************************************************************
 * Creates a Listener Object that is stored in the System Data
 * pSystem:    Pointer to the System
 **************************************************************/
SLresult CreateListener( SLESSystemData* pSystem )
{
    SLresult        Result          = SL_RESULT_FEATURE_UNSUPPORTED;
    SLuint32        nInterfaces     = 0;
	SLInterfaceID   Interfaces[16];
	SLboolean       bRequired[16];

    if(!pSystem)
        return SL_RESULT_PARAMETER_INVALID;

    if(pSystem->ListenerObj)
        return SL_RESULT_PARAMETER_INVALID;

    // Add Interfaces
	Interfaces[nInterfaces] = SL_IID_3DLOCATION; bRequired[nInterfaces] = SL_BOOLEAN_TRUE; nInterfaces++;

	Result = (*(pSystem->EngineItf))->CreateListener(pSystem->EngineItf, &pSystem->ListenerObj, nInterfaces, Interfaces, bRequired);
	ASSERT_GOTO_ERROR(Result == SL_RESULT_SUCCESS);

    Result = (*(pSystem->ListenerObj))->Realize(pSystem->ListenerObj, SL_BOOLEAN_FALSE);
	ASSERT_GOTO_ERROR(Result == SL_RESULT_SUCCESS);

    Result = (*(pSystem->ListenerObj))->GetInterface(pSystem->ListenerObj, SL_IID_3DLOCATION, &pSystem->ListenerLocItf);
	ASSERT_GOTO_ERROR(Result == SL_RESULT_SUCCESS);

error:
    return Result;
}

/**************************************************************
 * Updates the Listener Positions and Rotation
 * pSystem:    Pointer to the System
 * PosX,Y,Z    Position for the Listener in meters
 * RotX,Y      Direction for the Listener.
 *             For simplicity we assume we are only rotating on the X-Y axis.
 **************************************************************/
SLresult SetListenerPosition(SLESSystemData* pSystem, float PosX, float PosY, float PosZ, float DirX, float DirY )
{
   SLresult     Result = SL_RESULT_FEATURE_UNSUPPORTED;
   SLVec3D      vSLUp;

   if(!pSystem)
        return SL_RESULT_PARAMETER_INVALID;

   if( !pSystem->ListenerObj || !pSystem->ListenerLocItf)
        return SL_RESULT_FEATURE_UNSUPPORTED;

   // Create SL Vectors (in mm)
   pSystem->vListenerPos.x = (SLint32)(PosX*1000);
   pSystem->vListenerPos.y = (SLint32)(PosY*1000);
   pSystem->vListenerPos.z = (SLint32)(PosZ*1000);

   pSystem->vListenerDir.x = (SLint32)(DirX*1000);
   pSystem->vListenerDir.y = (SLint32)(DirY*1000);
   pSystem->vListenerDir.z = 0;

   // Our up vector is always up the z axis in this example
   vSLUp.x = 0;
   vSLUp.y = 0;
   vSLUp.z = 1000;

   // Set the Position
   Result = (*(pSystem->ListenerLocItf))->SetLocationCartesian(pSystem->ListenerLocItf, &pSystem->vListenerPos);
   ASSERT_GOTO_ERROR(Result == SL_RESULT_SUCCESS);

   // Set the Orientation
   Result = (*(pSystem->ListenerLocItf))->SetOrientationVectors(pSystem->ListenerLocItf, &pSystem->vListenerDir, &vSLUp);
   ASSERT_GOTO_ERROR(Result == SL_RESULT_SUCCESS);

error:
   return Result;
}

SLresult CreatePlayer3d( PlayerData* pPlayer, const SLESSystemData* pSystem, const SLchar* pzUri )
{
    SLresult					Result = 0;

	SLDataSource                DataSource;
	SLDataLocator_URI           DataLocatorURISource;
	SLDataFormat_MIME           DataFormatMIME;
    SLchar                      zMimeType[32];
    SLchar                      zFullUri[64];

	SLDataSink                  DataSink;
	SLDataLocator_OutputMix     DataLocatorSink;

	SLuint32                    nInterfaces = 0;
	SLInterfaceID               Interfaces[16];
	SLboolean                   bRequired[16];

    if(!pPlayer || !pSystem || !pzUri)
        return SL_RESULT_PARAMETER_INVALID;

    // Get the mime type
    Result = GetMimeType(pzUri, zMimeType);
    ASSERT(Result == SL_RESULT_SUCCESS);

    // Get the URI
    sprintf(zFullUri, "%s/%s", MEDIA_PATH, pzUri);

    // Setup the Source
	DataLocatorURISource.locatorType = SL_DATALOCATOR_URI;
	DataLocatorURISource.URI         = zFullUri;
	DataFormatMIME.formatType        = SL_DATAFORMAT_MIME;
	DataFormatMIME.containerType     = SL_CONTAINERTYPE_WAV;
	DataFormatMIME.mimeType          = zMimeType;

	DataSource.pLocator			 	 = &DataLocatorURISource;
	DataSource.pFormat				 = &DataFormatMIME;

	// Setup Sink
	DataLocatorSink.locatorType   = SL_DATALOCATOR_OUTPUTMIX;
	DataLocatorSink.outputMix     = pSystem->OutputMixObj;

	DataSink.pLocator             = &DataLocatorSink;
	DataSink.pFormat              = 0; //Ignored with OutputMix

	// Explicit Interfaces
	Interfaces[nInterfaces] = SL_IID_VOLUME;      bRequired[nInterfaces] = SL_BOOLEAN_TRUE;  nInterfaces++;
    Interfaces[nInterfaces] = SL_IID_SEEK;        bRequired[nInterfaces] = SL_BOOLEAN_TRUE;  nInterfaces++;
    Interfaces[nInterfaces] = SL_IID_3DSOURCE;    bRequired[nInterfaces] = SL_BOOLEAN_FALSE; nInterfaces++;
    Interfaces[nInterfaces] = SL_IID_3DLOCATION;  bRequired[nInterfaces] = SL_BOOLEAN_FALSE; nInterfaces++;

    // Player
	Result = (*pSystem->EngineItf)->CreateAudioPlayer(pSystem->EngineItf, &pPlayer->Object, &DataSource, &DataSink, nInterfaces, Interfaces, bRequired);
	ASSERT_GOTO_ERROR(Result == SL_RESULT_SUCCESS);

	Result = (*(pPlayer->Object))->Realize(pPlayer->Object, SL_BOOLEAN_FALSE);
	ASSERT_GOTO_ERROR(Result == SL_RESULT_SUCCESS);

	// Cache Interfaces
    Result = (*(pPlayer->Object))->GetInterface(pPlayer->Object, SL_IID_PLAY, &pPlayer->PlayItf);
	ASSERT_GOTO_ERROR(Result == SL_RESULT_SUCCESS);

    Result = (*(pPlayer->Object))->GetInterface(pPlayer->Object, SL_IID_VOLUME, &pPlayer->VolumeItf);
	ASSERT_GOTO_ERROR(Result == SL_RESULT_SUCCESS);

    Result = (*(pPlayer->Object))->GetInterface(pPlayer->Object, SL_IID_SEEK, &pPlayer->SeekItf);
	ASSERT_GOTO_ERROR(Result == SL_RESULT_SUCCESS);

    // 3D Interfaces...It is ok if these fail.
    Result = (*(pPlayer->Object))->GetInterface(pPlayer->Object, SL_IID_3DSOURCE, &pPlayer->ThreeDSourceItf);
	Result = (*(pPlayer->Object))->GetInterface(pPlayer->Object, SL_IID_3DLOCATION, &pPlayer->ThreeDLocationItf);
	

    if(pPlayer->SeekItf)
    {
		SLmillisecond Dur = 0;
		Result = (*(pPlayer->PlayItf))->GetDuration(pPlayer->PlayItf, &Dur);
		ASSERT(Result == SL_RESULT_SUCCESS);
            	

		(*(pPlayer->SeekItf))->SetLoop(pPlayer->SeekItf, SL_BOOLEAN_TRUE, 0, Dur);
		ASSERT(Result == SL_RESULT_SUCCESS);	
    }

error:
    return Result;
}

/**************************************************************
 * Updates the Player's Positions
 * pPlayer:    Pointer to the Player
 * pSystem:    Pointer to the System
 * PosX,Y,Z    Position for the Listener in meters
 **************************************************************/
SLresult SetPosition( PlayerData* pPlayer, const SLESSystemData* pSystem, float PosX, float PosY, float PosZ)
{
    SLresult     Result  = SL_RESULT_FEATURE_UNSUPPORTED;

    if(!pPlayer)
        return SL_RESULT_PARAMETER_INVALID;

    // Use SLES 3D Render
    if(pPlayer->ThreeDLocationItf)
    {
        SLVec3D      vSLPos;

        // Create SL Vectors (in mm)
        vSLPos.x = (SLint32)(PosX * 1000.0f);
        vSLPos.y = (SLint32)(PosY * 1000.0f);
        vSLPos.z = (SLint32)(PosZ * 1000.0f);

        // Set the Position
        Result = (*(pPlayer->ThreeDLocationItf))->SetLocationCartesian(pPlayer->ThreeDLocationItf, &vSLPos);
        ASSERT_GOTO_ERROR(Result == SL_RESULT_SUCCESS);
    }
    else if(pPlayer->VolumeItf) // Pan and Fade
    {
        SLpermille Pan      = 0; // 0: Center, -1000 Left, 1000 Right
        SLmillibel Fade     = 0;

        if(!pSystem)
            return SL_RESULT_PARAMETER_INVALID;

        if( !pSystem->ListenerObj || !pSystem->ListenerLocItf)
            return SL_RESULT_FEATURE_UNSUPPORTED;

        // TODO: Calc Pan and Fade

	    Result =  (*pPlayer->VolumeItf)->SetStereoPosition(pPlayer->VolumeItf, Pan);
        ASSERT_GOTO_ERROR(Result == SL_RESULT_SUCCESS);

        Result =  SetVolume(pPlayer, Fade);
	    ASSERT_GOTO_ERROR(Result == SL_RESULT_SUCCESS);
    }

error:
    return Result;
}


SLresult CreatePlayerStream(PlayerData* pPlayer, const SLESSystemData* pSystem, SLmilliHertz SampleRate, SLuint32 BitsPerSample, SLuint32 nChannels,
                            slBufferQueueCallback Callback, void* pContext)
{
    SLresult					Result = 0;

	SLDataSource                DataSource;
	SLDataLocator_BufferQueue   BufferQueueLocator;
	SLDataFormat_PCM            PcmFormat;

	SLDataSink                  DataSink;
	SLDataLocator_OutputMix     DataLocatorSink;

	SLuint32                    nInterfaces = 0;
	SLInterfaceID               Interfaces[16];
	SLboolean                   bRequired[16];

    if(!pPlayer || !pSystem || !Callback)
        return SL_RESULT_PARAMETER_INVALID;

    
    // Setup the Source
	BufferQueueLocator.locatorType  = SL_DATALOCATOR_BUFFERQUEUE;
    BufferQueueLocator.numBuffers   = MAX_STREAM_BUFFERS;
    PcmFormat.formatType            = SL_DATAFORMAT_PCM;
    PcmFormat.bitsPerSample         = BitsPerSample;
    PcmFormat.samplesPerSec         = SampleRate;   // Note: Error in Spec. Really in  mHz not Hz
    PcmFormat.numChannels           = nChannels;
    PcmFormat.channelMask           = 0;
    PcmFormat.containerSize         = BitsPerSample;
    PcmFormat.endianness            = 1;

	DataSource.pLocator			 	 = &BufferQueueLocator;
	DataSource.pFormat				 = &PcmFormat;

	// Setup Sink
	DataLocatorSink.locatorType   = SL_DATALOCATOR_OUTPUTMIX;
	DataLocatorSink.outputMix     = pSystem->OutputMixObj;

	DataSink.pLocator             = &DataLocatorSink;
	DataSink.pFormat              = 0; //Ignored with OutputMix

	// Explicit Interfaces
	Interfaces[nInterfaces] = SL_IID_BUFFERQUEUE;      bRequired[nInterfaces] = SL_BOOLEAN_TRUE;  nInterfaces++;    

    // Player
	Result = (*pSystem->EngineItf)->CreateAudioPlayer(pSystem->EngineItf, &pPlayer->Object, &DataSource, &DataSink, nInterfaces, Interfaces, bRequired);
	ASSERT_GOTO_ERROR(Result == SL_RESULT_SUCCESS);

	Result = (*(pPlayer->Object))->Realize(pPlayer->Object, SL_BOOLEAN_FALSE);
	ASSERT_GOTO_ERROR(Result == SL_RESULT_SUCCESS);

	// Cache Interfaces
    Result = (*(pPlayer->Object))->GetInterface(pPlayer->Object, SL_IID_PLAY, &pPlayer->PlayItf);
	ASSERT_GOTO_ERROR(Result == SL_RESULT_SUCCESS);
        
    Result = (*(pPlayer->Object))->GetInterface(pPlayer->Object, SL_IID_BUFFERQUEUE, &pPlayer->BufferQueueItf);
	ASSERT_GOTO_ERROR(Result == SL_RESULT_SUCCESS);	

    if(pPlayer->BufferQueueItf) {
        (*pPlayer->BufferQueueItf)->RegisterCallback(pPlayer->BufferQueueItf, Callback, pContext);
    }

error:
    return Result;
}

SLresult QueueBuffer(PlayerData* pPlayer, void* pBuf, SLuint32 Size)
{
    SLresult	Result = SL_RESULT_SUCCESS;

    if(!pPlayer || !pBuf || !Size)
        return SL_RESULT_PARAMETER_INVALID;

    if(pPlayer->BufferQueueItf) {
        Result = (*pPlayer->BufferQueueItf)->Enqueue(pPlayer->BufferQueueItf, pBuf, Size);
        ASSERT_GOTO_ERROR(Result == SL_RESULT_SUCCESS);	
    }

error:
    return Result;
}
