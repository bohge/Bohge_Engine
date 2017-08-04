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
#include "Framework/Utility.hpp"
#include "ShaderAssembler.h"
#include "ShaderSegment.h"


using namespace eastl;
using namespace BohgeEngine;

#define ROOT_LAYER 0
#define LAYER_BEGIN (ROOT_LAYER+1)

namespace LuaMaterialSystemPlugin
{
	//-------------------------------------------------------------------------------------------------------
	ShaderAssembler::ParameterNode::ParameterNode( const ShaderParameter* sp, ShaderSegment* provider )
		:m_pParameter( sp ),
		m_pProvider( provider ),
		m_pFragmentNeeder(NULL)
	{

	}
	//-------------------------------------------------------------------------------------------------------
	void ShaderAssembler::ParameterNode::FixParametersName( const eastl::string& add )
	{
		string to = m_pParameter->GetName() + add;
		if ( NULL != m_pProvider )
		{
			m_pProvider->RenameOutputParameter( m_pParameter->GetInputsType(), to );
		}
		FOR_EACH( eastl::list<ShaderSegment*>, it, m_pParamNeeders )
		{
			(*it)->RenameInputParameter( m_pParameter->GetInputsType(), to );
		}
	}


	///////////////////////
	//		参数层		 //
	///////////////////////
	//-------------------------------------------------------------------------------------------------------
	ShaderAssembler::ParameterLayer::~ParameterLayer( void )
	{
		FOR_EACH( NodeMap, it, m_ParameterNodeMap )
		{
			SAFE_DELETE( it->second );
		}
	}
	//-------------------------------------------------------------------------------------------------------
	ShaderAssembler::ParameterNode* ShaderAssembler::ParameterLayer::InsertParameter( const ShaderParameter* sp, ShaderSegment* provider )
	{
#ifdef _DEBUG
		NodeMap::iterator it = m_ParameterNodeMap.find( sp->GetInputsType() );
		if( m_ParameterNodeMap.end() != it )
		{
			ASSERT( false );
		}
#endif
		ParameterNode* para = NEW ParameterNode( sp, provider );
		m_ParameterNodeMap.insert( make_pair( sp->GetInputsType(), para ) );
		return para;
	}
	//-------------------------------------------------------------------------------------------------------
	ShaderAssembler::ParameterNode* ShaderAssembler::ParameterLayer::hasProvider( const ShaderParameter::InputsType& it )
	{
		NodeMap::iterator node = m_ParameterNodeMap.find( it );
		if ( m_ParameterNodeMap.end() != node )
		{
			return node->second;
		}
		return NULL;
	}
	//-------------------------------------------------------------------------------------------------------
	void ShaderAssembler::ParameterLayer::FixParametersName( const eastl::string& add )
	{
		FOR_EACH( NodeMap, it, m_ParameterNodeMap )
		{
			it->second->FixParametersName( add );
		}
	}
	//-------------------------------------------------------------------------------------------------------
	eastl::string ShaderAssembler::ParameterLayer::AsVertexParameter( ShaderSource& shader ) const
	{
		string res;
		FOR_EACH_CONST( NodeMap, it, m_ParameterNodeMap )
		{
			res += it->second->GetParameter()->AsParameterString();
			if ( it->second->GetParameter()->isUniform() )
			{
				shader.PushUniform( it->second->GetParameter() );
			}
			else
			{
				shader.PushAttribute( it->second->GetParameter() );
			}
		}
		return res;
	}
	//-------------------------------------------------------------------------------------------------------
	eastl::string ShaderAssembler::ParameterLayer::AsFragmentParameter( ShaderSource& shader ) const
	{
		string res;
		FOR_EACH_CONST( NodeMap, it, m_ParameterNodeMap )
		{
			if ( NULL == it->second->GetProvider() )//这个的意思是在vs没有人提供这个参数
			{
				res += it->second->GetParameter()->AsParameterString();
				ASSERT( it->second->GetParameter()->isUniform() );//必须是这个
				shader.PushUniform( it->second->GetParameter() );
			}
		}
		return res;
	}
	//-------------------------------------------------------------------------------------------------------
	eastl::string ShaderAssembler::ParameterLayer::AsVarying() const
	{
		string res;
		FOR_EACH_CONST( NodeMap, it, m_ParameterNodeMap )
		{
			if ( NULL != it->second->GetProvider() )
			{
				res += it->second->GetParameter()->AsVaryingString( it->second->GetVaryingName() );
			}
		}
		return res;
	}
	//-------------------------------------------------------------------------------------------------------
	eastl::string ShaderAssembler::ParameterLayer::AsVaryingEquation() const
	{
		string res;
		FOR_EACH_CONST( NodeMap, it, m_ParameterNodeMap )
		{
			if ( NULL != it->second->GetProvider() )
			{
				//res += "\t" + it->second->GetVaryingName() + " = " + it->second->GetParameter()->GetName() + ";\r\n";
				res += it->second->GetParameter()->AsVaryingEquation( it->second->GetVaryingName() );
			}
		}
		return res;
	}
	//-------------------------------------------------------------------------------------------------------
	eastl::string ShaderAssembler::ParameterLayer::AsEquaToVarying( ) const
	{
		string res;
		FOR_EACH_CONST( NodeMap, it, m_ParameterNodeMap )
		{
			if ( NULL != it->second->GetProvider() )
			{
				res += it->second->GetFragmentNeeder()->AsVariables() + ";\r\n";
				res += it->second->GetFragmentNeeder()->AsEquaToVarying( it->second->GetVaryingName() );
			}
		}
		return res;
	}






