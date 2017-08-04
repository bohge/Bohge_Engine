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
#include "IAsyncSource.h"
#include "Log.h"



namespace BohgeEngine
{
	RTTI_TYPE_IMPLEMENT( IAsyncSource );
	//------------------------------------------------------------------------------------------------------
	IAsyncSource::IAsyncSource( IResourceProperty::SourceType cst, IResourceProperty::DispatchThread dt, bool hf )
		:m_eSourceType( cst ),
		m_pISourceMetadataList(NEW ISourceMetadataList),
		m_eDispatchThread( dt ),
		m_isHostingFree( hf ),
		m_Depends(1),
		m_nHashCode(0),
		m_FuncPtr(NULL),
		m_HasDelayDepends(false),
		m_pSourceLoadedCallBack( NEW Delegate1<void,const SmartPtr<IAsyncSource>& > ),
		m_pSourceLoadedDelayDependsCallBack( NEW Delegate1<void,const SmartPtr<IAsyncSource>& > )
	{
	}
	//------------------------------------------------------------------------------------------------------
	IAsyncSource::IAsyncSource( const IAsyncSource& rhs )
	{
		ASSERT( false );//禁止拷贝构造
	}
	//------------------------------------------------------------------------------------------------------
	IAsyncSource::~IAsyncSource(void)
	{
		ASSERT( NULL == m_pISourceMetadataList );
		SAFE_DELETE( m_FuncPtr );
		SAFE_DELETE( m_pSourceLoadedCallBack );
		SAFE_DELETE( m_pSourceLoadedDelayDependsCallBack );
	}
	//------------------------------------------------------------------------------------------------------
	void IAsyncSource::_DoYieldJob()
	{
		DEBUGLOG("IGeneralSource can't yield!\n");
		ASSERT(false);
	}
	//------------------------------------------------------------------------------------------------------
	void IAsyncSource::_DependenceReady( )
	{
		if ( 0 == -- m_Depends
			//&& m_pSourceLoadedCallBack
			)
		{
			if ( m_HasDelayDepends )
			{
				m_pSourceLoadedDelayDependsCallBack->Multicast( *m_pSelfReference );
				m_HasDelayDepends = false;//自动解除
			}
			else
			{
				m_pSourceLoadedCallBack->Multicast( *m_pSelfReference );
				SAFE_DELETE( m_pSourceLoadedCallBack );//节省内存，直接删除
				SAFE_DELETE( m_pSourceLoadedDelayDependsCallBack );
			}
		}
	}
	//------------------------------------------------------------------------------------------------------
	Delegate1<void, const SmartPtr<IAsyncSource>& >::FunctionType* IAsyncSource::DependencePushed()
	{
		return MEMBER_FUNC_PTR(&IAsyncSource::OnDependencePushed);
	}

	//------------------------------------------------------------------------------------------------------
	Delegate1<void, const SmartPtr<IAsyncSource>& >::FunctionType* IAsyncSource::DependenceLoaded()
	{
		return MEMBER_FUNC_PTR(&IAsyncSource::OnDependenceLoaded);
	}
	//------------------------------------------------------------------------------------------------------
	void IAsyncSource::OnDependencePushed( const SmartPtr<IAsyncSource>&  dep )
	{
		++ m_Depends;
	}
	//------------------------------------------------------------------------------------------------------
	void IAsyncSource::OnDependenceLoaded( const SmartPtr<IAsyncSource>&  dep )
	{
		//这个地方传进来的是其他资源的原始数据，而不是自己的，所以这里不能直接向下传递
		_DependenceReady( );
	}
	//------------------------------------------------------------------------------------------------------
	void IAsyncSource::ClearMetadata()
	{
		ASSERT( m_pISourceMetadataList );
		FOR_EACH( ISourceMetadataList, it, *m_pISourceMetadataList )
		{
			(*it)->ReleaseMetadate();
			SAFE_DELETE( *it );
		}
		m_pISourceMetadataList->clear();
		SAFE_DELETE( m_pISourceMetadataList );
	}
	//------------------------------------------------------------------------------------------------------
	uint IAsyncSource::_GetResourceHashCode() const
	{

		int index = 0;
		eastl::vector<byte> bytes;
		FOR_EACH_CONST( ISourceMetadataList, it, *m_pISourceMetadataList )
		{
			(*it)->GetIdentifier( bytes );
		}
		return Utility::HashCode( bytes );
	}
	//------------------------------------------------------------------------------------------------------
	void IAsyncSource::ReleaseUnnecessaryDataAfterLoaded()
	{
		_DoReleaseUnnecessaryDataAfterLoaded();
		ClearMetadata();
	}
}