#include "SoundManagerSL.h"
#include "SoundPlayerSL.h"
#include "Decoder.h"

#ifdef _OPENSL
#include <SLES/OpenSLES.h>


namespace BohgeEngine
{
	//-------------------------------------------------------------------------------------------------------
	SoundManagerSL::SoundManagerSL(void)
	{
	}
	//-------------------------------------------------------------------------------------------------------
	SoundManagerSL::~SoundManagerSL(void)
	{
	}
	//-------------------------------------------------------------------------------------------------------
	void SoundManagerSL::_OnCreate()
	{
		//初始化系统
		SLresult result;

		// create engine
		result = slCreateEngine(&m_pEngineObject, 0, NULL, 0, NULL, NULL);
		ASSERT(SL_RESULT_SUCCESS == result);

		// realize the engine
		result = (*m_pEngineObject)->Realize(m_pEngineObject, SL_BOOLEAN_FALSE);
		ASSERT(SL_RESULT_SUCCESS == result);

		// get the engine interface
		result = (*m_pEngineObject)->GetInterface(m_pEngineObject, SL_IID_ENGINE, &m_pEngine);
		ASSERT(SL_RESULT_SUCCESS == result);

		// create output mix
		const SLInterfaceID ids[1] = {SL_IID_ENVIRONMENTALREVERB};
		const SLboolean req[1] = {SL_BOOLEAN_FALSE};
		result = (*m_pEngine)->CreateOutputMix(m_pEngine, &m_pMixObject, 1, ids, req);
		ASSERT(SL_RESULT_SUCCESS == result);

		// realize the output mix
		result = (*m_pMixObject)->Realize(m_pMixObject, SL_BOOLEAN_FALSE);
		ASSERT(SL_RESULT_SUCCESS == result);

		// get the environmental reverb interface
		SLEnvironmentalReverbSettings reverbSettings = SL_I3DL2_ENVIRONMENT_PRESET_STONECORRIDOR;
		result = (*m_pMixObject)->GetInterface(m_pMixObject, SL_IID_ENVIRONMENTALREVERB, &m_pEnvironmental);
		if (SL_RESULT_SUCCESS == result) 
		{
			result = (*m_pEnvironmental)->SetEnvironmentalReverbProperties(m_pEnvironmental, &reverbSettings);
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void SoundManagerSL::_OnDestroy()
	{
		//释放系统
	}
	//-------------------------------------------------------------------------------------------------------
	SoundPlayer* SoundManagerSL::CreatePlayer( int hash, int index, Decoder* res )
	{
		SLresult result;
		SoundPlayerSL* player = NEW SoundPlayerSL( hash, index, res );
		player->CreatePlayer( m_pEngine, m_pMixObject );
		return player;
	}
}
#endif