	///////////////////////
	//		拼装器		 //
	///////////////////////
	//-------------------------------------------------------------------------------------------------------
	ShaderAssembler::ShaderAssembler(void)
	{
	}
	//-------------------------------------------------------------------------------------------------------
	ShaderAssembler::~ShaderAssembler(void)
	{
	}
	//-------------------------------------------------------------------------------------------------------
	void ShaderAssembler::CompileShader( const Segments& vs, const Segments& ps, ShaderSource& shader )
	{
		ParameterLayers vs_layer;//顶点参数层
		ParameterLayers ps_layer;//像素参数曾
		ParameterLayer varying_layer;//输入层

		vs_layer.resize( vs.size() + 1 );//+1是root层是对饮输入输出的
		ps_layer.resize( ps.size() + 1  );

		_LinkParameterRoute( vs, vs_layer );
		_LinkParameterRoute( ps, ps_layer );
		_FixName( vs_layer );
		_FixName( ps_layer );
		_LinkVaryingRoute( vs_layer, ps_layer[ROOT_LAYER], varying_layer );
		_ComposeVertex( vs_layer[ROOT_LAYER], varying_layer, vs, shader );
		_ComposeFragment( ps_layer[ROOT_LAYER], varying_layer, ps, shader );
	}
	//-------------------------------------------------------------------------------------------------------
	void ShaderAssembler::_LinkVaryingRoute( ParameterLayers& vs_layer, ParameterLayer& ps_root, ParameterLayer& varying )
	{
		char num = '0';
		string name("vParameter");
		name.push_back( num );
		ParameterLayer::NodeMap& ps_input = ps_root.GetParameters();
		FOR_EACH( ParameterLayer::NodeMap, par, ps_input )
		{
			bool isProvided = false;
			for ( int i = vs_layer.size()-1; i >= 0 ; i -- )
			{
				ParameterNode* provider = vs_layer[i].hasProvider( par->first );
				if ( NULL != provider )//如果在vs有人提供，就是用vs的
				{
					isProvided = true;
					name[name.size()-1] = num;
					ParameterNode* node = varying.InsertParameter( provider->GetParameter(), provider->GetProvider() );
					node->SetFragmentNeeder( par->second->GetParameter() );
					node->SetVaryingName( name );
					par->second->HaveProvider( provider->GetProvider() );
					num++;
					break;
				}
			}
			if ( !isProvided )//如果没人提供
			{
				varying.InsertParameter( par->second->GetParameter(), NULL );
			}
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void ShaderAssembler::_LinkParameterRoute( const Segments& vs, ParameterLayers& layer )
	{
		int layerIndex = LAYER_BEGIN;
		bool firstlayer = true;
		FOR_EACH_CONST( Segments, seg, vs )
		{
			const ParameterMap& inputs = (*seg)->GetSegmentInput();//得到第一段需要的输入
			const ParameterMap& output = (*seg)->GetSegmentOutput();
			if ( firstlayer )//如果第一次进入，将全部的参数输入当作原始需求
			{
				firstlayer = false;
				FOR_EACH_CONST( ParameterMap, par, inputs )//迭代将数据设置为原始输入
				{
					ParameterNode* provider = layer[ROOT_LAYER].InsertParameter( par->second, NULL );//向外部需求的，没有提供者
					provider->HaveNeeder( *seg );
				}
				FOR_EACH_CONST( ParameterMap, par, output )
				{
					layer[layerIndex].InsertParameter( par->second, (*seg) );//输入的是提供者的语句段
				}
			}
			else//如果不是第一次进入，需要逐层递归寻找提供者，如果最后还是找不到，就需要在根输入中加入这个属性
			{
				FOR_EACH_CONST( ParameterMap, par, inputs )//迭代将数据设置为原始输入
				{
					bool isProvided = false;
					for ( int i = layerIndex-1; i >= 0 ; i -- )
					{
						ParameterNode* provider = layer[i].hasProvider( par->first );
						if ( NULL != provider )
						{
							isProvided = true;
							provider->HaveNeeder( *seg );
							break;
						}
					}
					if ( !isProvided )//如果没人提供
					{
						ParameterNode* provider = layer[ROOT_LAYER].InsertParameter( par->second, NULL );//给根节点添加一个需求者
						provider->HaveNeeder( *seg );
					}
				}
				FOR_EACH_CONST( ParameterMap, par, output )
				{
					layer[layerIndex].InsertParameter( par->second, *seg );//输入的是提供者的语句段
				}
			}
			layerIndex++;
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void ShaderAssembler::_FixName( ParameterLayers& layer )
	{
		char fix = '0';
		FOR_EACH( ParameterLayers, it, layer )
		{
			string fixstr;
			fixstr.push_back( fix );
			it->FixParametersName( fixstr );
			fix++;
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void ShaderAssembler::_ComposeVertex( const ParameterLayer& root, ParameterLayer& varying, const Segments& vs, ShaderSource& shader )
	{
		shader.PushBackVertexCode( root.AsVertexParameter( shader ) );//这个代码写的太恶心了，有空一定要重构
		shader.PushBackVertexCode( "\r\n\r\n\r\n" );
		shader.PushBackVertexCode( varying.AsVarying() );
		shader.PushBackVertexCode( "\r\n\r\n\r\nvoid main()\r\n{\r\n" );
		FOR_EACH_CONST( Segments, it, vs )
		{
			shader.PushBackVertexCode( (*it)->GetCode() );
			shader.PushBackVertexCode( "\r\n" );
		}
		shader.PushBackVertexCode( varying.AsVaryingEquation() );
		shader.PushBackVertexCode( "\r\n}\r\n" );
	}
	//-------------------------------------------------------------------------------------------------------
	void ShaderAssembler::_ComposeFragment( const ParameterLayer& root, ParameterLayer& varying, const Segments& ps, ShaderSource& shader )
	{
		shader.PushBackFragmentCode( varying.AsFragmentParameter( shader ) );//这个代码写的太恶心了，有空一定要重构
		shader.PushBackFragmentCode( "\r\n\r\n\r\n" );
		shader.PushBackFragmentCode( varying.AsVarying() );
		shader.PushBackFragmentCode( "\r\n\r\n\r\nvoid main()\r\n{\r\n" );
		shader.PushBackFragmentCode( varying.AsEquaToVarying() );
		FOR_EACH_CONST( Segments, it, ps )
		{
			shader.PushBackFragmentCode( (*it)->GetCode() );
			shader.PushBackFragmentCode( "\r\n" );
		}
		shader.PushBackFragmentCode( "\r\n}\r\n" );
	}

}


