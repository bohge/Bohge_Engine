#pragma once
#include <string>


namespace BohgeEngine
{
	class SoundResource;
	class SoundResourceManager
	{
	public:
		enum Constant
		{
			SC_DEFUALT_SOUND_BUFFER_SIZE	= 1024 * 8,
		};
	private:
		static SoundResourceManager*		m_pInstance;
	public:
		static inline SoundResourceManager* Instance() { return m_pInstance; };
		static void Create();
		static void Destroy();
	private:
		SoundResourceManager(void);
		~SoundResourceManager(void);
	public:
		SoundResource* LoadSoundResource( const std::string& path );
	};
}
