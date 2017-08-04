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
#include "IMonopolyResource.h"
#include "ISharedResource.h"
#include "ISoundComponent.h"
#include "IResourceSystem.h"
#include "IDownloadHandle.h"
#include "IMaterialSystem.h"
#include "IThreadSpecific.h"
#include "IJobDispatcher.h"
#include "IThreadEntity.h"
#include "IThreadSystem.h"
#include "IThreadRWLock.h"
#include "IThreadEvent.h"
#include "IThreadMutex.h"
#include "ISoundSystem.h"
#include "IPostHandle.h"
#include "IWebSystem.h"
#include "IInserter.h"
#include "ILoader.h"
#include "IJob.h"




namespace BohgeEngine
{
	SYSTEM_SINGLETON_IMPLEMENT( IThreadSystem );
	SYSTEM_SINGLETON_IMPLEMENT( IWebSystem );
	SYSTEM_SINGLETON_IMPLEMENT( ISoundSystem );
	SYSTEM_SINGLETON_IMPLEMENT( IResourceSystem );
	SYSTEM_SINGLETON_IMPLEMENT( IMaterialSystem );

	RTTI_TYPE_IMPLEMENT( IInserter );
	RTTI_TYPE_IMPLEMENT( IJob );
	RTTI_TYPE_IMPLEMENT( IPostHandle );
	RTTI_TYPE_IMPLEMENT( IThreadEntity );
	RTTI_TYPE_IMPLEMENT( ThreadProperty );
	RTTI_TYPE_IMPLEMENT( IThreadSystem );
	RTTI_TYPE_IMPLEMENT( IJobDispatcher );
	RTTI_TYPE_IMPLEMENT( IThreadEvent );
	RTTI_TYPE_IMPLEMENT( IThreadMutex );
	RTTI_TYPE_IMPLEMENT( IThreadRWLock );
	RTTI_TYPE_IMPLEMENT( IThreadSpecific );
	RTTI_TYPE_IMPLEMENT( ISoundSystem );
	RTTI_TYPE_IMPLEMENT( ISoundComponent );
	RTTI_TYPE_IMPLEMENT( IResourceSystem );
	RTTI_TYPE_IMPLEMENT( IAsyncResource );
	RTTI_TYPE_IMPLEMENT( ISharedResource );
	RTTI_TYPE_IMPLEMENT( IMonopolyResource );
	RTTI_TYPE_IMPLEMENT( ILoader );
	RTTI_TYPE_IMPLEMENT( IWebSystem );
	RTTI_TYPE_IMPLEMENT( IDownloadHandle );
	RTTI_TYPE_IMPLEMENT( MaterialProperty );
}