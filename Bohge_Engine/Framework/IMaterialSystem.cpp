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
#include "IMaterialSystem.h"
#include "Framework.h"


namespace BohgeEngine
{
	RTTI_TYPE_IMPLEMENT( IMaterialSystem );
	//-------------------------------------------------------------------------------------------------------
	IMaterialSystem::IMaterialSystem( void )
		:m_ePipelineType( (PipelineProperty::PiplineType)-1 ),
		m_pPassTypeArray(NULL),
		m_ActivePassType(0)
	{
		SYSTEM_SINGLETON_INITIALIZE;
	}
	//-------------------------------------------------------------------------------------------------------
	IMaterialSystem::~IMaterialSystem( void )
	{
		SYSTEM_SINGLETON_DESTROY;
	}
	//-------------------------------------------------------------------------------------------------------
	BOHGE_FORCEINLINE void IMaterialSystem::SetPipelienType( PipelineProperty::PiplineType pt, int apt )
	{
		ASSERT( 9 == PipelineProperty::PT_PASS_TYPE_COUNT );
		m_ePipelineType = pt;
		m_ActivePassType = apt;
		if ( m_ActivePassType & PipelineProperty::APT_LIGHTING_ALL )
		{
			m_pPassTypeArray->push_back( PipelineProperty::PT_LIGHTING_AMBIENT );
			m_pPassTypeArray->push_back( PipelineProperty::PT_LIGHTING_DIRECTION );
			m_pPassTypeArray->push_back( PipelineProperty::PT_LIGHTING_POINT );
			m_pPassTypeArray->push_back( PipelineProperty::PT_LIGHTING_SPOT );
		}
		if ( m_ActivePassType & PipelineProperty::APT_UNIVERSAL_DEPTH )
		{
			m_pPassTypeArray->push_back( PipelineProperty::PT_UNIVERSAL_DEPTH );
		}
		if ( m_ActivePassType & PipelineProperty::APT_UNIVERSAL_NORMAL )
		{
			m_pPassTypeArray->push_back( PipelineProperty::PT_UNIVERSAL_NORMAL );
		}
		if ( m_ActivePassType & PipelineProperty::APT_UNIVERSAL_DECAL )
		{
			m_pPassTypeArray->push_back( PipelineProperty::PT_UNIVERSAL_DECAL );
		}
		if ( m_ActivePassType & PipelineProperty::APT_UNIVERSAL_MULTI_RENDER_TARGET )
		{
			m_pPassTypeArray->push_back( PipelineProperty::PT_UNIVERSAL_MULTI_RENDER_TARGET );
		}
		if ( m_ActivePassType & PipelineProperty::APT_UNIVERSAL_POST_EFFECT )
		{
			m_pPassTypeArray->push_back( PipelineProperty::PT_UNIVERSAL_POST_EFFECT );
		}
		_OnPipelineTypeSet();
	}
	//-------------------------------------------------------------------------------------------------------
	void IMaterialSystem::RegistEvent( Framework& framework )
	{
		framework.RegisterEventListener( FrameworkEvent::ET_RENDER_ON_ENTRY, MEMBER_FUNC_PTR( &IMaterialSystem::OnFrameworkEntry ) );
		framework.RegisterEventListener( FrameworkEvent::ET_RENDER_ON_EXIT, MEMBER_FUNC_PTR( &IMaterialSystem::OnFrameworkExit ) );
	}
	//-------------------------------------------------------------------------------------------------------
	void IMaterialSystem::OnFrameworkEntry( float time, Framework& framework )
	{
		m_pPassTypeArray = NEW PipelineProperty::ActivePassTypeArray;
	}
	//-------------------------------------------------------------------------------------------------------
	void IMaterialSystem::OnFrameworkExit( float time, Framework& framework )
	{
		SAFE_DELETE( m_pPassTypeArray );
	}

}