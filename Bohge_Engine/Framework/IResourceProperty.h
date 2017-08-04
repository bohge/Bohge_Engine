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
#pragma once

namespace BohgeEngine
{
	namespace IResourceProperty
	{
		enum SourceLoaderType
		{
			SLT_TEXTURE = 0,//加载一个纹理
			SLT_MESH,//网格
			SLT_SKELETON,//骨骼
		};

		enum DispatchThread//加载完毕的资源在其中一个线程进行done分发
		{
			DT_RENDER_THREAD = 0,
			DT_LOGIC_THREAD,
		};

		enum SourceType
		{
			CST_SHARED = 0,
			CST_MONOPOLY,
		};

		enum SharedSourceType
		{
			SST_TEXTURE = 0,
			SST_VERTEXBUFFER,
			SST_INDICESVUFFER,
			SST_SHADERPROGRAM,
			SST_RENDERTARGET,
			SST_MATERIAL,
			SST_RENDEROBJECT,
			SST_TEXTSET,
			SST_SCRIPT,

			SST_USERDEFINE,

			SST_COUNT,
		};

		enum SharedResourceType
		{
			SRT_TEXTURE = 0,
			SRT_VERTEXBUFFER,
			SRT_INDICESBUFFER,
			SRT_SHADERPROGRAM,
			SRT_RENDERTARGET,
			SRT_MATERIAL,
			SRT_RENDEROBJECT,
			SRT_TEXTSET,
			SRT_SCRIPT,

			SRT_USERDEFINE,

			SRT_COUNT,
		};
	}
}