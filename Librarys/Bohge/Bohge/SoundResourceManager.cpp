#include "Predefine.h"
#include "SoundResourceManager.h"
#include "SoundResourceDecoder.h"
#include "SoundResource.h"


namespace BohgeEngine
{

	SoundResourceManager* SoundResourceManager::m_pInstance;

	//-------------------------------------------------------------------------------------------------------
	void SoundResourceManager::Create()
	{
		m_pInstance = NEW SoundResourceManager();
	}
	//-------------------------------------------------------------------------------------------------------
	void SoundResourceManager::Destroy()
	{
		SAFE_DELETE( m_pInstance );
	}
	//-------------------------------------------------------------------------------------------------------
	SoundResourceManager::SoundResourceManager(void)
	{
	}
	//-------------------------------------------------------------------------------------------------------
	SoundResourceManager::~SoundResourceManager(void)
	{
	}
	//-------------------------------------------------------------------------------------------------------
	SoundResource* SoundResourceManager::LoadSoundResource( const std::string& path )
	{
		return NULL;
	}
}
