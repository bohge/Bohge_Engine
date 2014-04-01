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