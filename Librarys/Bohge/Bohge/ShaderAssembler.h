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



///////////////////////////////////////////
//		将shader片段拼装成完整shader		 //
///////////////////////////////////////////

#pragma once
#include "ShaderSegment.h"
#include "ShaderParameter.h"

#include <string>
#include <list>
#include <vector>
#include <map>

//这个complier整体写的不好，太多的指针的相互引用和传递，
//太多的莫名的成员变量，有时间要重构以下
namespace BohgeEngine
{
	class ShaderSegment;
	class AssembledMaterialShader;
	class ShaderAssembler
	{
	private:
		class ParameterNode //每一层的参数节点
		{
		private:
			std::string						m_VaryingName;//这个参数只有在它是一个varying的时候有效
			const ShaderParameter*			m_pFragmentNeeder;//这个参数只有在它是一个varying的时候有效
			const ShaderParameter*			m_pParameter;//记录参数
			ShaderSegment*					m_pProvider;//参数的提供者，如果是root层，则没有提供者，就是null
			std::list<ShaderSegment*>		m_pParamNeeders;//需要这个参数的语句段落
		public:
			ParameterNode( const ShaderParameter* sp, ShaderSegment* provider );//这个地方输入的是这个参数的提供者
			void FixParametersName( const std::string& add );//修改参数，避免重复
			BOHGE_FORCEINLINE void HaveNeeder( ShaderSegment* needer )//这个参数的需求者
			{
				m_pParamNeeders.push_back( needer );
			}
			BOHGE_FORCEINLINE void HaveProvider( ShaderSegment* provider )//这个参数的需求者
			{
				m_pProvider = provider;
			}
			BOHGE_FORCEINLINE const ShaderParameter* GetParameter() const
			{
				return m_pParameter;
			}
			BOHGE_FORCEINLINE ShaderSegment* GetProvider()
			{
				return m_pProvider;
			}
			BOHGE_FORCEINLINE void SetVaryingName( const std::string& name )
			{
				m_VaryingName = name;
			}
			BOHGE_FORCEINLINE const std::string& GetVaryingName()
			{
				return m_VaryingName;
			}
			BOHGE_FORCEINLINE void SetFragmentNeeder( const ShaderParameter* needer )
			{
				m_pFragmentNeeder = needer;
			}
			BOHGE_FORCEINLINE const ShaderParameter* GetFragmentNeeder()
			{
				return m_pFragmentNeeder;
			}
		};
		class ParameterLayer//参数层，shader有先后顺序的关系，这里按照先后顺序分层
		{
		public:
			typedef std::map< ShaderParameter::InputsType, ParameterNode* > NodeMap;
		private:
			NodeMap	m_ParameterNodeMap;//这一层所包含的全部参数
		public:
			~ParameterLayer(void);
		public:
			ParameterNode* InsertParameter( const ShaderParameter* sp, ShaderSegment* provider );//插入一个输入参数，输入的是参数的提供这，NULL为没有提供者
			ParameterNode* hasProvider( const ShaderParameter::InputsType& it );//是否含有it类型的输出
			void FixParametersName( const std::string& add );//修改参数，避免重复
			std::string AsVertexParameter( AssembledMaterialShader& shader ) const;//输出顶点shader的参数，顺便记录全部的参数
			std::string AsFragmentParameter( AssembledMaterialShader& shader ) const;//输出像素shader的参数，顺便记录全部的参数
			std::string AsVarying() const;//生成varying字符串
			std::string AsVaryingEquation() const;//在vs上对varying进行赋值的等式
			std::string AsEquaToVarying( ) const;//在ps上等式回本地变量
			BOHGE_FORCEINLINE NodeMap& GetParameters()
			{
				return m_ParameterNodeMap;
			}
			BOHGE_FORCEINLINE bool isInlcude( const ShaderParameter::InputsType& it ) const
			{
				NodeMap::const_iterator node = m_ParameterNodeMap.find( it );
				if ( m_ParameterNodeMap.end() != node )
				{
					return true;
				}
				return false;
			}
		};
	private:
		typedef std::vector< ParameterLayer >		ParameterLayers;
	public:
		ShaderAssembler(void);
		~ShaderAssembler(void);
	private:
		void _ComposeFragment( const ParameterLayer& root, ParameterLayer& varying, const Segments& ps, AssembledMaterialShader& shader );
		void _ComposeVertex( const ParameterLayer& root, ParameterLayer& varying, const Segments& vs, AssembledMaterialShader& shader );
		void _FixName( ParameterLayers& layer );
		void _LinkVaryingRoute( ParameterLayers& vs_layer, ParameterLayer& ps_root, ParameterLayer& varying );
		void _LinkParameterRoute( const Segments& vs, ParameterLayers& layer );//链接数据依赖关系
	public:
		void CompileShader( const Segments& vs, const Segments& ps, AssembledMaterialShader& shader, std::list<ShaderParameter::InputsType>& TextureList );
	};
}
