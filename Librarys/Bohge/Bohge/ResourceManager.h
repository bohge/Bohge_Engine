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



/////////////////////////
//		资源读取	   //
/////////////////////////

#pragma once

#include <string>
#include <map>
#include "Device.h"


namespace BohgeEngine
{
	class Texture;
	class Texture2D;
	class TextureCube;
	class ModelData;
	class ResourceManager
	{
	public:
		ResourceManager();
		~ResourceManager();	
		//纹理相关
	private:
		typedef std::pair<TextureData*, int> TextureReference;
		typedef std::map<uint, TextureReference> TextureReferenceMap;
	private:
		TextureReferenceMap			m_TextureMap;
	public:
		const TextureData& LoadTexture2D( Device::PixelFormat pf, const std::string& filename);//读取纹理放入资源队列，只放入，没有使用者
		const TextureData& LoadTextureCube( Device::PixelFormat pf, const std::string& f, const std::string& b, const std::string& u, const std::string& d, const std::string& l, const std::string& r );
		void ReleaseTexture(Device::PixelFormat pf, const std::string& name);


		//模型RendBuffer相关
	private:
		typedef std::pair<ModelData*, int> ModelsReference;//模型mesh的索引个数
		typedef std::map<uint, ModelsReference> ModelsReferenceMap;//模型名字对应mesh的map
	private:
		ModelsReferenceMap			m_ModelMap;
	public:
		const ModelData& LoadModel( const std::string& name );
		void ReleaseModel( const std::string& name );
	};

}