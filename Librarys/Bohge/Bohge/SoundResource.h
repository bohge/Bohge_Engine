#pragma once
#include <string>



namespace BohgeEngine
{
	class SoundResourceDecoder;
	class SoundResource//声音资源
	{
		//声音资源包在decoder的上层的用意是在加载资源的可以进行相同资源的加载管理
		//当一个声音资源被重复使用的时候，decoder不在流式加载资源，换言之不会再去释放旧资源
		//这样做可以用一定的空间资源换取时间
	private:
		SoundResourceDecoder*	m_pDecoder;
		int						m_nCurrentBufferPosition;//当前声音buffer的读取地址
	public:
		SoundResource( SoundResourceDecoder* decoder );
		~SoundResource(void);
	public:
	};
}
