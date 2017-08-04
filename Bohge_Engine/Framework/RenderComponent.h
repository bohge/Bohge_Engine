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
#include "RenderObjectEntity.h"
#include "IComponent.h"
#include "3DMath.h"



namespace BohgeEngine
{
	class BOHGE_FRAMEWORK RenderComponent : public IComponent
	{
		RTTI_DRIVER_TYPE_DECLEAR( RenderComponent, IComponent );
	private:
		Aabbox3df					m_OgiBindBox;
		Aabbox3df					m_TransBindBox[2];//交替
		RenderObjectEntity*			m_pRenderObject;
		void*						m_pUserData;//外部设置的用户自定义数据
		uint						m_eRenderProperty;
		bool						m_isDirtyBindBox;
		bool						m_isVisible;
		byte						m_BinaryIndex;
	public:
		RenderComponent(void);
		~RenderComponent(void);
	private:
		virtual void _DoOnAttachNode( );//当挂接在node上
	public:
		virtual void OnRootNodeChange( );//当所属node的根节点变化时
		virtual void Release( );//将自己释放掉
	public:
		void OnEntityLoaded( const SmartPtr<IAsyncSource>& sou );
		void MessageScale( const NodeMessage& message );
		void MessagePosition( const NodeMessage& message );
		void TryRecaculateBindBox();
	private:
		void _DoCaculateBindbox();
	public:
		void SetJobType( IJob::JobType jt );//资源的加载方式(同步异步)
		void PushMetadata( const IMetadata& meta );
		void SetTextureEntity( MaterialProperty::Material_Attribute att, const TextureEntity* te );
		void SetUserParameters( MaterialProperty::Material_Attribute att, MaterialProperty::UserParameterType spt, const Object& obj );
		void SetUserParametersPtr( MaterialProperty::Material_Attribute att, MaterialProperty::UserParameterType spt, const Object* ptr );
		void DependenceSource( IAsyncSource* source );
		void ConnectLoadedEvent( Delegate1<void, const SmartPtr<IAsyncSource>& >::FunctionType* ptr );
	public:
		BOHGE_FORCEINLINE bool isTransparent( int pt )
		{
			return m_pRenderObject->isTransparent( pt );
		}
		BOHGE_FORCEINLINE void SetRenderProperty( RenderObjectProperty::RenderProperty rp )
		{
			m_eRenderProperty |= rp;
		}
		BOHGE_FORCEINLINE void EraseRenderProperty( RenderObjectProperty::RenderProperty rp )
		{
			m_eRenderProperty &= ~rp;
		}
		BOHGE_FORCEINLINE bool isRenderProperty( RenderObjectProperty::RenderProperty rp )
		{
			return ( m_eRenderProperty & rp ) != 0;
		}
		BOHGE_FORCEINLINE bool isNotRenderProperty( RenderObjectProperty::RenderProperty rp )
		{
			return ( m_eRenderProperty & rp ) == 0;
		}
		BOHGE_FORCEINLINE void SetUserData( void* ptr )
		{
			m_pUserData = ptr;
		}
		template< typename T > T* GetUserData( )
		{
			return static_cast< T* >(m_pUserData);
		}
		BOHGE_FORCEINLINE uint GetMaterialID( int pt ) const
		{
			return m_pRenderObject->GetMaterialID( pt );
		}
		BOHGE_FORCEINLINE uint GetRenderQueue( int pt ) const
		{
			return m_pRenderObject->GetRenderQueue( pt );
		}
		BOHGE_FORCEINLINE void Draw( int pass ) 
		{
			m_pRenderObject->Draw( pass, _HostNode() );
		}
		BOHGE_FORCEINLINE void SetBindBox( const Aabbox3df& box )
		{
			m_OgiBindBox = box;
			m_TransBindBox[m_BinaryIndex] = m_OgiBindBox;
		}
		BOHGE_FORCEINLINE const Aabbox3df& GetBindBox( ) const
		{
			return m_TransBindBox[m_BinaryIndex];
		}
		BOHGE_FORCEINLINE void CreateResource( )//开始加载资源
		{
			m_pRenderObject->CreateResource();
		}
		BOHGE_FORCEINLINE void SetVisible( bool iv )
		{
			m_isVisible = iv;
		}
		BOHGE_FORCEINLINE bool isVisible()
		{
			return m_isVisible;
		}
		BOHGE_FORCEINLINE void ChangeVertexBuffer( const VertexStream& stream )//缓存更新
		{
			m_pRenderObject->ChangeVertexBuffer( stream );
		}
		BOHGE_FORCEINLINE void ChangeVertexBufferImmediately( const VertexStream& stream )//立即更新，必须在Render线程调用
		{
			m_pRenderObject->ChangeVertexBufferImmediately( stream );
		}
		BOHGE_FORCEINLINE void FlushVertexBuffer( const VertexStream& stream )//缓存更新
		{
			m_pRenderObject->FlushVertexBuffer( stream );
		}
		BOHGE_FORCEINLINE void FlushVertexBufferImmediately( const VertexStream& stream )//立即更新数据，必须在主线程调用
		{
			m_pRenderObject->FlushVertexBufferImmediately( stream );
		}
		BOHGE_FORCEINLINE void FlushIndiesBuffer( const IndicesStream& buffer )
		{
			m_pRenderObject->FlushIndiesBuffer( buffer);
		}
		BOHGE_FORCEINLINE void FlushIndiesBufferImmediately( const IndicesStream& buffer )
		{
			m_pRenderObject->FlushIndiesBufferImmediately( buffer);
		}
	};
}

