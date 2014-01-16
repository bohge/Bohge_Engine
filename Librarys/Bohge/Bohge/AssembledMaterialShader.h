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



//////////////////////////////////////////////////////////////
//		Shader的信息，包括uniform和attribute还有代码段		//
//////////////////////////////////////////////////////////////

#pragma once
#include "ShaderParameter.h"
#include "BaseShader.h"
#include <list>


namespace BohgeEngine
{
	class Device;
	class AssembledMaterialShader : public MaterialShader //自动合成的shader
	{
	private:
		struct Parameter
		{
			ShaderParameter::VariablesType	m_eType;
			ShaderParameter::InputsType		m_eInputType;
			std::string						m_ParamName;
			Parameter( const ShaderParameter::VariablesType& vt, const ShaderParameter::InputsType& it, const std::string& name )
				:m_eType(vt),m_eInputType(it),m_ParamName(name){}
		};
	private:
		typedef void (*UnifromFunction)(Device&, handle, const Material&);
	private:
		struct ShaderUniform
		{
			handle				m_hUniform;
			UnifromFunction		m_Function;
		};
	private:
		typedef std::list< Parameter >				ParameterList;
		typedef std::list< ShaderUniform* >			UnifromFunctionList;
	private:
		ParameterList			m_UniformList;
		ParameterList			m_AttributeList;
		std::string				m_VertexCode;
		std::string				m_FragmentCode;
		UnifromFunctionList		m_UnifromFunctionList;
	public:
		AssembledMaterialShader(void);
		virtual ~AssembledMaterialShader(void);
	private:
		UnifromFunction	_MappingUniformFunction(  const ShaderParameter::InputsType& it  );
		Attributes::ShaderAttributes _MappingAttributes( const ShaderParameter::InputsType& it );
	protected:
		virtual void _SetParameters();//使用这个shader设置shader所需要的全部属性和参数
	public:
		virtual bool Initialization();//初始化这个shader
	public:
		BOHGE_FORCEINLINE const Material& GetNode( ) const
		{
			return *m_pMaterial;
		}
		BOHGE_FORCEINLINE void PushBackVertexCode( const std::string& code )
		{
			m_VertexCode += code;
		}
		BOHGE_FORCEINLINE void PushBackFragmentCode( const std::string& code )
		{
			m_FragmentCode += code;
		}
		BOHGE_FORCEINLINE const std::string& GetVertexCode()
		{
			return m_VertexCode;
		}
		BOHGE_FORCEINLINE const std::string& GetFragmentCode()
		{
			return m_FragmentCode;
		}
		BOHGE_FORCEINLINE void PushAttribute( const ShaderParameter* sp )
		{
			m_AttributeList.push_back( Parameter( sp->GetVariableType(), sp->GetInputsType(), sp->GetName() ) );
		}
		BOHGE_FORCEINLINE void PushUniform( const ShaderParameter* sp )
		{
			m_UniformList.push_back( Parameter( sp->GetVariableType(), sp->GetInputsType(), sp->GetName() ) );
		}
	};
}
