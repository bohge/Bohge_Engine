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
#include "RenderObjectResource.h"
#include "RenderObjectSource.h"
#include "IAsyncEntity.h"



namespace BohgeEngine
{
	class BOHGE_FRAMEWORK RenderObjectEntity : public IAsyncEntity< RenderObjectResource, RenderObjectSource >
	{
		RTTI_DRIVER_TYPE_DECLEAR( RenderObjectEntity, IAsyncEntity );
	public:
		RenderObjectEntity(void);
		~RenderObjectEntity(void);
	private:
		virtual void _OnCreateResource();//准备加载资源的时候调用
		virtual void _OnResourceCreated();//资源加载完毕的回掉
	public:
		BOHGE_FORCEINLINE void SetTextureEntity( MaterialProperty::Material_Attribute att, const TextureEntity* te )
		{
			_GetResourcePtr()->SetTextureEntity( att, te );
		}
		BOHGE_FORCEINLINE void SetUserParameters( MaterialProperty::Material_Attribute att, MaterialProperty::UserParameterType spt, const Object& obj )
		{
			_GetSourcePtr()->SetUserParameters( att, spt, obj );
		}
		BOHGE_FORCEINLINE void SetUserParametersPtr( MaterialProperty::Material_Attribute att, MaterialProperty::UserParameterType spt, const Object* ptr )
		{
			_GetSourcePtr()->SetUserParametersPtr( att, spt, ptr );
		}
		BOHGE_FORCEINLINE void ChangeVertexBuffer( const VertexStream& stream )
		{
			_GetResourcePtr()->ChangeVertexBuffer( stream );
		}
		BOHGE_FORCEINLINE void ChangeVertexBufferImmediately( const VertexStream& stream )
		{
			_GetResourcePtr()->ChangeVertexBufferImmediately( stream );
		}
		BOHGE_FORCEINLINE void FlushVertexBuffer( const VertexStream& stream )
		{
			_GetResourcePtr()->FlushVertexBuffer( stream );
		}
		BOHGE_FORCEINLINE void FlushVertexBufferImmediately( const VertexStream& stream )
		{
			_GetResourcePtr()->FlushVertexBufferImmediately( stream );
		}
		BOHGE_FORCEINLINE void FlushIndiesBuffer( const IndicesStream& buffer )
		{
			_GetResourcePtr()->FlushIndiesBuffer( buffer);
		}
		BOHGE_FORCEINLINE void FlushIndiesBufferImmediately( const IndicesStream& buffer )
		{
			_GetResourcePtr()->FlushIndiesBufferImmediately( buffer);
		}
		BOHGE_FORCEINLINE uint GetMaterialID( int pt ) const
		{
			return _GetResourcePtr()->GetMaterialID( pt );
		}
		BOHGE_FORCEINLINE uint GetRenderQueue( int pt ) const
		{
			return _GetResourcePtr()->GetRenderQueue( pt );
		}
		BOHGE_FORCEINLINE bool isTransparent( int pt )
		{
			return _GetResourcePtr()->isTransparent( pt );
		}
		BOHGE_FORCEINLINE void Draw( int pass, Node* host )
		{
			_GetResourcePtr()->Draw( pass, host );
		}
		BOHGE_FORCEINLINE void Draw( int pass )
		{
			_GetResourcePtr()->Draw( pass, NULL );
		}
	};
}
