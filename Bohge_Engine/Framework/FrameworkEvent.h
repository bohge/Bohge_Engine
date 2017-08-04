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
#include "VariableType.h"
#include "Delegate.hpp"


#include <EASTL/map.h>
#include <EASTL/string.h>

namespace BohgeEngine
{
	class IPlugin;
	class Framework;
	namespace FrameworkEvent
	{
		enum EventType
		{
			ET_LOGIC_ON_ENTRY = 0,
			ET_LOGIC_ON_EXIT,
			ET_LOGIC_BEGIN_UPDATE,
			ET_LOGIC_DO_UPDATE,
			ET_LOGIC_END_UPDATE,

			ET_RENDER_ON_ENTRY,
			ET_RENDER_ON_EXIT,
			ET_RENDER_BEGIN_RENDER,
			ET_RENDER_DO_RENDER,
			ET_RENDER_END_RENDER,
			ET_RENDER_SWAP_RENDER_BUFFER,
		};
		enum Constant
		{
			GC_LOGIC_DELEGATE_COUNT = 5,//这里的数据要与上面的对应
			GC_RENDER_DELEGATE_COUNT = 6,
			GC_RENDER_THREAD_INDEX = 0,
			GC_LOGIC_THREAD_INDEX = 1,
			GC_THREAD_COUNT = 2,
		};
		typedef eastl::list< eastl::pair< uint, IPlugin* > > IPluginList;
	}
	typedef Delegate2< void, float, Framework& > FrameworkDelegate;
	typedef Delegate1< void, Framework& > LanuchEvent;
}